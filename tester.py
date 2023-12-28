import sys
import json
import os
import shutil
from enum import Enum

class CompilationType(Enum):
    Invalid = 0
    SingleFile = 1
    Makefile = 2

def guess_compilation_type():
    files = []
    makefile_exists = False

    content = os.listdir()
    for x in content:
        if os.path.isdir(x):
            continue

        filename, extension = os.path.splitext(x)
        if extension in ['.c', '.cpp']:
            files.append(x)

        if filename == 'Makefile' and extension == '':
            makefile_exists = True
    
    if makefile_exists:
        return (CompilationType.Makefile, files) 
    if len(files) == 1:
        return (CompilationType.SingleFile, files)
    return (CompilationType.Invalid, files)

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 tester.py TARGET_DIRECTORY")
        return

    path = sys.argv[1]
    
    if not os.path.exists(path):
        print("Error: directory does not exist.")
        return
    
    os.chdir(path)

    if os.path.exists('compile_commands.json'):
        print('Error: "compile_commands.json" already exists.')
        return

    if os.path.exists('.tester_tmp'):
        print('Error: ".tester_tmp" already exists.')
        return

    os.mkdir('.tester_tmp')
    os.system('cp -rf * ./.tester_tmp/')
    os.chdir('.tester_tmp')

    comp_type, files = guess_compilation_type()
    match comp_type:
        case CompilationType.Invalid:
            print("Error: Invalid compilation type.")
            return
        case CompilationType.SingleFile:
            cmds = [ { "directory": os.getcwd(), "arguments": ["gcc", files[0]], "file": files[0] } ]
            with open('../compile_commands.json', 'w', encoding='utf-8') as f:
                json.dump(cmds, f)
        case CompilationType.Makefile:
            os.system("bear -- make > /dev/null")

            cmds = json.load(open('compile_commands.json'))
            files = []
            for x in cmds:
                files.append(x['file'])
            os.system("mv compile_commands.json ../")

    for file in files:
        os.system(f'/bin/sh -c "oclint --rule=TooManyConsecutiveIfStatements "{file}" | grep etu"')

    os.chdir('..')
    os.remove('compile_commands.json')
    os.system("rm -rf .tester_tmp")

if __name__ == '__main__':
    main()