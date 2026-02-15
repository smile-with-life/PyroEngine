RandomMapGeneration

3D 体素沙盒（仿 Minecraft 视角）

运行

在当前目录执行：
python main.py --seed 1 --width 256 --depth 256 --max-height 80

如果卡顿，可调大渲染缩放或调大 stride：
python main.py --seed 1 --width 256 --depth 256 --max-height 80 --viewer-scale 10 --viewer-voxel-stride 3

安装（Windows）

双击运行：
install.bat

或手动：
（先确保已安装 Python 3.10+）
python -m venv .venv
.venv\Scripts\python.exe -m pip install -r requirements.txt

运行 main.py 时，各文件的作用

- main.py
  - 程序入口，只负责调用 Source/Cli.py 里的 main()
- Source/Cli.py
  - 命令行参数解析（seed/width/depth/max-height、窗口大小、viewer-scale、viewer-voxel-stride 等）
  - 生成地形高度图（调用 Terrain3D.py）
  - 把高度图转换成体素世界（调用 VoxelWorld.py）
  - 启动第一人称窗口（调用 ViewerFps.py）
- Source/Terrain3D.py
  - Terrain3dConfig：地形生成参数（随机种子 seed 决定“随机地图但可复现”）
  - generate_terrain_3d：基于伪随机噪声生成 (x,z)->height 的高度图
- Source/VoxelWorld.py
  - VoxelWorld：体素数据结构（分块存储、get/set 方块）
  - from_terrain：把高度图挤出为体素列（草/土/石）
  - raycast：射线拾取（用于准星选中、放置/破坏）
- Source/ViewerFps.py
  - FpsViewer：Tk 窗口 + 键盘控制（WASD、转头等）
  - 逐像素发射 raycast 做体素渲染（stride 越大越流畅但越糊）
  - F/G 破坏/放置方块（写回 VoxelWorld）
- Scripts/GenerateTerrain3D.py
  - 备用入口，等价于运行 main.py（同样调用 Cli.main()）
- Tests/TestTerrain3DGenerator.py
  - 测试高度图生成的确定性与边界
- Tests/TestVoxelWorld.py
  - 测试体素读写、from_terrain、raycast、放置/破坏逻辑
