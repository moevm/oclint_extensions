#!/bin/python3

import sys
import os
from shutil import copytree
import subprocess
import glob
from dataclasses import dataclass
import json
import argparse
import atexit

EXTENSIONS = [ ".h", ".hpp", ".c", ".cpp" ]
MAX_PRIORITY = 500

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

@dataclass(order=True)
class Message:
    path: str
    repo: str | None
    studwork_name: str | None
    line: int
    column: int
    rule: str
    text: str
    code_snippet: str

def read_line(filename, line):
    with open(filename, encoding='utf-8', errors='ignore') as f:
        line = f.readlines()[line - 1]
        return line.strip().replace('\t', ' ')

def json2msg(text, at):
    try:
        j = json.loads(text)
    except json.JSONDecodeError as e:
        eprint(f"failed to decode JSON at {at}: '{text}'")
        eprint(f"{e.lineno}:{e.colno} {e.msg}")
        return []

    res = []
    try:
        for v in j["violation"]:
            code = read_line(v["path"], v["startLine"])
            path = os.path.relpath(v["path"], at)
            res.append(Message(path, None, None, v["startLine"], v["startColumn"], v["rule"], v["message"], code))
    except json.JSONDecodeError:
        eprint(f"failed to get violations from JSON '{text}'")
        eprint(f"{e.lineno}:{e.colno} {e.msg}")

    return res

def test_studwork(path, args):
    srcfiles = [x for x in glob.glob(path + '/**/*', recursive=True) if os.path.splitext(x)[1] in EXTENSIONS]

    if len(srcfiles) == 0:
        return 1, [Message(path, None, None, 0, 0, "no files to check", "make sure your files are named correctly", "None")]

    default_args = ["--report-type=json", f"--max-priority-1={MAX_PRIORITY}", f"--max-priority-2={MAX_PRIORITY}", f"--max-priority-3={MAX_PRIORITY}", "--"]
    run = subprocess.run(
        ["oclint"] + srcfiles + args + default_args,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

    if run.stderr.decode() != "":
        eprint(f"failed to test studwork {path}")
        eprint(f"additional args passed: {args}")
        eprint(f"srcfiles detected: {srcfiles}")
        eprint(f"stdout: {run.stdout.decode()}")
        eprint(f"stderr: {run.stderr.decode()}")
        eprint("-----------")

    return run.returncode, json2msg(run.stdout.decode(), path)

def test_repo(path, args):
    eprint(f"repo {path}:")
    studworks = [a for a in os.listdir(path) if os.path.isdir(path + '/' + a)]

    result = []
    final_retval = 0

    for work in studworks:
        retval, output = test_studwork(path + '/' + work, args)

        for msg in output:
            msg.studwork_name = work

        result.extend(output)
        if retval != 0:
            final_retval = retval

    return final_retval, result

def test_dataset(path, args):
    eprint(f"dataset {path}:")
    repos = glob.glob(path + '/**/pr-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9][0-9]', recursive=True)

    result = []
    final_retval = 0
    for repo in repos:
        basename = repo[repo.rfind('/')+1:]
        retval, output = test_repo(repo, args)

        for msg in output:
            msg.repo = basename

        result.extend(output)
        if retval != 0:
            final_retval = retval

    return final_retval, result

def format_csv(msg_list: list[Message]):
    print('path\trepo\tstudwork_name\tline\tcolumn\trule\ttext\tcode')
    for msg in sorted(msg_list):
        print(f'{msg.path}\t{msg.repo}\t{msg.studwork_name}\t{msg.line}\t{msg.column}\t{msg.rule}\t{msg.text}\t{msg.code_snippet}')

def format_pretty(msg_list: list[Message]):
    for msg in msg_list:
        print(f"{msg.path}:{msg.line}:{msg.column} [{msg.rule}] {msg.text}")

FORMAT_FUNCTIONS = {
    "pretty": format_pretty,
    "csv": format_csv
}

TEST_FUNCTIONS = {
    "studwork": test_studwork,
    "repo": test_repo,
    "dataset": test_dataset
}

def split_argv(argv: list):
    if "--" in argv:
        return argv[0:argv.index("--")], argv[argv.index("--")+1:]
    return argv, []

def cleanup():
    os.system(f"rm -rf ./tmp/")

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument("--input", "-i", type=str, default="/app/solution/")
    parser.add_argument("--format", "-f", type=str, choices=FORMAT_FUNCTIONS.keys(), default="pretty")
    parser.add_argument("--test", "-t", type=str, choices=TEST_FUNCTIONS.keys(), required=True)

    argv, oclint_argv = split_argv(sys.argv[1:])
    args = parser.parse_args(argv)

    copytree(args.input, "./tmp/")
    atexit.register(cleanup)
    
    format_func = FORMAT_FUNCTIONS[args.format]
    test_func = TEST_FUNCTIONS[args.test]

    retval, data = test_func("./tmp/", oclint_argv)
    format_func(data)

    return retval

if __name__ == "__main__":
    sys.exit(main())
