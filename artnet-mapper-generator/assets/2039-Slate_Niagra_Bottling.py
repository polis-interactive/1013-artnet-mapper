import os
from enum import Enum

from src.bitmap import OutputConfig
from src.domain import InstallationConfig, Box, Point, Direction2d, Direction1d, Tracker
from src.generator import Generator


class NiagraPixelTypes(Enum):
    Ghost_Pixels = 1
    N_Pixels = 2
    Drop_Pixels = 3


def do_generate_first_attempt(generator: Generator):
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


"""
CHAIN A, B, C, UNIVERSES 1, 2, 3, 406 PIXELS
"""


def generate_abc(generator: Generator, tracker: Tracker):
    # big meaty chunk before n slopes down
    generator.generate_box(
        Point(x=0, y=0), Box(width=16, height=18), NiagraPixelTypes.N_Pixels.value, Direction2d.SnakeDownRight
    )

    # single lines, decreasing size
    generator.generate_line(
        Point(x=16, y=2), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=17, y=17), 15, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=18, y=4), 14, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=19, y=17), 13, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=20, y=6), 12, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=21, y=17), 10, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=22, y=9), 9, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=23, y=17), 8, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=24, y=11), 7, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=25, y=17), 6, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=26, y=13), 5, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=27, y=17), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    tracker.pixel_count += 406
    tracker.universe_count += 3


"""
CHAIN E, UNIVERSES 4, 5, 220 PIXELS
"""


def generate_e(generator: Generator, tracker: Tracker):
    # just a big box, easy boy
    generator.generate_box(
        Point(x=0, y=18), Box(width=10, height=22), NiagraPixelTypes.N_Pixels.value, Direction2d.SnakeDownRight
    )
    tracker.pixel_count += 220
    tracker.universe_count += 2


"""
CHAIN F, UNIVERSES 6, 7, 8, 9, 542 PIXELS
"""


