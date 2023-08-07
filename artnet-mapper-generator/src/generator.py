import json
import os
import shutil
from dataclasses import asdict
from typing import List

from src.bitmap import PixelTypeBitmap, ArtNetBitmap
from src.domain import InstallationConfig, InstallationLayout, Point, Box, Direction2d, \
    Direction1d, Universe, OutputConfig


def _installation_to_json(layout: InstallationLayout, config: InstallationConfig, output: OutputConfig):
    # Create a dictionary with the layout and config
    data = {
        "installation_layout": asdict(layout),
        "installation_config": asdict(config)
    }

    # Write the dictionary to a JSON file
    with open(f"{output.output_directory}/config.json", "w") as f:
        json.dump(data, f, default=lambda x: x.__dict__)


class Generator:

    # (0, 0) is the top left corner

    def __init__(self, config: OutputConfig, installation_config: InstallationConfig, controllers: List[str]):
        if not controllers:
            raise RuntimeError("Need to pass at least one artnet host controller")
        self._installation_config = installation_config
        self._installation_layout = InstallationLayout()
        for controller in controllers:
            self._installation_layout.controllers[controller] = []
        self._current_controller = controllers[0]
        self._current_universe = 0
        self._current_pixel_index = 0
        self._allocated_addresses = 0
        pixel_count = self._installation_config.pixel_types
        self._pixel_values = [round(i * 255 / (pixel_count - 1)) for i in range(pixel_count)]
        self._output = config
        self._create_output_directory()
        self._pixel_type_bitmap = PixelTypeBitmap(config, installation_config.dimensions)
        self._art_net_bitmap = ArtNetBitmap(config, installation_config.dimensions)
        self._point_set = set()

    def generate(self):
        self._pixel_type_bitmap.write_image()
        self._art_net_bitmap.write_image()
        _installation_to_json(self._installation_layout, self._installation_config, self._output)

    def _create_output_directory(self):
        output_directory = self._output.output_directory
        # clear the directory for a fresh run
        if os.path.exists(output_directory):
            shutil.rmtree(output_directory)
        os.makedirs(output_directory)

    def set_current_controller(self, controller: str):
        if controller not in self._installation_layout.controllers:
            raise RuntimeError(f"Controller {controller} is invalid")
        self._current_controller = controller

    def set_current_universe(self, universe: int, pixel_index: int = 0):
        if universe < 0:
            raise RuntimeError(f"Universe {universe} is invalid")
        elif pixel_index < 0 or pixel_index >= self._get_max_pixel_index():
            raise RuntimeError(f"Pixel index {pixel_index} is invalid")
        self._current_universe = universe
        self._current_pixel_index = 0

    def _get_max_pixel_index(self) -> int:
        return 170 if not self._installation_config.rgbw_pixels else 128

    def _get_pixel_width(self) -> int:
        return 3 if not self._installation_config.rgbw_pixels else 4

    def _get_current_universe(self) -> Universe:
        return self._installation_layout.universes[self._current_universe]

    def _get_current_controller(self) -> List[int]:
        return self._installation_layout.controllers[self._current_controller]

    # currently, we are only going to accept traversal that is convenient for Niagara
    def generate_box(self, starting_point: Point, box: Box, pixel_type: int, direction: Direction2d):
        final_point = Point(x=starting_point.x, y=starting_point.y)
        if direction == Direction2d.SnakeDownRight:
            final_point.x += box.width - 1
            final_point.y += box.height - 1

        if not (0 <= starting_point.x < self._installation_config.dimensions.width) or \
                not (0 <= final_point.x < self._installation_config.dimensions.width) or \
                not (0 <= starting_point.y < self._installation_config.dimensions.height) or \
                not (0 <= final_point.y < self._installation_config.dimensions.height):
            raise RuntimeError(f"{box} does not fit in {self._installation_config} at {starting_point}")
        if pixel_type < 0 or pixel_type > self._installation_config.pixel_types:
            raise RuntimeError(f"Pixel type {pixel_type} is invalid")

        if direction == Direction2d.SnakeDownRight:
            for x in range(box.width):
                starting_x = starting_point.x + x
                starting_y = starting_point.y + (0 if x % 2 == 0 else box.height - 1)
                line_point = Point(x=starting_x, y=starting_y)
                line_direction = Direction1d.Down if x % 2 == 0 else Direction1d.Up
                self._draw_line(line_point, pixel_type, line_direction, box.height)

    def generate_line(self, starting_point: Point, height, pixel_type: int, direction: Direction1d):
        final_point = Point(x=starting_point.x, y=starting_point.y)
        if direction == Direction1d.Up:
            final_point.y -= (height - 1)
        elif direction == Direction1d.Down:
            final_point.y += (height - 1)

        if not (0 <= starting_point.x < self._installation_config.dimensions.width) or \
                not (0 <= final_point.x < self._installation_config.dimensions.width) or \
                not (0 <= starting_point.y < self._installation_config.dimensions.height) or \
                not (0 <= final_point.y < self._installation_config.dimensions.height):
            raise RuntimeError(f"{height} does not fit in {self._installation_config} at {starting_point}")

        if pixel_type < 0 or pixel_type > self._installation_config.pixel_types:
            raise RuntimeError(f"Pixel type {pixel_type} is invalid")

        self._draw_line(starting_point, pixel_type, direction, height)

    def generate_point(self, current_point: Point, pixel_type: int):
        if not (0 <= current_point.x < self._installation_config.dimensions.width) or \
                not (0 <= current_point.y < self._installation_config.dimensions.height):
            raise RuntimeError(f"{current_point} is not in {self._installation_config}")

        if pixel_type < 0 or pixel_type > self._installation_config.pixel_types:
            raise RuntimeError(f"Pixel type {pixel_type} is invalid")

        self._draw_line(current_point, pixel_type, Direction1d.Down, 1)

    def _draw_line(self, current_point: Point, pixel_type: int, direction: Direction1d, count: int):
        if not self._installation_layout.universes:
            self._installation_layout.universes[self._current_universe] = Universe(
                id=self._current_universe, pixel_width=self._get_pixel_width(),
                start_index=0, pixel_count=0
            )
        current_universe = self._get_current_universe()
        current_controller = self._get_current_controller()
        if current_universe.id not in current_controller:
            current_controller.append(current_universe.id)

        increment_position_value = 1 if direction.direction_is_positive() else -1

        for _ in range(count):
            # check if we need to create a new universe
            if current_universe.pixel_count + 1 > self._get_max_pixel_index():
                self._current_universe += 1
                new_universe = Universe(
                    id=self._current_universe, pixel_width=self._get_pixel_width(),
                    start_index=self._allocated_addresses, pixel_count=0
                )
                self._installation_layout.universes[self._current_universe] = new_universe
                current_universe = new_universe
                current_controller.append(current_universe.id)

            # Create new pixel
            current_universe.pixel_count += 1
            self._pixel_type_bitmap.set_pixel_value(current_point, self._pixel_values[pixel_type])
            self._art_net_bitmap.set_pixel_mapping(self._allocated_addresses // self._get_pixel_width(), current_point)

            if current_point in self._point_set:
                raise RuntimeError(f"{current_point} is already allocated")
            self._point_set.add(current_point)

            # increment for next iter
            self._allocated_addresses += self._get_pixel_width()
            if direction.direction_is_y():
                current_point.y += increment_position_value
            else:
                current_point.x += increment_position_value
