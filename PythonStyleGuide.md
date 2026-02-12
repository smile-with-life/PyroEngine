# PyroEngine Python 代码规范（Google Style）

## 1. 目标与适用范围

本规范定义 PyroEngine 项目中 Python 代码 遵循的风格、命名、结构与工程化约束，旨在提升代码的一致性、可读性、可维护性，并降低跨语言（C++/Python）协作成本。

### 适用范围

- 工具链脚本（构建/打包/资源处理/部署）
- 数据处理与生成（dataset、标注、评测、统计）
- 训练/推理/评估脚本（若引擎相关 AI 模块使用 Python）
- 示例代码与 CLI 工具

## 2. 基本原则

- **可读性第一**：代码写给人读，其次才是给机器运行。

## 3. Import 规范
### 3.1 导入顺序（强制）

按以下顺序分组，并在组间空一行：
1. 标准库
2. 第三方库
3. 项目内模块（PyroEngine / repo 内）

示例：

```python
from __future__ import annotations

import dataclasses
import json
from typing import Any

import numpy as np
import requests

from pyroengine.tools.asset import pack_assets
from pyroengine.utils.log import get_logger
```

### 3.2 禁止行为

- 禁止 `from xxx import *`
- 避免循环 import；必要时改为局部 import 并说明原因。
- 项目内 import 使用 **绝对导入** 为主（从包根开始）。

## 4. 命名规范（Naming）

### 4.1 文件与模块命名

- Python 文件名：`lower_snake_case.py`下划线连接
- 包名同样用 `lower_snake_case/`
- 测试文件：`test_<function>.py`

示例：
```
asset_pipeline.py
model_exporter.py
tests/test_asset_pipeline.py
```
···
### 4.2 类型命名

- **类名**：
单词首字母大写



```python
class AssetDatabase:
    ...

class ConfigError(Exception):
    ...
```

### 4.3 函数与变量命名
函数名应该小写，如果想提高可读性可以用下划线分隔。
永远不要使用字母‘l’（小写的L），‘O’（大写的O），或者‘I’（大写的I）作为单字符变量名
- 函数/方法：def check_func():...
- 局部变量：`lower_snake_case`
- 私有成员：前缀 `_`（单下划线）
- 强私有（避免命名冲突）：`__name`（谨慎使用）

```python
def build_index(asset_root: str) -> dict[str, str]:
    file_count = 0
    ...
```

### 4.4 常量命名
常量一般默认全部为大写, 同时一定要表明好注释
- 常量：`UPPER_SNAKE_CASE`

```python
DEFAULT_TIMEOUT_SEC = 10
MAX_WORKERS = 8
```



### 5 行内注释

- 注释解释 "为什么"，而不是重复代码的"是什么"
- 复杂逻辑允许分块注释，但要保持简短

## 6. 代码结构与工程组织

### 6.1 推荐目录结构

```
pyroengine_py/
  pyroengine/
    __init__.py
    tools/
    pipeline/
    utils/
  scripts/
  tests/
  pyproject.toml
```

### 6.2 单文件职责

- 一个模块聚焦一个主题（asset pipeline / config / io / logging）
- 超过 300~500 行时优先拆分（以可读性为准）

### 6.3 入口（CLI）

- 可执行脚本统一放 `scripts/`
- 入口函数命名 `main()`，并使用：

```python
if __name__ == "__main__":
    raise SystemExit(main())
```


## 7. 日志（Logging）

- 禁止 `print()` 作为核心逻辑输出（脚本一次性调试除外）
- 使用 `logging`，并统一 logger 名称：
  - `logging.getLogger(__name__)`

日志等级规范：
- `debug`：细节
- `info`：关键流程阶段
- `warning`：可恢复问题
- `error`：失败但可继续
- `exception`：捕获异常时自动带堆栈