def generate_f(generator: Generator, tracker: Tracker):

    # line 1, all N
    generator.generate_line(
        Point(x=10, y=18), 23, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 2, all N, up
    generator.generate_line(
        Point(x=11, y=42), 25, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 3, n ghost n, down
    generator.generate_line(
        Point(x=12, y=18), 16, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=12, y=34), 7, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=12, y=41), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 4, n ghost pixel drop ghost n, up
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
        Point(x=13, y=30), 13, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 5, n pixel, ghost, n , ghost, drop, ghost pixel, n
    generator.generate_point(
        Point(x=14, y=18), NiagraPixelTypes.N_Pixels.value
    )
    generator.generate_line(
        Point(x=14, y=19), 3, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=22), 7, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=29), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=14, y=31), 12, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(
        Point(x=14, y=43), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=14, y=44), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 6, n ghost pixel drop ghost drop ghost pixel n pixel
    generator.generate_line(
        Point(x=15, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(
        Point(x=15, y=44), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=15, y=43), 17, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=15, y=26), 4, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=15, y=22), 3, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(
        Point(x=15, y=19), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_point(
        Point(x=15, y=18), NiagraPixelTypes.N_Pixels.value
    )

    # line 7 n ghost pixel drop ghost pixel n, down
    generator.generate_line(
        Point(x=16, y=18), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(
        Point(x=16, y=20), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=16, y=21), 23, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(
        Point(x=16, y=44), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=16, y=45), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 8 n ghost pixel drop n, up
    generator.generate_line(
        Point(x=17, y=47), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=17, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=17, y=44), 24, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=17, y=20), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 9, n ghost pixel, drop, ghost pixel n, down
    generator.generate_line(
        Point(x=18, y=18), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=18, y=21), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=18, y=22), 23, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=18, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=18, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 10, n ghost pixel drop ghost pixel n, up
    generator.generate_line(
        Point(x=19, y=47), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(
        Point(x=19, y=45), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=19, y=44), 22, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=19, y=22), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=19, y=21), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 11 n drop ghost pixel n, down
    generator.generate_line(
        Point(x=20, y=18), 5, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=20, y=23), 22, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(
        Point(x=20, y=45), NiagraPixelTypes.Ghost_Pixels.value
    )
    generator.generate_line(
        Point(x=20, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 12 n ghost pixel drop ghost pixel n, up
    generator.generate_line(
        Point(x=21, y=47), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=21, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=21, y=44), 20, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=21, y=24), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=21, y=23), 6, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 13, n ghost pixel drop ghost pixel n, down
    generator.generate_line(
        Point(x=22, y=18), 7, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=22, y=25), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=22, y=26), 19, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=22, y=45), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=22, y=46), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 14, n ghost pixel drop ghost pixel n
    generator.generate_line(
        Point(x=23, y=47), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=23, y=44), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=23, y=43), 17, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=23, y=26), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=23, y=25), 8, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 15, n ghost pixel, drop, ghost pixel n, down
    generator.generate_line(
        Point(x=24, y=18), 9, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=24, y=27), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=24, y=28), 16, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=24, y=44), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=24, y=45), 2, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 16, n ghost pixel drop ghost n, up
    generator.generate_line(
        Point(x=25, y=46), 3, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_point(Point(x=25, y=43), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=25, y=42), 13, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=25, y=29), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=25, y=27), 10, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # line 17 n ghost drop ghost pixel n
    generator.generate_line(
        Point(x=26, y=18), 12, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=26, y=30), 2, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=26, y=32), 9, NiagraPixelTypes.Drop_Pixels.value, Direction1d.Down
    )
    generator.generate_point(Point(x=26, y=41), NiagraPixelTypes.Ghost_Pixels.value)
    generator.generate_line(
        Point(x=26, y=42), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )

    # line 18 n ghost n
    generator.generate_line(
        Point(x=27, y=44), 4, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=27, y=40), 9, NiagraPixelTypes.Ghost_Pixels.value, Direction1d.Up
    )
    generator.generate_line(
        Point(x=27, y=31), 14, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    # straight last two n lines
    generator.generate_line(
        Point(x=28, y=18), 25, NiagraPixelTypes.N_Pixels.value, Direction1d.Down
    )
    generator.generate_line(
        Point(x=29, y=40), 23, NiagraPixelTypes.N_Pixels.value, Direction1d.Up
    )

    tracker.pixel_count += 562
    tracker.universe_count += 4


"""
CHAIN D, G, UNIVERSES 10, 11, 12, 13, 516 PIXELS
"""


def generate_dg(generator: Generator, tracker: Tracker):
    generator.generate_box(
        Point(x=28, y=17), Box(width=14, height=18), NiagraPixelTypes.N_Pixels.value, Direction2d.SnakeUpRight
    )
    tracker.pixel_count += 252
    tracker.universe_count += 2
    generator.generate_box(
        Point(x=41, y=18), Box(width=12, height=22), NiagraPixelTypes.N_Pixels.value, Direction2d.SnakeDownLeft
    )
    tracker.pixel_count += 264
    tracker.universe_count += 2


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
        controllers=["69.4.22.3"]
    )

    tracker = Tracker(universe_count=0, pixel_count=0)

    print("Starting ABC")
    generate_abc(generator, tracker)
    generator.assert_pixel_count(tracker.pixel_count)
    generator.assert_universe_count(tracker.universe_count)

    generator.start_next_universe()

    print("Starting E")
    generate_e(generator, tracker)
    generator.assert_pixel_count(tracker.pixel_count)
    generator.assert_universe_count(tracker.universe_count)

    generator.start_next_universe()

    print("Starting F")
    generate_f(generator, tracker)
    generator.assert_pixel_count(tracker.pixel_count)
    generator.assert_universe_count(tracker.universe_count)

    generator.start_next_universe()

    print("Starting DG")
    generate_dg(generator, tracker)
    generator.assert_pixel_count(tracker.pixel_count)
    generator.assert_universe_count(tracker.universe_count)

    generator.generate()


if __name__ == '__main__':
    generate()
