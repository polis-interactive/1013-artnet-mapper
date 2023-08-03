from dataclasses import dataclass

from src.domain import Box, Point


@dataclass(kw_only=True)
class OutputConfig:
    output_directory: str
    output_prefix: str


class Bitmap:
    pass


class PixelTypeBitmap:
    def __init__(self, config: OutputConfig, dimensions: Box):
        pass

    def set_pixel_value(self, p: Point, v: int):
        # assume 0 <= v <= 255
        pass


class ArtNetBitmap:
    def __init__(self, config: OutputConfig, dimensions: Box):
        pass

    def set_pixel_mapping(self, pixel_index: int, p: Point):
        # assume 0 <= pixel_index < width * height, 0 <= p.x <= width, 0 <= p.y <= height
        pass
