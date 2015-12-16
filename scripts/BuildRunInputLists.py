from __future__ import print_function

def get_root_files(filename):
    infile = file(filename)
    rootfiles = []
    for line in infile:
        rootfiles.append(line)
    rootfiles.sort()
    return rootfiles

def extract_filename(filepath):
    tokens = filepath.split("/")
    rootfile = tokens[len(tokens) - 1].rstrip()
    return rootfile

def get_run(filename):
    tokens = filename.split("_")
    fullrun = tokens[3].split("-");
    run = fullrun[0]
    id = fullrun[1]
    return (int(run), int(id))

def build_input_lists(runs, files):
    path = os.environ["TN228HOME"] + "/input_files/"
    for run, ids in runs.iteritems():
        run_files = build_input_list(run, ids, files)
        outputfile = file(path + "input_" + run + "_rdp.list", "w")
        for rootfile in run_files:
            outputfile.write(rootfile)
        outputfile.close();

def extract_run_bounds(run_subrun):
    tokens = run_subrun.split("_")
    if len(tokens) == 2:
        return (int(tokens[0]), int(tokens[1]))
    else:
        return (int(tokens[0]), -1)

import sys
def is_in_range(filerun, filesubrun, start, startsub, finish, finishsub):
    if startsub == -1:
        startsub = -sys.maxint - 1
    if finishsub == -1:
        finishsub = sys.maxint
    if start == finish:
        if filerun == start and filesubrun >= startsub and filesubrun <= finishsub:
            return True
        else:
            return False
    else:
        if filerun == start and filesubrun >= startsub:
            return True
        elif filerun == finish and filesubrun <= finishsub:
            return True
        elif filerun > start and filerun < finish:
            return True
    return False

import os
def build_input_list(run, ids, files):
    run_files = []
    for component in ids:
        start, startsub = extract_run_bounds(component[0])
        finish, finishsub = extract_run_bounds(component[1])
        for filepath in files:
            filename = extract_filename(filepath)
            filerun, filesubrun = get_run(filename)
            if is_in_range(filerun, filesubrun, start, startsub, finish, finishsub):
                run_files.append(filepath)
    return run_files

# Note runs 5a and 5b omit 10282 runs because it is unclear where 5a ends and 5b
# begins
runs = { \
    "1" : [("4165","4222"),("4367","4546"),("4671","4819"),("4979","5115")], \
    "2" : [("6462_11","6965_133"),("7397_47","7711_295"),("7743_10","7754_16")], \
    "3b" : [("8309","8453_2")], \
    "3c" : [("8550_3","8685_114"),("8710_15","8753_19")], \
    "4" : [("8983_16","9088_26"),("9128_16","9217_0"),("9322_19","9475_19"), \
    ("9580_3","9708_4"),("9708_5","9721_72"),("9789_9","9796_4")], \
    "5a" : [("10252_10", "10281")], \
    "5b" : [("10283", "10302_1"),("10518_28","10521_13")], \
    "5c" : [("10334_11","10518_9")], \
    "6a" : [("10932","10949"), ("11306","11328"),("11437","11442")], \
    "6b" : [("10954","11082"),("11126","11239")], \
    "6c" : [("11449","11491")], \
    "6d" : [("11492","11563")], \
    "6e" : [("11619","11647"),("11661","11687")]}

if os.getenv("TN228HOME") != "":
    files = get_root_files(os.environ["TN228HOME"] + "/input_files/local.list")
    run_files = build_input_lists(runs, files)
else:
    print("TN228HOME environment variable is not set. Exiting.")
