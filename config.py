import os, sys, yaml

def make_stub(base_dir, partition, process):
    src_dir = os.path.join(base_dir, "Partitions", partition["name"], process["name"], "Src")
    inc_dir = os.path.join(base_dir, "Partitions", partition["name"], process["name"], "Inc")
    os.makedirs(src_dir, exist_ok=True)
    os.makedirs(inc_dir, exist_ok=True)

    entry = process["entry"]

    # .c file
    c_path = os.path.join(src_dir, f"{entry}.c")
    if not os.path.exists(c_path):
        with open(c_path, "w") as f:
            f.write(f'#include "{entry}.h"\n')
            f.write("#include <stdio.h>\n\n")
            f.write(f"void {entry}(void) {{\n")
            f.write(f'    printf("{entry} running in {partition["name"]}/{process["name"]}\\n");\n')
            f.write("}\n")

    # .h file
    h_path = os.path.join(inc_dir, f"{entry}.h")
    if not os.path.exists(h_path):
        guard = f"{entry.upper()}_H"
        with open(h_path, "w") as f:
            f.write(f"#ifndef {guard}\n#define {guard}\n\n")
            f.write(f"void {entry}(void);\n\n")
            f.write(f"#endif // {guard}\n")

def main():
    if len(sys.argv) != 3:
        print("Usage: python config.py <config.yaml> <base_dir>")
        sys.exit(1)

    config_file, base_dir = sys.argv[1], sys.argv[2]
    with open(config_file) as f:
        config = yaml.safe_load(f)

    for part in config["partitions"]:
        for proc in part["processes"]:
            make_stub(base_dir, part, proc)

if __name__ == "__main__":
    main()
