import json
import os
import csv

csvfile = open('report.csv', 'w', newline='')
writer = csv.writer(csvfile, delimiter='!',
                    quotechar='"', quoting=csv.QUOTE_NONNUMERIC)

writer.writerow(["path", "line", "col", "line", "col", "category", "rule",
                 "message", "line"
])

def print_violation(v):
    lines=''
    try:
        lines = ''.join(open(v["path"]).readlines()[(v["startLine"] - 1):v["endLine"]])
    except:
        pass
    
    writer.writerow([ v["path"], v["startLine"], v["startColumn"], v["endLine"],
                      v["endColumn"], v["category"], v["rule"], v["message"],
                      lines
    ])

for x in os.listdir("out/"):
    with open("out/" + x) as f:
        s = f.read()
        if s != '':
            d = json.loads(s)
            for v in d["violation"]:
                print_violation(v)

csvfile.close()
