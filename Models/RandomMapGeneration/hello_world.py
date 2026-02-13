from __future__ import annotations

import logging
from pathlib import Path


def configure_logging() -> None:
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s %(levelname)s %(name)s: %(message)s",
    )


def write_output(output_path: Path, content: str) -> None:
    output_path.write_text(content, encoding="utf-8")


def main() -> int:
    configure_logging()
    logger = logging.getLogger(__name__)
    output_path = Path(__file__).resolve().parent / "output_hello.txt"
    content = "Hello World from RandomMapGeneration"
    write_output(output_path, content)
    logger.info("Hello output written to %s", output_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
