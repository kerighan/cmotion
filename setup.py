import subprocess
from glob import glob

import pybind11
from setuptools import Extension, find_packages, setup


def pkgconfig(pkg, kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    output = subprocess.getoutput('pkg-config --cflags --libs {}'.format(pkg))
    assert "not found" not in output
    for token in output.strip().split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kw


extension_kwargs = pkgconfig(
    'cairo libjpeg freetype2', dict(
        language='c++',
        include_dirs=[
            pybind11.get_include(),
            pybind11.get_include(True)],
        extra_compile_args=["-Ofast"]))


ext_modules = [
    Extension('cmotion', sorted(glob("src/*.cpp")), **extension_kwargs)
]


setup(
    name="cmotion",
    version="0.0.0",
    packages=find_packages(),
    include_package_data=True,
    ext_modules=ext_modules
)
