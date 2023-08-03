import os

from src.runtime import generate_assets

if __name__ == '__main__':
    root_dir = os.path.dirname(__file__)
    assets_dir = os.path.join(root_dir, "assets")
    generate_assets(assets_dir)
