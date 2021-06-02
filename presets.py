import json
import locale
import logging
import re
import subprocess
import traceback

from argparse import ArgumentParser
from contextlib import contextmanager
from pathlib import Path
from string import Template

logging.basicConfig(level=logging.INFO, format="%(levelname)s:%(funcName)15s: %(message)s")

"""
NOTE: the double $ as in $${presetName} is an escape,
      it is not substituted by string.Template
"""
TEMPLATE = r"""
{
    "version": 2,
    "cmakeMinimumRequired": {
    "major": 3,
    "minor": 0,
    "patch": 0
    },
    "configurePresets": [
    {
        "name": "user-configure-base",
        "hidden": true,
        "generator": "Ninja",
        "binaryDir": "${sourceDir}/out/build/$${presetName}",
        "cacheVariables": {
        "CMAKE_INSTALL_PREFIX": "${sourceDir}/out/install/$${presetName}",
        "Boost_DIR": "${boostDir}",
        "COSTRM_PYPATH": "${pyPath}",
        "COSTRM_PYVERS": "${pyVersion}"
        }
    },
    {
        "name": "x86-windows-debug",
        "inherits": [ "user-configure-base" ],
        "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
        }
    }
    ],
    "buildPresets": [
    {
        "name": "x86-windows-debug",
        "configurePreset": "x86-windows-debug"
    }
    ]
}
"""

@contextmanager
def notice(msg):
    try:
        yield None
    except:
        logging.error(msg)
        traceback.print_exc()

def findpyver(p: Path):
    e = "python(.*)\\.dll$"
    w = [re.search(e, x.as_posix()).group(1) for x in p.iterdir() if re.search("python(.*)\\.dll$", x.as_posix())]
    r = max(w, key=lambda x: len(x))  # longest match ex take python39.dll over python3.dll
    logging.info(f"checked: {p}")
    logging.info(f"got: {w} {r}")
    return r

def run(boostDir: Path, pyPath: Path):
    cwd: Path = Path.cwd().resolve(strict=True)
    
    sourceDir: Path = cwd / "CMakeLists.txt"

    pyVersion: str = pyPath and findpyver(pyPath) or None

    userPresets: Path = cwd / "CMakeUserPresets.json"

    logging.info(f"{pyVersion=}")
    logging.info(f"{pyPath=}")
    logging.info(f"{boostDir=}")
    logging.info(f"{userPresets=} {userPresets.exists()=}")

    if not userPresets.exists():
        logging.info("creating userPresets")

        s = Template(TEMPLATE)
        a = s.substitute(sourceDir=sourceDir.as_posix(),
                         boostDir=boostDir and boostDir.as_posix() or "",
                         pyPath=pyPath and pyPath.as_posix() or "",
                         pyVersion=pyVersion or "")
        print(a)

        with userPresets.open(mode="wt") as f:
            f.write(a)

    with userPresets.open(mode="rt") as f:
        logging.info("validating userPresets")

        z = f.read()
        j = json.loads(z)
        
        with notice("version and cmakeMinimumRequired entry"):
            assert (j["version"] == 2)
            assert ("cmakeMinimumRequired" in j and
                    "major" in j["cmakeMinimumRequired"] and
                    "minor" in j["cmakeMinimumRequired"] and
                    "patch" in j["cmakeMinimumRequired"])

        with notice("configurePresets and buildPresets entries must exist"):
            assert ("configurePresets" in j and
                    "buildPresets" in j)

        with notice("user-configure-base preset entry must exist"):
            a = list(filter(lambda x: x["name"] == "user-configure-base", j["configurePresets"]))[0]

        with notice("cacheVariables must exist"):
            assert ("cacheVariables" in a and
                    "Boost_DIR" in a["cacheVariables"] and
                    "COSTRM_PYPATH" in a["cacheVariables"] and
                    "COSTRM_PYVERS" in a["cacheVariables"])
        with notice("cacheVariable boost_DIR must contain BoostConfig.cmake"):
            assert (Path(a["cacheVariables"]["Boost_DIR"]) / "BoostConfig.cmake").exists()
        with notice("cacheVariable COSTRM_PYPATH must contain python.exe"):
            assert (Path(a["cacheVariables"]["COSTRM_PYPATH"]) / "python.exe").exists()
        with notice("cacheVariable COSTRM_PYPATH must contain python dll"):
            assert (Path(a["cacheVariables"]["COSTRM_PYPATH"]) / f"python{a['cacheVariables']['COSTRM_PYVERS']}.dll").exists()

        with notice("cmake --list-presets"):
            logging.info(f"{locale.getpreferredencoding()=}")
            c = subprocess.run(["cmake", "--list-presets"], shell=True, check=True, capture_output=True, encoding=locale.getpreferredencoding())
            logging.info(f"cmake --list-presets returned {c.stdout=}")
            assert re.search("x86-windows-debug", c.stdout)

        logging.info("completed")

if __name__ == '__main__':
    import sys
    sys.argv.append("--pyPath")
    sys.argv.append("C:/Users/Andrej/AppData/Local/Programs/Python/Python39")
    sys.argv.append("--boostDir")
    sys.argv.append("C:/Users/Andrej/test/boost/boost_1_76_0/lib64-msvc-14.2/cmake/Boost-1.76.0")
    parser = ArgumentParser()
    parser.add_argument("--boostDir", default=None, type=Path)
    parser.add_argument("--pyPath", default=None, type=Path)
    a = parser.parse_args()

    run(a.boostDir, a.pyPath)

# # GUI ask for directory
# from tkinter import Tk
# from tkinter.filedialog import askdirectory
# r = Tk()
# r.withdraw()
# d = askdirectory(mustexist=True)
