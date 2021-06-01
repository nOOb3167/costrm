from argparse import ArgumentParser
from pathlib import Path
import re
from string import Template

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

def findpyver(p: Path):
    e = "python(.*)\\.dll$"
    w = [re.search(e, x.as_posix()).group(1) for x in p.iterdir() if re.search("python(.*)\\.dll$", x.as_posix())]
    r = max(w, key=lambda x: len(x))  # longest match ex take python39.dll over python3.dll
    return r

def run(boostDir: Path, pyPath: Path):
    assert not boostDir or (boostDir / "BoostConfig.cmake").exists()
    assert not pyPath or (pyPath / "python.exe").exists()

    cwd: Path = Path.cwd().resolve(strict=True)
    
    sourceDir: Path = cwd / "CMakeLists.txt"
    assert sourceDir.exists()

    pyVersion: str = findpyver(pyPath) if pyPath else None

    userPresets: Path = cwd / "CMakeUserPresets.json"

    if True: # and not p.exists():
        s = Template(TEMPLATE)
        a = s.substitute(sourceDir=sourceDir.as_posix(),
                         boostDir=boostDir and boostDir.as_posix() or "",
                         pyPath=pyPath and pyPath.as_posix() or "",
                         pyVersion=pyVersion or "")
        print(a)

        with userPresets.open(mode="wt") as f:
            f.write(a)

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
