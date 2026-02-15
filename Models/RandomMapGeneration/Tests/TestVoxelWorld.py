from __future__ import annotations

import unittest

from Source.Terrain3D import Terrain3d
from Source.Terrain3D import Terrain3dConfig
from Source.VoxelWorld import AIR
from Source.VoxelWorld import ChunkConfig
from Source.VoxelWorld import DIRT
from Source.VoxelWorld import GRASS
from Source.VoxelWorld import STONE
from Source.VoxelWorld import VoxelWorld


class TestVoxelWorld(unittest.TestCase):
    def test_set_get_across_chunks(self) -> None:
        cfg = ChunkConfig(size_x=4, size_y=16, size_z=4)
        world = VoxelWorld(cfg=cfg, chunks={}, world_width=32, world_depth=32)
        world.set_block(0, 1, 0, DIRT)
        world.set_block(5, 2, 5, GRASS)
        world.set_block(9, 3, 9, STONE)
        self.assertEqual(world.get_block(0, 1, 0), DIRT)
        self.assertEqual(world.get_block(5, 2, 5), GRASS)
        self.assertEqual(world.get_block(9, 3, 9), STONE)

    def test_from_terrain_fills_columns(self) -> None:
        config = Terrain3dConfig(width=8, depth=8, max_height=10, seed=0, octaves=1)
        heights = tuple(tuple(3 for _ in range(config.width)) for _ in range(config.depth))
        terrain = Terrain3d(config=config, heights=heights)
        world = VoxelWorld.from_terrain(terrain, chunk_cfg=ChunkConfig(size_x=4, size_y=16, size_z=4))
        self.assertEqual(world.top_solid_y(2, 2), 3)
        self.assertEqual(world.get_block(2, 3, 2), GRASS)
        self.assertNotEqual(world.get_block(2, 0, 2), AIR)

    def test_raycast_hits_block_and_normal(self) -> None:
        cfg = ChunkConfig(size_x=8, size_y=16, size_z=8)
        world = VoxelWorld(cfg=cfg, chunks={}, world_width=32, world_depth=32)
        world.set_block(2, 2, 2, STONE)
        hit = world.raycast((2.5, 2.5, 0.5), (0.0, 0.0, 1.0), max_distance=10.0)
        self.assertIsNotNone(hit)
        assert hit is not None
        self.assertEqual((hit.x, hit.y, hit.z), (2, 2, 2))
        self.assertEqual(hit.normal, (0, 0, -1))

    def test_place_break(self) -> None:
        cfg = ChunkConfig(size_x=8, size_y=16, size_z=8)
        world = VoxelWorld(cfg=cfg, chunks={}, world_width=32, world_depth=32)
        world.set_block(1, 1, 1, DIRT)
        self.assertEqual(world.get_block(1, 1, 1), DIRT)
        world.set_block(1, 1, 1, AIR)
        self.assertEqual(world.get_block(1, 1, 1), AIR)
