from setuptools import setup, find_packages

setup(
    name='1014-artnet-mapper',
    version='0.1',
    packages=find_packages(),
    python_requires='>=3.11',
    install_requires=[
        'Pillow>=10.0.0',
        'jsonschema>=4.18.6',
        'numpy>=1.25',
        'numpngw>=0.1.2'
    ],
)