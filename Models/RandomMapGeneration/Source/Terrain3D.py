from __future__ import annotations

import dataclasses
import math


@dataclasses.dataclass(frozen=True)
class Terrain3dConfig:
    width: int = 129
    depth: int = 129
    seed: int = 0
    max_height: int = 64
    octaves: int = 5
    persistence: float = 0.5
    lacunarity: float = 2.0


@dataclasses.dataclass(frozen=True)
class Terrain3d:
    config: Terrain3dConfig
    heights: tuple[tuple[int, ...], ...]


def generate_terrain_3d(config: Terrain3dConfig) -> Terrain3d:
    _validate_config(config)
    heights: list[list[int]] = []
    for z in range(config.depth):
        row: list[int] = []
        for x in range(config.width):
            v = _fbm(x, z, config)
            h = int(round(v * config.max_height))
            row.append(h)
        heights.append(row)
    return Terrain3d(config=config, heights=tuple(tuple(r) for r in heights))


def _validate_config(config: Terrain3dConfig) -> None:
    if config.width < 2 or config.depth < 2:
        raise ValueError("width/depth must be >= 2")
    if config.max_height < 1:
        raise ValueError("max_height must be >= 1")
    if config.octaves < 1:
        raise ValueError("octaves must be >= 1")
    if not (0.0 < config.persistence <= 1.0):
        raise ValueError("persistence must be in (0, 1]")
    if config.lacunarity <= 1.0:
        raise ValueError("lacunarity must be > 1")


def _fbm(x: int, z: int, config: Terrain3dConfig) -> float:
    if config.width == 1 or config.depth == 1:
        return 0.0

    total = 0.0
    amplitude = 1.0
    frequency = 1.0
    max_amplitude = 0.0

    for octave in range(config.octaves):
        nx = x / (config.width - 1)
        nz = z / (config.depth - 1)
        total += amplitude * _value_noise(nx * frequency, nz * frequency, config.seed, octave)
        max_amplitude += amplitude
        amplitude *= config.persistence
        frequency *= config.lacunarity

    v = total / max_amplitude if max_amplitude > 0 else 0.0
    return max(0.0, min(1.0, v))


def _value_noise(x: float, z: float, seed: int, octave: int) -> float:
    x0 = math.floor(x)
    z0 = math.floor(z)
    x1 = x0 + 1
    z1 = z0 + 1

    tx = x - x0
    tz = z - z0
    sx = _smoothstep(tx)
    sz = _smoothstep(tz)

    v00 = _rand01(x0, z0, seed, octave)
    v10 = _rand01(x1, z0, seed, octave)
    v01 = _rand01(x0, z1, seed, octave)
    v11 = _rand01(x1, z1, seed, octave)

    a = _lerp(v00, v10, sx)
    b = _lerp(v01, v11, sx)
    return _lerp(a, b, sz)


def _smoothstep(t: float) -> float:
    return t * t * (3.0 - 2.0 * t)


def _lerp(a: float, b: float, t: float) -> float:
    return a + (b - a) * t


def _rand01(ix: int, iz: int, seed: int, octave: int) -> float:
    u = _mix_u32(ix, iz, seed, octave)
    return u / 2**32


def _mix_u32(ix: int, iz: int, seed: int, octave: int) -> int:
    n = (ix * 0x1F123BB5) ^ (iz * 0x05491333) ^ (seed * 0x9E3779B9) ^ (octave * 0x85EBCA6B)
    n &= 0xFFFFFFFF
    n ^= (n >> 16) & 0xFFFFFFFF
    n = (n * 0x7FEB352D) & 0xFFFFFFFF
    n ^= (n >> 15) & 0xFFFFFFFF
    n = (n * 0x846CA68B) & 0xFFFFFFFF
    n ^= (n >> 16) & 0xFFFFFFFF
    return n
