import os
import shutil
from typing import List

from src.bitmap import OutputConfig, PixelTypeBitmap, ArtNetBitmap
from src.domain import InstallationConfig, InstallationLayout, Point, Box, Direction2d, Direction1d, Universe


class Generator:

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
        self._current_position = Point(x=0, y=0)
        self._allocated_addresses = 0
        self._create_output_directory(config.output_directory)
        pixel_count = self._installation_config.pixel_types
        self._pixel_values = [round(i * 255 / (pixel_count - 1)) for i in range(pixel_count)]
        self._pixel_type_bitmap = PixelTypeBitmap(config, installation_config.dimensions)
        self._art_net_bitmap = ArtNetBitmap(config, installation_config.dimensions)

    @staticmethod
    def _create_output_directory(output_directory: str):
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

    def set_current_position(self, p: Point):
        if p.x < 0 or p.y < 0:
            raise RuntimeError(f"{p} can only contain positive values")
        if p.x >= self._installation_config.dimensions.width or \
                p.y >= self._installation_config.dimensions.height:
            raise RuntimeError(f"Position {p} does not fit inside {self._installation_config}")
        self._current_position = p

    def _get_max_pixel_index(self) -> int:
        return 170 if not self._installation_config.rgbw_pixels else 128

    def _get_pixel_width(self) -> int:
        return 3 if not self._installation_config.rgbw_pixels else 4

    def _get_current_universe(self) -> Universe:
        return self._installation_layout.universe_map[self._current_universe]

    def _get_current_controller(self) -> List[int]:
        return self._installation_layout.controllers[self._current_controller]

    # currently, we are only going to accept traversal that is convenient for Niagara
    def generate_box(self, box: Box, pixel_type: int, direction: Direction2d):
        if self._current_position.x + box.width >= self._installation_config.dimensions.width \
                or self._current_position.y + box.height >= self._installation_config.dimensions.height:
            raise RuntimeError(f"{box} does not fit in {self._installation_config} at {self._current_position}")
        if pixel_type < 0 or pixel_type > self._installation_config.pixel_types:
            raise RuntimeError(f"Pixel type {pixel_type} is invalid")

        if direction == Direction2d.SnakeDownRight:
            for x in range(box.width):
                starting_x = self._current_position.x + x
                starting_y = self._current_position.y - (0 if x % 2 == 0 else box.height - 1)
                starting_point = Point(x=starting_x, y=starting_y)
                line_direction = Direction1d.Down if x % 2 == 0 else Direction1d.Up
                self._draw_line(starting_point, pixel_type, line_direction, box.height)

            # set current position up for next run

    def _draw_line(self, p: Point, pixel_type: int, direction: Direction1d, count: int):
        if not self._installation_layout.universe_map:
            self._installation_layout.universe_map[self._current_universe] = Universe(
                id=self._current_universe, pixel_width=self._get_pixel_width(),
                start_index=0, pixel_count=0
            )
        current_universe = self._get_current_universe()
        current_controller = self._get_current_controller()
        if current_universe not in current_controller:
            current_controller.append(current_universe.id)

        increment_position_value = 1 if direction.direction_is_positive() else -1

        for _ in range(count):
            # check if we need to create a new universe
            if current_universe.pixel_count + 1 > self._get_max_pixel_index():
                self._current_universe += 1
                new_universe = Universe(
                    id=self._current_universe, pixel_width=self._get_pixel_width(),
                    start_index=self._allocated_addresses + 1, pixel_count=0
                )
                self._installation_layout.universe_map[self._current_universe] = new_universe
                current_universe = new_universe
                current_controller.append(current_universe.id)

            # Create new pixel
            current_universe.pixel_count += 1
            self._pixel_type_bitmap.set_pixel_value(p, self._pixel_values[pixel_type])
            self._art_net_bitmap.set_pixel_mapping(self._allocated_addresses, p)

            # increment for next iter
            self._allocated_addresses += self._get_pixel_width()
            if direction.direction_is_y():
                p.y += increment_position_value
            else:
                p.x += increment_position_value
