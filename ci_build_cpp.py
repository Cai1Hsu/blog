"""
This script is used to build the C++ code for the CI pipeline.
It finds all C++/C file in the `<root>/acm` and builds them.
"""

import os
import subprocess

search_dir = "acm"
failed_files = []

build_log_file = "build_log.txt"
log_file = open(build_log_file, "w")

def report():
    if failed_files.__len__() == 0:
        print("")
        print("All files compiled successfully.")
        return

    print("")
    print("The following files failed to compile:")
    
    counter = 1
    for file in failed_files:
        print(str(counter) + ". " + file)
    
    print("Please download the log in the artifacts and fix the errors.")
    exit(1)

def compile_code(fullname, shortname):
    compiler = "g++"

    if (fullname.endswith(".cpp")):
        compiler = "g++"

    if (fullname.endswith(".c")):
        compiler = "gcc"
        
    cmd = compiler + " -o /dev/null " + fullname
    
    print("Compiling " + shortname + " with '" + cmd + "'...", end="")
    log_file.write("\nCompiling " + shortname + " with '" + cmd + "'...")
    
    # compile
    result = subprocess.run(cmd, shell=True, capture_output=True)

    # check if compilation failed
    if result.returncode != 0:
        print("Failed", end="\n\n")
        log_file.write("Failed\n")
        log_file.write(result.stderr.decode("utf-8"))
        log_file.write("\n\n")
        failed_files.append(fullname)
        failed = True
        return
    
    print("Success", end="\n\n")
    log_file.write("Success\n")

def main():
    log_file.write("Starting build process...\n")

    for root, dirs, files in os.walk(search_dir):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".c"):
                src_file = os.path.join(root, file)
                print("Found file: " + src_file)
                
                compile_code(src_file, file)

    report()


if __name__ == "__main__":
    main()