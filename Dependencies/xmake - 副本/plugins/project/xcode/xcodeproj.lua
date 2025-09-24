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
-- @author      JXMaster
-- @file        xcodeproj.lua
--

-- imports
import("get_xcode_info")
import("pbxproj")

function make(outputdir)

    -- collect xcode info.
    -- see get_xcode_info.lua file for detailed description
    -- of the returned info table.
    local info = get_xcode_info(outputdir)

    -- create xcode dir.
    local project_bundle = path.join(info.project_dir, info.project_bundle)
    os.mkdir(project_bundle)

    -- create .pbxproj file.
    pbxproj(info)
end
