import hashlib, shutil, json, time, sys, os

def calc_total_time(seconds):
    # separate the integer part (for hours, minutes, and seconds) from the fractional part (for milliseconds)
    sec_int, millis = divmod(seconds, 1)
    millis = int(millis * 1000) # convert the fractional part to milliseconds

    min, sec = divmod(int(sec_int), 60)
    hour, min = divmod(min, 60)
    hours, minutes, seconds = int(hour), int(min), int(sec)

    t = [
        f"{hours} hour" + ("s" if hours > 1 else "") if hours > 0 else None,
        f"{minutes} minute" + ("s" if minutes > 1 else "") if minutes > 0 else None,
        f"{seconds} second" + ("s" if seconds > 1 else "") if seconds > 0 else None,
        f"{millis} ms" if millis > 0 else None
    ]
    t = list(filter(None, t))

    return ", ".join(t) if t else "0 seconds"

# https://stackoverflow.com/a/18351977/18121288
# though not widely known, str.endswith also accepts a tuple. You don't need to loop.
def get_files(path: str, ext: tuple):
    return [os.path.join(path, file) for path, _, files in os.walk(path) for file in files if file.endswith(ext)]

def join(lst, prefix="", separator=" "):
    return separator.join([prefix + i for i in lst])

def system(cmd):
    print(cmd)
    os.system(cmd)

def update_hash(obj):
    with open("obj\\index.json", "w") as f:
        json.dump(obj, f, indent=4)

def read_hash():
    with open("obj\\index.json", "r") as f:
        return json.load(f)

def hash_file(file):
    md5_h = hashlib.md5()

    with open(file, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            md5_h.update(chunk)

    return md5_h.hexdigest()

def create_name(name):
    return f"obj\\{name.replace("/", "_").replace("\\", "_").replace(":", "_")}.o"

def get_untracked_files(cpp_files):
    if not os.path.isdir("obj"):
        os.makedirs("obj")

    index = [
        {
            "name": i,
            "hash": hash_file(i)
        }
        for i in cpp_files
    ]

    files_to_compile, files_to_delete = [], []
    if not os.path.isfile("obj\\index.json"):
        files_to_compile = [i["name"] for i in index]

    else:
        cache = read_hash()
        cached_names = set(i["name"] for i in cache)
        indexed_names = set(i["name"] for i in index)

        new_compile_names = list(indexed_names - cached_names)
        delete_names = list(cached_names - indexed_names)

        if new_compile_names:
            files_to_compile.extend(new_compile_names)

        if delete_names:
            files_to_delete.extend(delete_names)

        cached_hash = set((i["name"], i["hash"]) for i in cache if i["name"] not in new_compile_names and i["name"] not in delete_names)
        indexed_hash = set((i["name"], i["hash"]) for i in index if i["name"] not in new_compile_names and i["name"] not in delete_names)

        for name, hash in indexed_hash - cached_hash:
            files_to_compile.append(name)

    update_hash(index)
    return files_to_compile, [create_name(i) for i in files_to_delete]

CONFIG = {
    "SRC_PATH": "src",
    "ICON_PATH": "src/ico.o",
    "INCLUDES": ["src/", "src/core/", "src/shared/", "src/vendor/"],
    "LINKERS": ["src/vendor/libcurl/lib"],
    "EXTRAS": "-lws2_32 -llibcurl",
    "STD": "c++20",
    "DLLPATH": "src\\vendor\\libcurl\\bin",
    "OUTPATH": "bin\\debug\\",
    "OUTNAME": "arc.exe",
    "OPTIMIZATION": "-O2",
    "PRECOMPILES": [
        ("src/arcpch.h.gch", "g++ src/arcpch.h"), # compile arc precompiled headers
        ("src/ico.o", "windres src/ico.rc -O coff -o src/ico.o") # create ico.o containing the data for arc icon
    ]
}
COMMON = f"{join(CONFIG["INCLUDES"], "-I")} {join(CONFIG["LINKERS"], "-L")} {CONFIG["EXTRAS"]} {CONFIG["OPTIMIZATION"]} -std={CONFIG["STD"]} -Wall"

if not os.path.isdir(CONFIG["OUTPATH"]):
    os.makedirs(CONFIG["OUTPATH"])

if len(sys.argv) <= 1:
    # precompile files
    [system(cmd) for path, cmd in CONFIG["PRECOMPILES"] if not os.path.isfile(path)]

    # get all the untracked files
    cpp_files = get_files(CONFIG["SRC_PATH"], (".cpp", ".c"))
    files_to_compile, files_to_delete = get_untracked_files(cpp_files)

    # remove files that are meant to be removed
    [shutil.rmtree(i) if os.path.isdir(i) else os.remove(i) for i in files_to_delete if os.path.exists(i)]

    start_time = time.time()

    # compile all files that are meant to be compiled into object files
    for filename in files_to_compile:
        objname = create_name(filename)
        log = f"Compiling {filename}"
        print(f"\033[97mCompiling\033[0m \033[90m{filename}\033[0m", " " * (70 - len(log) - 4), f"-> {objname}")

        if filename.endswith(".cpp"):
            os.system(f"g++ -c {filename} {COMMON} -o {objname}")

        elif filename.endswith(".c"):
            os.system(f"gcc -c {filename} {COMMON} -o {objname}")
    print() if files_to_compile else None

    # link object files together and compile the exe
    obj_files = get_files("obj", (".o"))
    system(f"g++ {CONFIG["ICON_PATH"]} {join(obj_files)} {COMMON} -o {CONFIG["OUTPATH"] + CONFIG["OUTNAME"]}")

    # copy all the necessary dlls and files into `bin` folder
    dll_files = get_files(CONFIG["DLLPATH"], (".dll", ".pem"))
    [shutil.copy2(file, CONFIG["OUTPATH"]) for file in dll_files]

    print("total time:", calc_total_time(time.time() - start_time))

input("Press enter to run.")
os.system(f"call {CONFIG["OUTPATH"] + CONFIG["OUTNAME"]}")
