####! /usr/bin/env python
#############################################

#from enum import Enum
#GLNodeCrasheType = Enum('HighCrash', 'LowCrash', 'BothCrash')

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OKGREEN = ''
        self.WARNING = ''
        self.FAIL = ''
        self.ENDC = ''

def myprint(toPrint):
    if (GLDebug == 1):
        print toPrint

GLDebug = 0;

GLNodes = ['F1', "F2", "F3", "F4", "F5", "F6"]
GLInputs = ['x1', "x2", "x3", "x4", "x5", "x6", "x7"];

GLConnectionsDown = {    "F1": ["x1", "x2"], "F2": ["x3"], "F3":["x5", "x6"],
                        "F4": ["x4", "F3", "x7"], "F5": ["F2", "F4"],
                        "F6": ["F1", "F5"]}

GLConnectionsUp = {"x1": "F1", "x2":"F1", "x3":"F2", "x4":"F4", "x5":"F3",
                  "x6":"F3", "x7":"F4", "F1":"F6", "F2":"F5", "F3":"F4",
                  "F4":"F5", "F5":"F6", "F6": "LAST"}

GLLastNode = "F6";

GLNodesTypeList = {"F1":"OR", "F2":"NOT", "F3":"XOR", "F4":"THREE_OR",
                  "F5": "AND_NOT", "F6": "OR_NOT"}
