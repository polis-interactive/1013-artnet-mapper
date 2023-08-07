import os
from enum import Enum

from src.bitmap import OutputConfig
from src.domain import InstallationConfig, Box, Point, Direction2d, Direction1d
from src.generator import Generator


class NiagraPixelTypes(Enum):
    Ghost_Pixels = 1
    N_Pixels = 2
    Drop_Pixels = 3


def generate():
    assets_dir = os.path.dirname(__file__)
    output_dir = os.path.join(assets_dir, "2039-Slate_Niagra_Bottling")
    generator = Generator(
        config=OutputConfig(
            output_directory=output_dir,
            output_prefix="niagra"
        ),
        installation_config=InstallationConfig(
            buffer_count=4,
            rgbw_pixels=False,
            dimensions=Box(width=42, height=48),
            pixel_types=4
        ),
        controllers=["69.4.20.4"]
    )
    # left section
    generator.generate_box(
        Point(x=0, y=0), Box(width=10, height=40), NiagraPixelTypes.N_Pixels.value, Direction2d.SnakeDownRight
    )

    # two lines, all full
    generator.generate_line(
        Point(x=10, y=0), 41, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=11, y=42), 43, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # first complex line
    generator.generate_line(
        Point(x=12, y=0), 33, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=12, y=33), 3, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=12, y=36), 2, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=12, y=38), 3, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=12, y=41), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # second complex line
    generator.generate_line(
        Point(x=13, y=45), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(
        Point(x=13, y=41), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=13, y=40), 8, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=13, y=32), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=13, y=30), 31, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # start of the tip
    generator.generate_line(
        Point(x=14, y=0), 19, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=19), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=21), 8, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=29), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=31), 12, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=43), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # Super scuffed blocked drop
    generator.generate_line(
        Point(x=15, y=46), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=15, y=43), 16, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=15, y=27), 6, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=15, y=21), 2, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=15, y=19), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=15, y=18), 19, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # start of full drop, n slope, full height
    generator.generate_line(
        Point(x=16, y=2), 18, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=16, y=20), 25, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=16, y=45), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # powering through bottom 8, second one
    generator.generate_line(
        Point(x=17, y=47), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=17, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=17, y=44), 24, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=17, y=20), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=17, y=19), 17, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # hits the line on the mock
    generator.generate_line(
        Point(x=18, y=4), 17, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=18, y=21), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=18, y=22), 23, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=18, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=18, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # just past the mock line
    generator.generate_line(
        Point(x=19, y=47), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=19, y=45), 23, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=19, y=22), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=19, y=21), 17, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # last bottom drop, no ghosts
    generator.generate_line(
        Point(x=20, y=6), 17, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=20, y=23), 23, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=20, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # back up, bottom ghost
    generator.generate_line(
        Point(x=21, y=47), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=21, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=21, y=44), 21, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=21, y=23), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # down we go, drop getting shorter
    generator.generate_line(
        Point(x=22, y=9), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=22, y=25), 20, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=22, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=22, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # drops getting off the base!
    generator.generate_line(
        Point(x=23, y=47), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=23, y=44), 19, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=23, y=25), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # finally off the bottom
    generator.generate_line(
        Point(x=24, y=11), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=24, y=27), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=24, y=28), 16, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=24, y=44), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # closing in, final 3
    generator.generate_line(
        Point(x=25, y=46), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=25, y=42), 14, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=25, y=28), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=25, y=27), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # 2 left, then 2 straight lines and a block
    generator.generate_line(
        Point(x=26, y=13), 17, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=26, y=30), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=26, y=31), 10, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=26, y=41), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=26, y=42), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # last one, super scuffed
    generator.generate_line(
        Point(x=27, y=44), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=27, y=40), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=27, y=38), 5, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=27, y=33), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=27, y=31), 17, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # straight lines ahoy, big ones too!
    generator.generate_line(
        Point(x=28, y=0), 43, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    # last line!
    generator.generate_line(
        Point(x=29, y=40), 41, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # square finisher
    generator.generate_box(
        Point(x=30, y=0), Box(width=12, height=40), NiagraPixelTypes.N_Pixels.value, Direction2d.SnakeDownRight
    )

    generator.generate()


if __name__ == '__main__':
    generate()
