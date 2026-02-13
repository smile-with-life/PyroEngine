from __future__ import annotations

import math
import time
import tkinter as tk

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
        pitch_deg: float = -6.0,
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
        min_scale = 8
        self._render_scale = max(min_scale, min(12, render_scale))
        self._render_width = max(64, width // self._render_scale)
        self._render_height = max(64, height // self._render_scale)
        self._voxel_stride = max(1, min(6, int(voxel_stride)))
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
        self._fps = max(1, min(120, fps))
        self._fps = min(self._fps, 15)

        self._cam_x = (self._world.world_width - 1) * 0.5
        self._cam_z = (self._world.world_depth - 1) * 0.15
        self._cam_y = 0.0

        self._keys: set[str] = set()
        self._last_t = time.perf_counter()
        self._last_hit: tuple[int, int, int, tuple[int, int, int]] | None = None

        self._root = tk.Tk()
        self._root.title("RandomMapGeneration FPS Viewer")
        self._root.geometry(f"{width}x{height}")
        self._root.resizable(False, False)

        self._canvas = tk.Canvas(self._root, width=width, height=height, highlightthickness=0)
        self._canvas.pack(fill="both", expand=True)
        self._img_item = self._canvas.create_image(0, 0, anchor="nw")
        cx = width // 2
        cy = height // 2
        self._crosshair_h = self._canvas.create_line(cx - 8, cy, cx + 8, cy, fill="white")
        self._crosshair_v = self._canvas.create_line(cx, cy - 8, cx, cy + 8, fill="white")
        self._hud_item = self._canvas.create_text(
            10,
            10,
            anchor="nw",
            fill="white",
            text="WASD 移动  方向键转头  Q/E 上下看  F 破坏  G 放置  B 方块  +/- 起伏  1-4 清晰度  ESC 退出",
            font=("Consolas", 12),
        )
        self._photo_raw = tk.PhotoImage(width=self._render_width, height=self._render_height)
        self._photo: tk.PhotoImage | None = None

        self._root.bind("<KeyPress>", self._on_key_down)
        self._root.bind("<KeyRelease>", self._on_key_up)
        self._root.protocol("WM_DELETE_WINDOW", self._root.destroy)

    def run(self) -> None:
        self._root.after(0, self._tick)
        self._root.mainloop()

    def _on_key_down(self, event: tk.Event) -> None:
        key = str(event.keysym)
        if key == "Escape":
            self._root.destroy()
            return
        if key in ("f", "F"):
            self._break_block()
            return
        if key in ("g", "G"):
            self._place_block()
            return
        if key in ("b", "B"):
            self._block_size = 1 if self._block_size > 1 else 3
            return
        if key in ("1", "2", "3", "4"):
            self._voxel_stride = int(key)
            return
        if key in ("plus", "equal"):
            self._height_exaggeration = min(4.0, self._height_exaggeration + 0.1)
            return
        if key in ("minus", "underscore"):
            self._height_exaggeration = max(0.5, self._height_exaggeration - 0.1)
            return
        self._keys.add(key)

    def _on_key_up(self, event: tk.Event) -> None:
        key = str(event.keysym)
        self._keys.discard(key)

    def _tick(self) -> None:
        now = time.perf_counter()
        dt = max(0.0, min(0.1, now - self._last_t))
        self._last_t = now

        self._update_camera(dt)
        self._render()

        delay_ms = int(round(1000 / self._fps))
        self._root.after(delay_ms, self._tick)

    def _update_camera(self, dt: float) -> None:
        move = self._move_speed * dt
        turn = self._turn_speed * dt

        if "Left" in self._keys:
            self._yaw_deg -= turn
        if "Right" in self._keys:
            self._yaw_deg += turn

        if "Up" in self._keys:
            self._pitch_deg -= turn * 0.6
        if "Down" in self._keys:
            self._pitch_deg += turn * 0.6

        if "q" in self._keys or "Q" in self._keys:
            self._pitch_deg += turn
        if "e" in self._keys or "E" in self._keys:
            self._pitch_deg -= turn

        self._pitch_deg = max(-25.0, min(25.0, self._pitch_deg))

        yaw_rad = math.radians(self._yaw_deg)
        fwd_x = math.sin(yaw_rad)
        fwd_z = math.cos(yaw_rad)
        right_x = fwd_z
        right_z = -fwd_x

        if "w" in self._keys or "W" in self._keys:
            self._cam_x += fwd_x * move
            self._cam_z += fwd_z * move
        if "s" in self._keys or "S" in self._keys:
            self._cam_x -= fwd_x * move
            self._cam_z -= fwd_z * move
        if "a" in self._keys or "A" in self._keys:
            self._cam_x -= right_x * move
            self._cam_z -= right_z * move
        if "d" in self._keys or "D" in self._keys:
            self._cam_x += right_x * move
            self._cam_z += right_z * move

        self._cam_x = max(0.0, min((self._world.world_width - 1), self._cam_x))
        self._cam_z = max(0.0, min((self._world.world_depth - 1), self._cam_z))
        self._update_camera_y()

    def _update_camera_y(self) -> None:
        x = int(round(self._cam_x))
        z = int(round(self._cam_z))
        top = self._world.top_solid_y(x, z)
        base = max(0, top)
        eye = self._eye_height if self._eye_height is not None else 2.0
        self._cam_y = base + eye

    def _break_block(self) -> None:
        hit = self._last_hit
        if hit is None:
            return
        x, y, z, _n = hit
        self._world.set_block(x, y, z, AIR)

    def _place_block(self) -> None:
        hit = self._last_hit
        if hit is None:
            return
        x, y, z, n = hit
        tx = x + n[0]
        ty = y + n[1]
        tz = z + n[2]
        if self._world.get_block(tx, ty, tz) == AIR:
            self._world.set_block(tx, ty, tz, DIRT)

    def _render(self) -> None:
        pixels = _render_voxel_frame(
            self._world,
            screen_width=self._render_width,
            screen_height=self._render_height,
            camera=(self._cam_x, self._cam_y, self._cam_z),
            yaw_deg=self._yaw_deg,
            pitch_deg=self._pitch_deg,
            fov_deg=self._fov_deg,
            max_distance=32.0 if self._max_distance is None else float(self._max_distance),
            fog_strength=self._fog_strength,
            stride=self._voxel_stride,
        )
        self._last_hit = _raycast_center(self._world, (self._cam_x, self._cam_y, self._cam_z), self._yaw_deg, self._pitch_deg)
        _blit_to_photoimage(self._photo_raw, pixels)
        img = self._photo_raw
        if self._render_scale != 1:
            img = self._photo_raw.zoom(self._render_scale, self._render_scale)
        self._photo = img
        self._canvas.itemconfig(self._img_item, image=self._photo)
        self._canvas.itemconfig(
            self._hud_item,
            text=f"WASD 移动  方向键转头  Q/E 上下看  F 破坏  G 放置  B 方块  +/- 起伏  1-4 清晰度  ESC 退出   x={self._cam_x:.1f} z={self._cam_z:.1f} yaw={self._yaw_deg:.1f} pitch={self._pitch_deg:.1f} ex={self._height_exaggeration:.1f} block={self._block_size} stride={self._voxel_stride}",
        )


def _blit_to_photoimage(img: tk.PhotoImage, pixels: list[list[tuple[int, int, int]]]) -> None:
    height = len(pixels)
    for y in range(height):
        row = pixels[y]
        row_parts: list[str] = ["{"]
        for x in range(len(row)):
            r, g, b = row[x]
            row_parts.append(f"#{r:02x}{g:02x}{b:02x}")
        row_parts.append("}")
        img.put(" ".join(row_parts), to=(0, y))


def _render_voxel_frame(
    world: VoxelWorld,
    screen_width: int,
    screen_height: int,
    camera: tuple[float, float, float],
    yaw_deg: float,
    pitch_deg: float,
    fov_deg: float,
    max_distance: float,
    fog_strength: float,
    stride: int = 1,
) -> list[list[tuple[int, int, int]]]:
    sky_top = (80, 140, 220)
    sky_bottom = (170, 220, 255)
    sky_horizon = (220, 240, 255)

    pixels: list[list[tuple[int, int, int]]] = [[(0, 0, 0) for _ in range(screen_width)] for _ in range(screen_height)]
    for sy in range(screen_height):
        t = sy / max(1, screen_height - 1)
        if t < 0.55:
            pixels[sy] = [_lerp_rgb(sky_top, sky_horizon, t / 0.55) for _ in range(screen_width)]
        else:
            pixels[sy] = [_lerp_rgb(sky_horizon, sky_bottom, (t - 0.55) / 0.45) for _ in range(screen_width)]

    yaw = math.radians(yaw_deg)
    pitch = math.radians(pitch_deg)

    forward = _normalize3(math.sin(yaw) * math.cos(pitch), -math.sin(pitch), math.cos(yaw) * math.cos(pitch))
    right = _normalize3(math.cos(yaw), 0.0, -math.sin(yaw))
    up = _normalize3(
        forward[1] * right[2] - forward[2] * right[1],
        forward[2] * right[0] - forward[0] * right[2],
        forward[0] * right[1] - forward[1] * right[0],
    )

    tan_half = math.tan(math.radians(fov_deg) * 0.5)
    aspect = screen_width / max(1, screen_height)
    light = _normalize3(0.55, 0.85, 0.25)

    stride = max(1, min(8, int(stride)))

    for sy in range(0, screen_height, stride):
        sample_sy = min(screen_height - 1, sy + (stride * 0.5))
        py = (1.0 - (2.0 * (sample_sy + 0.5) / screen_height)) * (tan_half / max(1e-6, aspect))
        for sx in range(0, screen_width, stride):
            sample_sx = min(screen_width - 1, sx + (stride * 0.5))
            px = ((2.0 * (sample_sx + 0.5) / screen_width) - 1.0) * tan_half
            dir_x = forward[0] + right[0] * px + up[0] * py
            dir_y = forward[1] + right[1] * px + up[1] * py
            dir_z = forward[2] + right[2] * px + up[2] * py
            dir2 = _normalize3(dir_x, dir_y, dir_z)
            hit = world.raycast(camera, dir2, max_distance=max_distance)
            if hit is None:
                continue

            base = _block_color(hit.block_id)
            n = hit.normal
            shade = 0.38 + 0.75 * max(0.0, (n[0] * light[0] + n[1] * light[1] + n[2] * light[2]))
            col = (
                int(round(base[0] * shade)),
                int(round(base[1] * shade)),
                int(round(base[2] * shade)),
            )
            fog = (hit.distance / max(1e-6, max_distance)) ** 1.15
            fog = max(0.0, min(1.0, fog * fog_strength))
            color = _lerp_rgb(col, sky_horizon, fog)
            for yy in range(sy, min(screen_height, sy + stride)):
                row = pixels[yy]
                for xx in range(sx, min(screen_width, sx + stride)):
                    row[xx] = color

    return pixels


def _raycast_center(world: VoxelWorld, camera: tuple[float, float, float], yaw_deg: float, pitch_deg: float) -> tuple[int, int, int, tuple[int, int, int]] | None:
    yaw = math.radians(yaw_deg)
    pitch = math.radians(pitch_deg)
    dir_x = math.sin(yaw) * math.cos(pitch)
    dir_z = math.cos(yaw) * math.cos(pitch)
    dir_y = -math.sin(pitch)
    hit = world.raycast(camera, (dir_x, dir_y, dir_z), max_distance=6.0)
    if hit is None:
        return None
    return (hit.x, hit.y, hit.z, hit.normal)


def _block_color(block_id: int) -> tuple[int, int, int]:
    if block_id == 1:
        return (95, 170, 70)
    if block_id == 2:
        return (130, 110, 70)
    if block_id == 3:
        return (120, 120, 125)
    return (255, 0, 255)


def _lerp_rgb(a: tuple[int, int, int], b: tuple[int, int, int], t: float) -> tuple[int, int, int]:
    t = max(0.0, min(1.0, t))
    return (
        int(round(a[0] + (b[0] - a[0]) * t)),
        int(round(a[1] + (b[1] - a[1]) * t)),
        int(round(a[2] + (b[2] - a[2]) * t)),
    )


def _normalize3(x: float, y: float, z: float) -> tuple[float, float, float]:
    l = math.sqrt(x * x + y * y + z * z)
    if l <= 1e-9:
        return (0.0, 1.0, 0.0)
    return (x / l, y / l, z / l)
