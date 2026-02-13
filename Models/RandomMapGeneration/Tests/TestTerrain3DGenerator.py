from __future__ import annotations

import unittest

from RandomMapGeneration.Terrain3D import Terrain3dConfig
from RandomMapGeneration.Terrain3D import generate_terrain_3d


class TestTerrain3dGenerator(unittest.TestCase):
    def test_deterministic(self) -> None:
        config = Terrain3dConfig(seed=123, width=33, depth=17, max_height=20, octaves=4)
        a = generate_terrain_3d(config)
        b = generate_terrain_3d(config)
        self.assertEqual(a.heights, b.heights)

    def test_dimensions_and_bounds(self) -> None:
        config = Terrain3dConfig(seed=1, width=9, depth=7, max_height=12, octaves=3)
        terrain = generate_terrain_3d(config)
        self.assertEqual(len(terrain.heights), config.depth)
        self.assertTrue(all(len(row) == config.width for row in terrain.heights))
        for row in terrain.heights:
            for h in row:
                self.assertGreaterEqual(h, 0)
                self.assertLessEqual(h, config.max_height)
