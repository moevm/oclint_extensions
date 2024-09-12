import sys
import os
from shutil import copytree
import subprocess
import glob
from dataclasses import dataclass
import json

EXTENSIONS = [ ".h", ".hpp", ".c", ".cpp" ]

@dataclass
class Message:
    path: str
    repo: str | None
    studwork_name: str | None
    line: int
    column: int
    rule: str
    text: str
    code_snippet: str

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def read_line(filename, line):
    try:
        file = open(filename, encoding="utf-8") 
        line = file.readlines()[line - 1] 
        file.close()
        return line.strip()
    except:
        return "can`t decode utf-8 for some reason, look it up yourself IDK"
    
def json2msg(text):
    try:
        j = json.loads(text)
    except json.JSONDecodeError:
        return []

    res = []
    for v in j["violation"]:
        try:
            code = read_line(v["path"], v["startLine"])
            res.append(Message(v["path"], None, None, v["startLine"], v["startColumn"], v["rule"], v["message"], code))
        except json.JSONDecodeError:
            pass
    return res

MAX_PRIORITY = 500

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
        eprint("-----------")

    return run.returncode, json2msg(run.stdout.decode())

def test_repo(path, args):
    studworks = [a for a in os.listdir(path) if os.path.isdir(path + '/' + a)]

    result = []
    final_retval = 0

    for work in studworks:
        retval, output = test_studwork(path + '/' + work, args)
        
        for i in range(len(output)):
            output[i].studwork_name = work

        result.extend(output)
        if retval != 0:
            final_retval = retval

    return final_retval, result

def test_dataset(path, args):
    repos = glob.glob(path + '/**/pr-????-????', recursive=True)
    
    result = []
    final_retval = 0
    for repo in repos:
        basename = repo[repo.rfind('/')+1:]
        retval, output = test_repo(repo, args)
        
        for i in range(len(output)):
            output[i].repo = basename
        
        result.extend(output)
        if retval != 0:
            final_retval = retval
    
    return final_retval, result

def make_csv(msg_list: list[Message]):
    print('"path","repo","studwork_name","line","column","rule","text","code"')
    for msg in msg_list:
        print(f'"{msg.path}","{msg.repo}","{msg.studwork_name}","{msg.line}","{msg.column}","{msg.rule}","{msg.text}","{msg.code_snippet}"')

def make_pretty(msg_list: list[Message]):
    for msg in msg_list:
        print(f"{msg.path}:{msg.line}:{msg.column} [{msg.rule}] {msg.text}")

PRINT_FUNCTIONS = { "pretty": make_pretty, "csv": make_csv }
TEST_FUNCTIONS = { "studwork": test_studwork, "repo": test_repo, "dataset": test_dataset }
USAGE_STR = "usage: python3 entry.py {studwork/repo/dataset} {pretty/csv} [additional args for oclint...]"

def main():
    copytree("/app/solution/", "/app/tmp/")

    if (len(sys.argv) < 3) or (sys.argv[1] not in TEST_FUNCTIONS) or (sys.argv[2] not in PRINT_FUNCTIONS):
        print(USAGE_STR)
        return 1

    retval, output = TEST_FUNCTIONS[sys.argv[1]]("/app/tmp", sys.argv[3:])
    PRINT_FUNCTIONS[sys.argv[2]](output)
    return retval

if __name__ == "__main__":
    sys.exit(main())
