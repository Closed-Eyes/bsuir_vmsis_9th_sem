#! /usr/bin/env python

from array import *
from Globals import *

def calcLogicFunctions(nodeName, args):
	realLogicFunctions = {"OR" : calcOr,
           "NOT" : calcNot,
           "XOR" : calcXor,
           "THREE_OR" : calcThreeOr,
           "AND_NOT" : calcAndNot,
           "OR_NOT" : calcOrNot
	}
	elementType = GLNodesTypeList[nodeName]
	return realLogicFunctions[elementType](args)
#element_type = names_to_types[node_name]
#return real_logic_functions[element_type](args)

def calcOr(args):
	return (args[0] | args[1])
def calcNot(arg):
	if (arg == 0):
		return 1
	else:
		return 0
def calcThreeOr(args):
        return (args[0] | args[1] | args[2])
def calcXor(args):
        return (args[0] ^ args[1])
def calcAndNot(args):
        result = (args[0] & args[1])
	if (result == 0):
		return 1
	else:
		return 0
		
def calcOrNot(args):
        result = (args[0] | args[1])
	if (result == 0):
                return 1
        else:
                return 0 

############# Generation of test combos

def generateAllTestCombos():
	initialCombo = 255
	for index in range(0, 255):
		stringRepresentation = str(bin(initialCombo))
		additionalZeroCount = int(8 + 2) - len(stringRepresentation)
		if (additionalZeroCount > 0):
			for zeroNumber in range(1, int(additionalZeroCount)):
				stringRepresentation = str(stringRepresentation) + str('0') 
		print stringRepresentation
		initialCombo = initialCombo - 1

############# Get scheme output 

def getSchemeOutput(inputs, brokenName, brokenType):
	GLConnectionsDown
	GLConnectionsUp
	for input in GLInputs 

############# TEST Elements functions


args31 = array('i', [0, 0, 0])
args32 = array('i', [0, 0, 1])
args33 = array('i', [0, 1, 0])
args34 = array('i', [0, 1, 1])
args35 = array('i', [1, 0, 0])
args36 = array('i', [1, 0, 1])
args37 = array('i', [1, 1, 0])
args38 = array('i', [1, 1, 1])


args21 = array('i', [0, 0])
args22 = array('i', [0, 1])
args23 = array('i', [1, 0])
args24 = array('i', [1, 1])

args11 = 1
args12 = 0

print "TEST F1"
print calcLogicFunctions("F1", args21)
print calcLogicFunctions("F1", args22)
print calcLogicFunctions("F1", args23)
print calcLogicFunctions("F1", args24)

print "TEST F2"
print calcLogicFunctions("F2", args11)
print calcLogicFunctions("F2", args12)

print "TEST F3"
print calcLogicFunctions("F3", args21)
print calcLogicFunctions("F3", args22)
print calcLogicFunctions("F3", args23)
print calcLogicFunctions("F3", args24)

print "TEST F4"
print calcLogicFunctions("F4", args31)
print calcLogicFunctions("F4", args32)
print calcLogicFunctions("F4", args33)
print calcLogicFunctions("F4", args34)
print calcLogicFunctions("F4", args35)
print calcLogicFunctions("F4", args36)
print calcLogicFunctions("F4", args37)
print calcLogicFunctions("F4", args38)

print "TEST F5"
print calcLogicFunctions("F5", args21)
print calcLogicFunctions("F5", args22)
print calcLogicFunctions("F5", args23)
print calcLogicFunctions("F5", args24)

print "TEST F6"
print calcLogicFunctions("F6", args21)
print calcLogicFunctions("F6", args22)
print calcLogicFunctions("F6", args23)
print calcLogicFunctions("F6", args24)

generateAllTestCombos()
