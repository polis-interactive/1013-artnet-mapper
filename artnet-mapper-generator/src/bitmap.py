import math
import os.path
from dataclasses import dataclass
from PIL import Image
import numpy as np
from numpngw import write_png

from src.domain import Box, Point, OutputConfig


class PixelTypeBitmap:
    def __init__(self, config: OutputConfig, dimensions: Box):
        self._data = np.zeros(
            (dimensions.height, dimensions.width), dtype=np.uint8
        )
        file_name = f"pixels.bmp"
        self._file_out = os.path.join(config.output_directory, file_name)

    def write_image(self):
        img = Image.fromarray(self._data)
        img.save(self._file_out, 'BMP')

    def set_pixel_value(self, p: Point, v: int):
        # assume 0 <= v <= 255
        self._data[p.y, p.x] = v


class ArtNetBitmap:
    def __init__(self, config: OutputConfig, dimensions: Box):
        self._width = dimensions.width
        self._height = dimensions.height
        self._data = np.zeros(
            (dimensions.height, dimensions.width, 3), dtype=np.uint8
        )
        file_name = f"artnet.png"
        self._file_out = os.path.join(config.output_directory, file_name)

    def write_image(self):
        write_png(self._file_out, self._data)

    def set_pixel_mapping(self, pixel_index: int, p: Point):
        # assume 0 <= pixel_index < width * height, 0 <= p.x < width, 0 <= p.y < height
        write_x = pixel_index % self._width
        write_y = pixel_index // self._width
        self._data[write_y, write_x, 0] = min(math.ceil(p.x / (self._width - 1) * 255), 255)
        self._data[write_y, write_x, 1] = min(math.ceil(p.y / (self._height) * 255), 255)
