from __future__ import annotations

import math
import sys
import time

from Source.Terrain3D import Terrain3d
from Source.VoxelWorld import AIR
from Source.VoxelWorld import DIRT
from Source.VoxelWorld import VoxelWorld


class FpsViewer:
    def __init__(
        self,
        terrain: Terrain3d,
        voxel_world: VoxelWorld | None = None,
        width: int = 960,
        height: int = 540,
        render_scale: int = 3,
        voxel_stride: int = 2,
        yaw_deg: float = 0.0,
        pitch_deg: float = 6.0,
        fov_deg: float = 80.0,
        step: float = 1.0,
        max_distance: float | None = None,
        fog_strength: float = 0.85,
        eye_height: float | None = None,
        height_exaggeration: float = 1.8,
        block_size: int = 1,
        normal_y_scale: float = 1.2,
        move_speed: float = 18.0,
        turn_speed: float = 90.0,
        fps: int = 20,
    ) -> None:
        self._terrain = terrain
        self._world = voxel_world if voxel_world is not None else VoxelWorld.from_terrain(terrain)
        self._width = width
        self._height = height
        self._voxel_stride = max(1, min(6, int(voxel_stride)))
        self._render_scale = max(1, min(12, int(render_scale)))
        internal_scale = max(1, self._render_scale * self._voxel_stride)
        self._render_width = max(64, width // internal_scale)
        self._render_height = max(64, height // internal_scale)
        self._yaw_deg = yaw_deg
        self._pitch_deg = pitch_deg
        self._fov_deg = fov_deg
        self._step = step
        self._max_distance = max_distance
        self._fog_strength = fog_strength
        self._eye_height = eye_height
        self._height_exaggeration = max(0.1, height_exaggeration)
        self._block_size = max(1, int(block_size))
        self._normal_y_scale = max(0.1, normal_y_scale)
        self._move_speed = move_speed
        self._turn_speed = turn_speed
        self._fps = max(1, min(240, fps))

        self._cam_x = (self._world.world_width - 1) * 0.5
        self._cam_z = (self._world.world_depth - 1) * 0.15
        self._cam_y = 0.0

        self._last_hit: tuple[int, int, int, tuple[int, int, int]] | None = None
        self._pending_updates: list[tuple[int, int, int, int]] = []

    def run(self) -> None:
        try:
            import moderngl
            import moderngl_window as mglw
            from moderngl_window import geometry
        except Exception as e:  # pragma: no cover
            raise RuntimeError("缺少 GPU 渲染依赖：moderngl / moderngl-window / pyglet。请先运行 install.bat 或 pip install -r requirements.txt") from e

        viewer = self

        class _GpuApp(mglw.WindowConfig):
            gl_version = (3, 3)
            title = "RandomMapGeneration GPU Viewer"
            window_size = (viewer._width, viewer._height)
            aspect_ratio = None
            resizable = False
            vsync = True

            def __init__(self, **kwargs):
                super().__init__(**kwargs)
                self._keys_down: set[int] = set()
                self._last_t = time.perf_counter()
                self._last_present = self._last_t

                w = viewer._world.world_width
                h = viewer._world.cfg.size_y
                d = viewer._world.world_depth

                blocks_data = viewer._world.to_dense_r8()
                self._blocks_tex = self.ctx.texture3d((w, h, d), 1, data=blocks_data, dtype="u1")
                self._blocks_tex.filter = (moderngl.NEAREST, moderngl.NEAREST)
                self._blocks_tex.repeat_x = False
                self._blocks_tex.repeat_y = False
                self._blocks_tex.repeat_z = False
                self._has_voxels = any(blocks_data)

                self._scene_prog = self.ctx.program(
                    vertex_shader="""
                        #version 330
                        in vec2 in_position;
                        in vec2 in_texcoord_0;
                        out vec2 v_uv;
                        void main() {
                            v_uv = in_texcoord_0;
                            gl_Position = vec4(in_position, 0.0, 1.0);
                        }
                    """,
                    fragment_shader="""
                        #version 330
                        uniform usampler3D blocks;
                        uniform vec3 world_size;
                        uniform vec3 camera_pos;
                        uniform vec3 cam_forward;
                        uniform vec3 cam_right;
                        uniform vec3 cam_up;
                        uniform float tan_half_fov;
                        uniform float aspect;
                        uniform float max_distance;
                        uniform float fog_strength;
                        uniform vec2 inv_resolution;
                        in vec2 v_uv;
                        out vec4 f_color;

                        vec3 sky_color(float t) {
                            vec3 sky_top = vec3(80.0, 140.0, 220.0) / 255.0;
                            vec3 sky_horizon = vec3(220.0, 240.0, 255.0) / 255.0;
                            vec3 sky_bottom = vec3(170.0, 220.0, 255.0) / 255.0;
                            if (t < 0.55) {
                                return mix(sky_top, sky_horizon, t / 0.55);
                            }
                            return mix(sky_horizon, sky_bottom, (t - 0.55) / 0.45);
                        }

                        vec3 block_color(int id) {
                            if (id == 1) return vec3(95.0, 170.0, 70.0) / 255.0;
                            if (id == 2) return vec3(130.0, 110.0, 70.0) / 255.0;
                            if (id == 3) return vec3(120.0, 120.0, 125.0) / 255.0;
                            return vec3(1.0, 0.0, 1.0);
                        }

                        bool in_bounds(ivec3 p) {
                            return p.x >= 0 && p.y >= 0 && p.z >= 0
                                && p.x < int(world_size.x) && p.y < int(world_size.y) && p.z < int(world_size.z);
                        }

                        void main() {
                            vec2 p = v_uv * 2.0 - 1.0;
                            float px = p.x * tan_half_fov;
                            float py = p.y * tan_half_fov / max(1e-6, aspect);
                            vec3 rd = normalize(cam_forward + cam_right * px + cam_up * py);
                            vec3 ro = camera_pos;

                            ivec3 map = ivec3(floor(ro));
                            ivec3 stepv = ivec3(sign(rd));
                            vec3 inv_rd = 1.0 / max(abs(rd), vec3(1e-6));
                            vec3 delta = inv_rd;

                            vec3 side;
                            side.x = (rd.x >= 0.0) ? (float(map.x + 1) - ro.x) * delta.x : (ro.x - float(map.x)) * delta.x;
                            side.y = (rd.y >= 0.0) ? (float(map.y + 1) - ro.y) * delta.y : (ro.y - float(map.y)) * delta.y;
                            side.z = (rd.z >= 0.0) ? (float(map.z + 1) - ro.z) * delta.z : (ro.z - float(map.z)) * delta.z;

                            vec3 normal = vec3(0.0);
                            float t = 0.0;
                            bool hit = false;
                            int hit_id = 0;

                            for (int i = 0; i < 2048; i++) {
                                if (t > max_distance) break;
                                if (in_bounds(map)) {
                                    uint v = texelFetch(blocks, map, 0).r;
                                    if (v != 0u) {
                                        hit = true;
                                        hit_id = int(v);
                                        break;
                                    }
                                }

                                if (side.x < side.y) {
                                    if (side.x < side.z) {
                                        map.x += stepv.x;
                                        t = side.x;
                                        side.x += delta.x;
                                        normal = vec3(-float(stepv.x), 0.0, 0.0);
                                    } else {
                                        map.z += stepv.z;
                                        t = side.z;
                                        side.z += delta.z;
                                        normal = vec3(0.0, 0.0, -float(stepv.z));
                                    }
                                } else {
                                    if (side.y < side.z) {
                                        map.y += stepv.y;
                                        t = side.y;
                                        side.y += delta.y;
                                        normal = vec3(0.0, -float(stepv.y), 0.0);
                                    } else {
                                        map.z += stepv.z;
                                        t = side.z;
                                        side.z += delta.z;
                                        normal = vec3(0.0, 0.0, -float(stepv.z));
                                    }
                                }
                            }

                            vec3 col;
                            vec3 sky = sky_color(1.0 - v_uv.y);
                            if (!hit) {
                                col = sky;
                            } else {
                                vec3 base = block_color(hit_id);
                                vec3 light = normalize(vec3(0.55, 0.85, 0.25));
                                float shade = 0.38 + 0.75 * max(0.0, dot(normal, light));
                                vec3 lit = base * shade;
                                float fog = pow(clamp(t / max(1e-6, max_distance), 0.0, 1.0), 1.15);
                                fog = clamp(fog * fog_strength, 0.0, 1.0);
                                col = mix(lit, vec3(220.0, 240.0, 255.0) / 255.0, fog);
                            }

                            vec2 d = abs(v_uv - vec2(0.5));
                            float thickness_x = inv_resolution.x * 1.5;
                            float thickness_y = inv_resolution.y * 1.5;
                            if (d.x < thickness_x && d.y < 0.02) col = vec3(1.0);
                            if (d.y < thickness_y && d.x < 0.02) col = vec3(1.0);

                            f_color = vec4(col, 1.0);
                        }
                    """,
                )
                self._scene_prog["blocks"].value = 0
                self._scene_prog["world_size"].value = (float(w), float(h), float(d))

                self._render_size = (viewer._render_width, viewer._render_height)
                self._color_tex = self.ctx.texture(self._render_size, 4)
                self._color_tex.filter = (moderngl.NEAREST, moderngl.NEAREST)
                self._fbo = self.ctx.framebuffer(color_attachments=[self._color_tex])

                self._blit_prog = self.ctx.program(
                    vertex_shader="""
                        #version 330
                        in vec2 in_position;
                        in vec2 in_texcoord_0;
                        out vec2 v_uv;
                        void main() {
                            v_uv = in_texcoord_0;
                            gl_Position = vec4(in_position, 0.0, 1.0);
                        }
                    """,
                    fragment_shader="""
                        #version 330
                        uniform sampler2D tex;
                        in vec2 v_uv;
                        out vec4 f_color;
                        void main() {
                            f_color = texture(tex, v_uv);
                        }
                    """,
                )
                self._blit_prog["tex"].value = 0
                self._quad = geometry.quad_fs()

            def key_event(self, key, action, modifiers):  # type: ignore[override]
                k = self.wnd.keys
                press = getattr(k, "ACTION_PRESS", 1)
                release = getattr(k, "ACTION_RELEASE", 0)
                repeat = getattr(k, "ACTION_REPEAT", None)
                if action == press or action == 1 or (repeat is not None and action == repeat):
                    self._keys_down.add(key)
                elif action == release or action == 0:
                    self._keys_down.discard(key)

                if self._pressed(action) and key == getattr(k, "ESCAPE", None):
                    self.wnd.close()
                    return
                if self._pressed(action) and key == getattr(k, "F", None):
                    viewer._break_block()
                    return
                if self._pressed(action) and key == getattr(k, "G", None):
                    viewer._place_block()
                    return
                if self._pressed(action) and key == getattr(k, "B", None):
                    viewer._block_size = 1 if viewer._block_size > 1 else 3
                    return
                if self._pressed(action) and key in {getattr(k, "NUMBER_1", -1), getattr(k, "KEY_1", -2), getattr(k, "ONE", -3)}:
                    viewer._voxel_stride = 1
                    viewer._recompute_render_size()
                    return
                if self._pressed(action) and key in {getattr(k, "NUMBER_2", -1), getattr(k, "KEY_2", -2), getattr(k, "TWO", -3)}:
                    viewer._voxel_stride = 2
                    viewer._recompute_render_size()
                    return
                if self._pressed(action) and key in {getattr(k, "NUMBER_3", -1), getattr(k, "KEY_3", -2), getattr(k, "THREE", -3)}:
                    viewer._voxel_stride = 3
                    viewer._recompute_render_size()
                    return
                if self._pressed(action) and key in {getattr(k, "NUMBER_4", -1), getattr(k, "KEY_4", -2), getattr(k, "FOUR", -3)}:
                    viewer._voxel_stride = 4
                    viewer._recompute_render_size()
                    return

            def unicode_char_entered(self, char):  # type: ignore[override]
                if char in {"f", "F"}:
                    viewer._break_block()
                elif char in {"g", "G"}:
                    viewer._place_block()

            def _pressed(self, action: int) -> bool:
                k = self.wnd.keys
                press = getattr(k, "ACTION_PRESS", 1)
                repeat = getattr(k, "ACTION_REPEAT", None)
                return action == press or action == 1 or (repeat is not None and action == repeat)

            def _down(self, *names: str) -> bool:
                k = self.wnd.keys
                is_key_pressed = getattr(self.wnd, "is_key_pressed", None)
                for name in names:
                    code = getattr(k, name, None)
                    if code is None:
                        continue
                    if callable(is_key_pressed):
                        try:
                            if is_key_pressed(code):
                                return True
                        except Exception:
                            pass
                    if code in self._keys_down:
                        return True
                return False

            def on_render(self, _t: float, _dt: float):  # type: ignore[override]
                now = time.perf_counter()
                dt = max(0.0, min(0.1, now - self._last_t))
                self._last_t = now
                self._last_present = now

                move = viewer._move_speed * dt
                turn = viewer._turn_speed * dt

                if self._down("LEFT"):
                    viewer._yaw_deg -= turn
                if self._down("RIGHT"):
                    viewer._yaw_deg += turn
                if self._down("UP"):
                    viewer._pitch_deg -= turn * 0.6
                if self._down("DOWN"):
                    viewer._pitch_deg += turn * 0.6

                if self._down("Q"):
                    viewer._pitch_deg += turn
                if self._down("E"):
                    viewer._pitch_deg -= turn

                viewer._pitch_deg = max(-25.0, min(25.0, viewer._pitch_deg))

                yaw_rad = math.radians(viewer._yaw_deg)
                fwd_x = math.sin(yaw_rad)
                fwd_z = math.cos(yaw_rad)
                right_x = fwd_z
                right_z = -fwd_x

                if self._down("W"):
                    viewer._cam_x += fwd_x * move
                    viewer._cam_z += fwd_z * move
                if self._down("S"):
                    viewer._cam_x -= fwd_x * move
                    viewer._cam_z -= fwd_z * move
                if self._down("A"):
                    viewer._cam_x -= right_x * move
                    viewer._cam_z -= right_z * move
                if self._down("D"):
                    viewer._cam_x += right_x * move
                    viewer._cam_z += right_z * move

                viewer._cam_x = max(0.0, min((viewer._world.world_width - 1), viewer._cam_x))
                viewer._cam_z = max(0.0, min((viewer._world.world_depth - 1), viewer._cam_z))
                viewer._update_camera_y()

                viewer._last_hit = _raycast_center(viewer._world, (viewer._cam_x, viewer._cam_y, viewer._cam_z), viewer._yaw_deg, viewer._pitch_deg, max_distance=64.0)
                hit_text = ""
                if viewer._last_hit is not None:
                    hx, hy, hz, _hn = viewer._last_hit
                    hit_text = f" hit=({hx},{hy},{hz})"
                vox_text = " vox=ok" if self._has_voxels else " vox=empty"
                self.wnd.title = f"RandomMapGeneration GPU Viewer   x={viewer._cam_x:.1f} z={viewer._cam_z:.1f} yaw={viewer._yaw_deg:.1f} pitch={viewer._pitch_deg:.1f}{hit_text}{vox_text}"
                self._flush_pending_updates()
                self._ensure_render_targets()

                yaw = math.radians(viewer._yaw_deg)
                pitch = math.radians(viewer._pitch_deg)
                forward = _normalize3(math.sin(yaw) * math.cos(pitch), -math.sin(pitch), math.cos(yaw) * math.cos(pitch))
                right = _normalize3(math.cos(yaw), 0.0, -math.sin(yaw))
                up = _normalize3(
                    forward[1] * right[2] - forward[2] * right[1],
                    forward[2] * right[0] - forward[0] * right[2],
                    forward[0] * right[1] - forward[1] * right[0],
                )

                tan_half = math.tan(math.radians(viewer._fov_deg) * 0.5)
                aspect = viewer._render_width / max(1, viewer._render_height)

                self._fbo.use()
                self.ctx.viewport = (0, 0, viewer._render_width, viewer._render_height)
                self.ctx.clear(0.0, 0.0, 0.0, 1.0)

                self._blocks_tex.use(0)
                self._scene_prog["camera_pos"].value = (viewer._cam_x, viewer._cam_y, viewer._cam_z)
                self._scene_prog["cam_forward"].value = forward
                self._scene_prog["cam_right"].value = right
                self._scene_prog["cam_up"].value = up
                self._scene_prog["tan_half_fov"].value = float(tan_half)
                self._scene_prog["aspect"].value = float(aspect)
                self._scene_prog["max_distance"].value = float(64.0 if viewer._max_distance is None else viewer._max_distance)
                self._scene_prog["fog_strength"].value = float(viewer._fog_strength)
                self._scene_prog["inv_resolution"].value = (1.0 / viewer._render_width, 1.0 / viewer._render_height)
                self._quad.render(self._scene_prog)

                self.wnd.use()
                self.ctx.viewport = (0, 0, viewer._width, viewer._height)
                self._color_tex.use(0)
                self._quad.render(self._blit_prog)

            def _flush_pending_updates(self) -> None:
                if not viewer._pending_updates:
                    return
                for x, y, z, block_id in viewer._pending_updates:
                    self._blocks_tex.write(bytes([block_id & 0xFF]), viewport=(x, y, z, 1, 1, 1))
                viewer._pending_updates.clear()

            def _ensure_render_targets(self) -> None:
                target_size = (viewer._render_width, viewer._render_height)
                if target_size == self._render_size:
                    return
                self._render_size = target_size
                self._color_tex.release()
                self._fbo.release()
                self._color_tex = self.ctx.texture(self._render_size, 4)
                self._color_tex.filter = (moderngl.NEAREST, moderngl.NEAREST)
                self._fbo = self.ctx.framebuffer(color_attachments=[self._color_tex])

        argv0 = sys.argv[0] if sys.argv else "main"
        old_argv = sys.argv[:]
        sys.argv = [argv0]
        try:
            mglw.run_window_config(_GpuApp)
        finally:
            sys.argv = old_argv

    def _update_camera(self, dt: float) -> None:
        move = self._move_speed * dt
        turn = self._turn_speed * dt

        _ = (move, turn)

    def _update_camera_y(self) -> None:
        x = int(round(self._cam_x))
        z = int(round(self._cam_z))
        top = self._world.top_solid_y(x, z)
        base = max(0, top)
        eye = self._eye_height if self._eye_height is not None else 2.0
        self._cam_y = base + eye

    def _recompute_render_size(self) -> None:
        internal_scale = max(1, self._render_scale * self._voxel_stride)
        self._render_width = max(64, self._width // internal_scale)
        self._render_height = max(64, self._height // internal_scale)

    def _break_block(self) -> None:
        hit = self._last_hit
        if hit is None:
            hit = _raycast_center(self._world, (self._cam_x, self._cam_y, self._cam_z), self._yaw_deg, self._pitch_deg, max_distance=64.0)
        if hit is None:
            return
        x, y, z, _n = hit
        self._world.set_block(x, y, z, AIR)
        self._pending_updates.append((x, y, z, AIR))

    def _place_block(self) -> None:
        hit = self._last_hit
        if hit is None:
            hit = _raycast_center(self._world, (self._cam_x, self._cam_y, self._cam_z), self._yaw_deg, self._pitch_deg, max_distance=64.0)
        if hit is None:
            return
        x, y, z, n = hit
        tx = x + n[0]
        ty = y + n[1]
        tz = z + n[2]
        if self._world.get_block(tx, ty, tz) == AIR:
            self._world.set_block(tx, ty, tz, DIRT)
            self._pending_updates.append((tx, ty, tz, DIRT))


def _raycast_center(world: VoxelWorld, camera: tuple[float, float, float], yaw_deg: float, pitch_deg: float, max_distance: float = 6.0) -> tuple[int, int, int, tuple[int, int, int]] | None:
    yaw = math.radians(yaw_deg)
    pitch = math.radians(pitch_deg)
    dir_x = math.sin(yaw) * math.cos(pitch)
    dir_z = math.cos(yaw) * math.cos(pitch)
    dir_y = -math.sin(pitch)
    hit = world.raycast(camera, (dir_x, dir_y, dir_z), max_distance=max_distance)
    if hit is None:
        return None
    return (hit.x, hit.y, hit.z, hit.normal)


def _normalize3(x: float, y: float, z: float) -> tuple[float, float, float]:
    l = math.sqrt(x * x + y * y + z * z)
    if l <= 1e-9:
        return (0.0, 1.0, 0.0)
    return (x / l, y / l, z / l)
