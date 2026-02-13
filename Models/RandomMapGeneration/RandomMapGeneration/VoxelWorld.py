from __future__ import annotations

import dataclasses
import math
from typing import Iterable

from RandomMapGeneration.Terrain3D import Terrain3d


AIR = 0
GRASS = 1
DIRT = 2
STONE = 3


@dataclasses.dataclass(frozen=True)
class ChunkConfig:
    size_x: int = 16
    size_y: int = 96
    size_z: int = 16


def _floor_div(a: int, b: int) -> int:
    return a // b if a >= 0 else -((-a + b - 1) // b)


def _mod_floor(a: int, b: int) -> int:
    return a - _floor_div(a, b) * b


@dataclasses.dataclass
class Chunk:
    cfg: ChunkConfig
    blocks: bytearray

    @classmethod
    def empty(cls, cfg: ChunkConfig) -> "Chunk":
        return cls(cfg=cfg, blocks=bytearray(cfg.size_x * cfg.size_y * cfg.size_z))

    def _index(self, lx: int, ly: int, lz: int) -> int:
        return (ly * self.cfg.size_z + lz) * self.cfg.size_x + lx

    def get_local(self, lx: int, ly: int, lz: int) -> int:
        return self.blocks[self._index(lx, ly, lz)]

    def set_local(self, lx: int, ly: int, lz: int, block_id: int) -> None:
        self.blocks[self._index(lx, ly, lz)] = block_id & 0xFF


@dataclasses.dataclass
class RayHit:
    x: int
    y: int
    z: int
    normal: tuple[int, int, int]
    block_id: int
    distance: float


@dataclasses.dataclass
class VoxelWorld:
    cfg: ChunkConfig
    chunks: dict[tuple[int, int], Chunk]
    world_width: int
    world_depth: int

    @classmethod
    def from_terrain(
        cls,
        terrain: Terrain3d,
        chunk_cfg: ChunkConfig | None = None,
        dirt_depth: int = 4,
        base_level: int = 0,
    ) -> "VoxelWorld":
        cfg = chunk_cfg or ChunkConfig()
        chunks: dict[tuple[int, int], Chunk] = {}
        world = cls(cfg=cfg, chunks=chunks, world_width=terrain.config.width, world_depth=terrain.config.depth)

        max_y = cfg.size_y - 1
        for z in range(terrain.config.depth):
            for x in range(terrain.config.width):
                h = int(terrain.heights[z][x])
                top = max(base_level, min(max_y, h))
                for y in range(base_level, top + 1):
                    if y == top:
                        block = GRASS
                    elif y >= top - dirt_depth:
                        block = DIRT
                    else:
                        block = STONE
                    world.set_block(x, y, z, block)
        return world

    def in_bounds_xz(self, x: int, z: int) -> bool:
        return 0 <= x < self.world_width and 0 <= z < self.world_depth

    def in_bounds_xyz(self, x: int, y: int, z: int) -> bool:
        return self.in_bounds_xz(x, z) and 0 <= y < self.cfg.size_y

    def _chunk_coords(self, x: int, z: int) -> tuple[int, int, int, int]:
        cx = _floor_div(x, self.cfg.size_x)
        cz = _floor_div(z, self.cfg.size_z)
        lx = _mod_floor(x, self.cfg.size_x)
        lz = _mod_floor(z, self.cfg.size_z)
        return cx, cz, lx, lz

    def _get_chunk(self, cx: int, cz: int) -> Chunk:
        key = (cx, cz)
        chunk = self.chunks.get(key)
        if chunk is None:
            chunk = Chunk.empty(self.cfg)
            self.chunks[key] = chunk
        return chunk

    def get_block(self, x: int, y: int, z: int) -> int:
        if not self.in_bounds_xyz(x, y, z):
            return AIR
        cx, cz, lx, lz = self._chunk_coords(x, z)
        chunk = self.chunks.get((cx, cz))
        if chunk is None:
            return AIR
        return chunk.get_local(lx, y, lz)

    def set_block(self, x: int, y: int, z: int, block_id: int) -> None:
        if not self.in_bounds_xyz(x, y, z):
            return
        cx, cz, lx, lz = self._chunk_coords(x, z)
        chunk = self._get_chunk(cx, cz)
        chunk.set_local(lx, y, lz, block_id)

    def top_solid_y(self, x: int, z: int) -> int:
        if not self.in_bounds_xz(x, z):
            return -1
        for y in range(self.cfg.size_y - 1, -1, -1):
            if self.get_block(x, y, z) != AIR:
                return y
        return -1

    def raycast(
        self,
        origin: tuple[float, float, float],
        direction: tuple[float, float, float],
        max_distance: float = 6.0,
        step_limit: int = 2048,
    ) -> RayHit | None:
        ox, oy, oz = origin
        dx, dy, dz = direction

        mag = math.sqrt(dx * dx + dy * dy + dz * dz)
        if mag <= 1e-9:
            return None
        dx /= mag
        dy /= mag
        dz /= mag

        x = int(math.floor(ox))
        y = int(math.floor(oy))
        z = int(math.floor(oz))

        step_x = 1 if dx > 0 else -1 if dx < 0 else 0
        step_y = 1 if dy > 0 else -1 if dy < 0 else 0
        step_z = 1 if dz > 0 else -1 if dz < 0 else 0

        def intbound(s: float, ds: float) -> float:
            if ds == 0.0:
                return math.inf
            if ds > 0.0:
                return (math.floor(s + 1.0) - s) / ds
            return (s - math.floor(s)) / (-ds)

        t_max_x = intbound(ox, dx)
        t_max_y = intbound(oy, dy)
        t_max_z = intbound(oz, dz)

        t_delta_x = math.inf if dx == 0.0 else abs(1.0 / dx)
        t_delta_y = math.inf if dy == 0.0 else abs(1.0 / dy)
        t_delta_z = math.inf if dz == 0.0 else abs(1.0 / dz)

        normal = (0, 0, 0)
        t = 0.0

        for _ in range(step_limit):
            if t > max_distance:
                return None

            if self.in_bounds_xyz(x, y, z):
                bid = self.get_block(x, y, z)
                if bid != AIR:
                    return RayHit(x=x, y=y, z=z, normal=normal, block_id=bid, distance=t)

            if t_max_x < t_max_y:
                if t_max_x < t_max_z:
                    x += step_x
                    t = t_max_x
                    t_max_x += t_delta_x
                    normal = (-step_x, 0, 0)
                else:
                    z += step_z
                    t = t_max_z
                    t_max_z += t_delta_z
                    normal = (0, 0, -step_z)
            else:
                if t_max_y < t_max_z:
                    y += step_y
                    t = t_max_y
                    t_max_y += t_delta_y
                    normal = (0, -step_y, 0)
                else:
                    z += step_z
                    t = t_max_z
                    t_max_z += t_delta_z
                    normal = (0, 0, -step_z)

        return None


def iter_chunk_coords_for_world(cfg: ChunkConfig, width: int, depth: int) -> Iterable[tuple[int, int]]:
    max_cx = _floor_div(width - 1, cfg.size_x)
    max_cz = _floor_div(depth - 1, cfg.size_z)
    for cz in range(0, max_cz + 1):
        for cx in range(0, max_cx + 1):
            yield (cx, cz)
