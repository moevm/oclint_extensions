import os
import argparse
import json

def prepare_directory(args):
    if not os.path.exists(args.path):
        print("Error: directory does not exist.")
        return False

    os.chdir(args.path)
    if os.path.exists('compile_commands.json'):
        print('Error: "compile_commands.json" already exists.')
        return False

    if os.path.exists('.tester_tmp'):
        print('Error: ".tester_tmp" already exists.')
        return False

    os.mkdir('.tester_tmp')
    os.system('cp -rf * ./.tester_tmp/')
    os.chdir('.tester_tmp')
    return True


def cleanup():
    os.chdir('..')
    os.system("rm -rf .tester_tmp")

def find_cfile():
    files = [f for f in os.listdir() if (os.path.isfile(f) and os.path.splitext(f)[-1] == '.c')]
    if len(files) != 1:
        print(f"Error: expected one .c file, but found {len(files)}")
        return False

    return files[0]  


def build_compile_commands_for_make():
    retval = os.system("bear -- make > /dev/null")
    if os.WEXITSTATUS(retval) != 0:
        print("Error: failed to build compile_commands.json")
        return False
    return True

def build_compile_commands_for_gcc():
    file = find_cfile()
    if not file:
        return False

    cmds = [{ 
        "directory": os.getcwd(),
        "arguments": ["gcc", file],
        "file": file
    }]

    try:
        with open('compile_commands.json', 'w', encoding='utf-8') as f:
            json.dump(cmds, f)
    except IOError:
        print("Error: failed to write into compile_commands.json")
        return False

    return True


def build_compile_commands(build_system):
    match build_system:
        case 'make':
            return build_compile_commands_for_make()            
        case 'gcc':
            return build_compile_commands_for_gcc()
        case _:
            print("Error: unknown build system")
            return False


def get_files_from_compile_commands():
    files = []
    try:
        with open('compile_commands.json', 'r', encoding='utf-8') as cmds_file:
            data = json.load(cmds_file)
            for entry in data:
                files.append(entry["file"])
            
    except IOError:
        print("Error: failed to open compile_commands.json")
        return False
                
    return files

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('path', help = 'Path to coursework src directory')
    parser.add_argument('build_system', choices = ['make', 'gcc'], help = 'Build system used in coursework')
    parser.add_argument('--flags', help = 'Params for oclint')
    args = parser.parse_args()

    if (args.flags == None):
        args.flags = ''

    if not prepare_directory(args):
        exit(1)

    if not build_compile_commands(args.build_system):
        cleanup()
        exit(1)

    files = get_files_from_compile_commands()
    if not files:
        cleanup()
        exit(1)

    for file in files:
        os.system(f'oclint {args.flags}  "{file}"')

    cleanup()

if __name__ == '__main__':
    main()
