import os

# https://stackoverflow.com/a/18351977/18121288
# Though not widely known, str.endswith also accepts a tuple. You don't need to loop.
def get_files(path: str, ext: tuple):
    return [os.path.join(path, file) for path, _, files in os.walk(path) for file in files if file.endswith(ext)]

def join(lst, prefix="", separator=" "):
    return separator.join([prefix + i for i in lst])

def sys(cmd):
    print(cmd)
    os.system(cmd)

CONFIG = {
    "SRC_PATH": "src",
    "ICON_PATH": "src/ico.o",
    "INCLUDES": ["src/", "src/shared/"],
    "EXTRAS": "-lws2_32",
    "STD": "c++20",
    "OUTPATH": "bin\\arc.exe",
    "OPTIMIZATION": "-O2",
    "PRECOMPILES": [
        ("src/arcpch.h.gch", "g++ src/arcpch.h"), # compile arc precompiled headers
        ("src/ico.o", "windres src/ico.rc -O coff -o src/ico.o") # create ico.o containing the data for arc icon
    ]
}
COMMON = f"{join(CONFIG["INCLUDES"], "-I")} {CONFIG["EXTRAS"]} {CONFIG["OPTIMIZATION"]} -std={CONFIG["STD"]} -Wall"

[sys(cmd) for path, cmd in CONFIG["PRECOMPILES"] if not os.path.isfile(path)]

obj_files = get_files(CONFIG["SRC_PATH"], (".cpp", ".c"))
sys(f"g++ {CONFIG["ICON_PATH"]} {join(obj_files)} {COMMON} -o {CONFIG["OUTPATH"]}")
os.system(f"call {CONFIG["OUTPATH"]}")
