--!A cross-platform build utility based on Lua
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--
-- Copyright (C) 2015-present, Xmake Open Source Community.
--
-- @author      ruki
-- @file        tidy.lua
--

-- imports
import("core.base.option")
import("core.base.task")
import("core.base.semver")
import("core.project.config")
import("core.project.project")
import("lib.detect.find_tool")
import("async.runjobs")
import("utils.progress")
import("private.action.require.impl.packagenv")
import("private.action.require.impl.install_packages")

-- the clang.tidy options
local options = {
    {"l", "list",       "k",  nil,  "Show the clang-tidy checks list."},
    {"j", "jobs",       "kv", tostring(os.default_njob()),
                                    "Set the number of parallel check jobs."},
    {"q", "quiet",      "k",  nil,  "Run clang-tidy in quiet mode."},
    {nil, "fix",        "k",  nil,  "Apply suggested fixes."},
    {nil, "fix_errors", "k",  nil,  "Apply suggested errors fixes."},
    {nil, "fix_notes",  "k",  nil,  "Apply suggested notes fixes."},
    {nil, "create",     "k",  nil,  "Create a .clang-tidy file."},
    {nil, "configfile", "kv", nil,  "Specify the path of .clang-tidy or custom config file"},
    {nil, "compdb",     "kv", nil,  "Specify the path of the compile_commands.json file"},
    {nil, "checks",     "kv", nil,  "Set the given checks.",
                                    "e.g.",
                                    "    - xmake check clang.tidy --checks=\"*\""},
    {"f", "files",      "kv", nil,  "Set files path with pattern",
                                    "e.g.",
                                    "    - xmake check clang.tidy -f src/main.c",
                                    "    - xmake check clang.tidy -f 'src/*.c" .. path.envsep() .. "src/**.cpp'"},
    {nil, "targets",    "vs",  nil,  "Check the sourcefiles of the given target.",
                                    ".e.g",
                                    "    - xmake check clang.tidy",
                                    "    - xmake check clang.tidy [targets]"}
}

-- show checks list
function _show_list(clang_tidy)
    os.execv(clang_tidy, {"--list-checks"})
end

-- create .clang-tidy config file
function _create_config(clang_tidy, opt)
    local projectdir = project.directory()
    local argv = {"--dump-config"}
    if opt.checks then
        table.insert(argv, "--checks=" .. opt.checks)
    end
    os.execv(clang_tidy, argv, {stdout = path.join(projectdir, ".clang-tidy"), curdir = projectdir})
end

-- add sourcefiles in target
function _add_target_files(sourcefiles, target)
    for _, sourcebatch in pairs(target:sourcebatches()) do
        -- we can only use rulename to filter them because sourcekind may be bound to multiple rules
        local rulename = sourcebatch.rulename
        if rulename == "c.build" or rulename == "c++.build"
            or rulename == "objc.build" or rulename == "objc++.build"
            or rulename == "cuda.build" or rulename == "c++.build.modules" then
            table.join2(sourcefiles, sourcebatch.sourcefiles)
        end
    end
end

function _run_clang_tidy(clang_tidy, argv, opt)
    -- https://github.com/llvm/llvm-project/pull/120547
    if clang_tidy.version and semver.compare(clang_tidy.version, "19.1.6") > 0 and #argv > 10 then
        local argsfile = os.tmpfile() .. ".args.txt"
        io.writefile(argsfile, os.args(argv))
        argv = {"@" .. argsfile}
        os.vrunv(clang_tidy.program, argv, opt)
        os.rm(argsfile)
    else
        os.vrunv(clang_tidy.program, argv, opt)
    end
end

