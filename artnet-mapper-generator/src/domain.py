from __future__ import annotations
from collections import defaultdict
from dataclasses import dataclass, field
from enum import Enum
from typing import Dict, List


@dataclass(kw_only=True)
class Point:
    x: int
    y: int

    def __eq__(self, other: Point):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash((self.x, self.y))


@dataclass(kw_only=True)
class Box:
    width: int
    height: int


class Direction2d(Enum):
    SnakeDownRight = 0


class Direction1d(Enum):
    Down = 0
    Up = 1
    Left = 2
    Right = 3

    def direction_is_y(self) -> bool:
        return True if self == Direction1d.Up or self == Direction1d.Down else False

    def direction_is_positive(self) -> bool:
        return True if self == Direction1d.Down or self == Direction1d.Right else False


@dataclass
class Universe:
    id: int = 0
    pixel_width: int = 3
    start_index: int = 0
    pixel_count: int = 1


@dataclass
class InstallationLayout:
    universes: Dict[int, Universe] = field(default_factory=dict)
    controllers: Dict[str, List[int]] = field(default_factory=dict)


@dataclass
class InstallationConfig:
    buffer_count: int
    rgbw_pixels: bool
    dimensions: Box
    pixel_types: int


@dataclass(kw_only=True)
class OutputConfig:
    output_directory: str
    output_prefix: str
