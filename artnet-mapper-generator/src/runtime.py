import importlib.util
import glob
import logging


logger = logging.getLogger(__file__)


def generate_assets(asset_dir: str):

    asset_files = glob.glob(f"{asset_dir}/*.py")
    if not asset_files:
        logger.exception(f"Couldn't find any python files in specified assets dir: {asset_dir}")
        return

    logger.info(f"Running for {len(asset_files)} asset definitions")

    cntr = 1

    for asset_file in asset_files:
        logger.info(f"Starting to generate {asset_file} [{cntr}/{len(asset_files)}]")

        try:
            module_name = asset_file.split('/')[-1].replace('.py', '')
            spec = importlib.util.spec_from_file_location(module_name, asset_file)
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)

            if not hasattr(module, "generate"):
                logger.exception(f"Asset definition {asset_file} has no method 'generate'")
                return

            logger.info(f"Generating {asset_file} [{cntr}/{len(asset_files)}]")
            module.generate()
            logger.info(f"Finished Generating {asset_file} [{cntr}/{len(asset_files)}]")
            cntr += 1

        except Exception as e:
            logger.exception(e)
            logger.exception(f"Failed on asset definition {asset_file}; exiting")
            return

    logger.info(f"Successfully ran all {len(asset_files)} asset definitions")
