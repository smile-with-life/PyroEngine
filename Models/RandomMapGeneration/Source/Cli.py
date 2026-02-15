from __future__ import annotations

import argparse
import logging

from Source.Terrain3D import Terrain3dConfig
from Source.Terrain3D import generate_terrain_3d
from Source.ViewerFps import FpsViewer
from Source.VoxelWorld import VoxelWorld


def main(argv: list[str] | None = None) -> int:
    args = _parse_args(argv)
    _configure_logging()

    config = Terrain3dConfig(
        width=args.width,
        depth=args.depth,
        seed=args.seed,
        max_height=args.max_height,
        octaves=args.octaves,
        persistence=args.persistence,
        lacunarity=args.lacunarity,
    )

    terrain = generate_terrain_3d(config)
    world = VoxelWorld.from_terrain(terrain)
    viewer = FpsViewer(
        terrain,
        voxel_world=world,
        width=args.viewer_width,
        height=args.viewer_height,
        render_scale=args.viewer_scale,
        voxel_stride=args.viewer_voxel_stride,
        yaw_deg=args.viewer_yaw_deg,
        pitch_deg=args.viewer_pitch_deg,
        fov_deg=args.viewer_fov_deg,
        step=args.viewer_step,
        max_distance=args.viewer_max_distance,
        fog_strength=args.viewer_fog_strength,
        eye_height=args.viewer_eye_height,
        height_exaggeration=args.viewer_height_exaggeration,
        block_size=args.viewer_block_size,
        normal_y_scale=args.viewer_normal_y_scale,
        move_speed=args.viewer_move_speed,
        turn_speed=args.viewer_turn_speed,
        fps=args.viewer_fps,
    )
    viewer.run()
    return 0


def _configure_logging() -> None:
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s %(levelname)s %(name)s: %(message)s",
    )


def _parse_args(argv: list[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(prog="RandomMapGeneration")
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--width", type=int, default=129)
    parser.add_argument("--depth", type=int, default=129)
    parser.add_argument("--max-height", type=int, default=64, dest="max_height")
    parser.add_argument("--octaves", type=int, default=5)
    parser.add_argument("--persistence", type=float, default=0.5)
    parser.add_argument("--lacunarity", type=float, default=2.0)
    parser.add_argument("--viewer-width", type=int, default=960, dest="viewer_width")
    parser.add_argument("--viewer-height", type=int, default=540, dest="viewer_height")
    parser.add_argument("--viewer-scale", type=int, default=3, dest="viewer_scale")
    parser.add_argument("--viewer-voxel-stride", type=int, default=2, dest="viewer_voxel_stride")
    parser.add_argument("--viewer-yaw-deg", type=float, default=0.0, dest="viewer_yaw_deg")
    parser.add_argument("--viewer-pitch-deg", type=float, default=6.0, dest="viewer_pitch_deg")
    parser.add_argument("--viewer-fov-deg", type=float, default=80.0, dest="viewer_fov_deg")
    parser.add_argument("--viewer-step", type=float, default=1.0, dest="viewer_step")
    parser.add_argument("--viewer-max-distance", type=float, default=None, dest="viewer_max_distance")
    parser.add_argument("--viewer-fog-strength", type=float, default=0.85, dest="viewer_fog_strength")
    parser.add_argument("--viewer-eye-height", type=float, default=None, dest="viewer_eye_height")
    parser.add_argument("--viewer-height-exaggeration", type=float, default=1.8, dest="viewer_height_exaggeration")
    parser.add_argument("--viewer-block-size", type=int, default=1, dest="viewer_block_size")
    parser.add_argument("--viewer-normal-y-scale", type=float, default=1.2, dest="viewer_normal_y_scale")
    parser.add_argument("--viewer-move-speed", type=float, default=18.0, dest="viewer_move_speed")
    parser.add_argument("--viewer-turn-speed", type=float, default=90.0, dest="viewer_turn_speed")
    parser.add_argument("--viewer-fps", type=int, default=30, dest="viewer_fps")
    return parser.parse_args(argv)