-- check sourcefiles
function _check_sourcefiles(clang_tidy, sourcefiles, opt)
    opt = opt or {}
    local projectdir = project.directory()
    local argv = {}
    if opt.checks then
        table.insert(argv, "--checks=" .. opt.checks)
    end
    if opt.fix then
        table.insert(argv, "--fix")
    end
    if opt.fix_errors then
        table.insert(argv, "--fix-errors")
    end
    if opt.fix_notes then
        table.insert(argv, "--fix-notes")
    end
    if opt.compdbfile then
        table.insert(argv, "-p")
        table.insert(argv, opt.compdbfile)
    end
    if opt.configfile then
        table.insert(argv, "--config-file=" .. opt.configfile)
    end
    if opt.quiet then
        table.insert(argv, "--quiet")
    end

    -- split sourcefiles
    local arguments_maxn = 32
    local sourcefiles_argv = {}
    local sourcefiles_jobs = {}
    for _, sourcefile in ipairs(sourcefiles) do
        if not path.is_absolute(sourcefile) then
            sourcefile = path.absolute(sourcefile, projectdir)
        end
        table.insert(sourcefiles_argv, sourcefile)
        if #sourcefiles_argv >= arguments_maxn then
            table.insert(sourcefiles_jobs, sourcefiles_argv)
            sourcefiles_argv = {}
        end
    end
    if #sourcefiles_argv > 0 then
        table.insert(sourcefiles_jobs, sourcefiles_argv)
    end

    -- run clang-tidy
    runjobs("checker.tidy", function (index, total, opt)
        local tidy_argv = sourcefiles_jobs[index]
        progress.show(index * 100 / total, "clang-tidy.analyzing %s .. %d", tidy_argv[1], #tidy_argv)
        _run_clang_tidy(clang_tidy, tidy_argv, {curdir = projectdir})
    end, {total = #sourcefiles_jobs, comax = opt.jobs or os.default_njob()})
end

-- do check
function _check(clang_tidy, opt)
    opt = opt or {}

    -- generate compile_commands.json first
    local filepath = option.get("compdb")
    if not filepath then
        -- @see https://github.com/xmake-io/xmake/issues/5583#issuecomment-2337696628
        local outputdir
        local extraconf = project.extraconf("target.rules", "plugin.compile_commands.autoupdate")
        if extraconf then
            outputdir = extraconf.outputdir
        end
        if outputdir then
            filepath = path.join(outputdir, "compile_commands.json")
        end
    end
    if not filepath then
        filepath = "compile_commands.json"
    end
    if not os.isfile(filepath) then
        local outputdir = os.tmpfile() .. ".dir"
        local filename = path.filename(filepath)
        filepath = outputdir and path.join(outputdir, filename) or filename
        task.run("project", {quiet = true, kind = "compile_commands", lsp = "clangd", outputdir = outputdir})
    end
    opt.compdbfile = path.absolute(filepath)

    -- get sourcefiles
    local sourcefiles = {}
    if opt.files then
        local files = path.splitenv(opt.files)
        for _, file in ipairs(files) do
            for _, filepath in ipairs(os.files(file)) do
                table.insert(sourcefiles, filepath)
            end
        end
    else
        local targetnames = opt.targets
        if targetnames then
            for _, targetname in ipairs(targetnames) do
                local target = assert(project.target(targetname), "unknown target(%s)", targetname)
                _add_target_files(sourcefiles, target)
            end
        else
            for _, target in ipairs(project.ordertargets()) do
                _add_target_files(sourcefiles, target)
            end
        end
    end

    -- check files
    _check_sourcefiles(clang_tidy, sourcefiles, opt)
end

function main(argv)

    -- parse arguments
    local args = option.parse(argv or {}, options, "Use clang-tidy to check project code."
                                           , ""
                                           , "Usage: xmake check clang.tidy [options]")

    -- enter the environments of llvm
    local oldenvs = packagenv.enter("llvm")

    -- find clang-tidy
    local packages = {}
    local clang_tidy = find_tool("clang-tidy", {version = true})
    if not clang_tidy then
        table.join2(packages, install_packages("llvm"))
    end

    -- enter the environments of installed packages
    for _, instance in ipairs(packages) do
        instance:envs_enter()
    end

    -- we need to force detect and flush detect cache after loading all environments
    if not clang_tidy then
        clang_tidy = find_tool("clang-tidy", {force = true, version = true})
    end
    assert(clang_tidy, "clang-tidy not found!")

    -- list checks
    if args.list then
        _show_list(clang_tidy.program)
    elseif args.create then
        _create_config(clang_tidy.program, args)
    else
        _check(clang_tidy, args)
    end
    os.setenvs(oldenvs)
end

