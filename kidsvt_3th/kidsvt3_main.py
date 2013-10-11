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
def calcNot(args):
	if (args == 0):
		return 1
	else:
		return 0
def calcThreeOr(args):
        return (args[0] | args[1] | args[2])
def calcXor(args):
	print (args)
	exit()
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

def getTestInputListFromBinary(string):
	list = []
	string = str(string) 
	for char in string:
		list.append(char)
	return list

def generateAllTestCombos():
	initialCombo = 127
	allTestList = []
	for index in range(0, 127):
		stringRepresentation = str(bin(initialCombo))
		additionalZeroCount = int(8 + 2) - len(stringRepresentation)
		if (additionalZeroCount > 0):
			for zeroNumber in range(1, int(additionalZeroCount)):
				stringRepresentation = str(stringRepresentation) + str('0') 
		currentList = getTestInputListFromBinary(stringRepresentation)
		allTestList.append(currentList)
		initialCombo = initialCombo - 1
	return allTestList

############# Get scheme output 

def isItemInList(item, list):
	for currentItem in list :
		if (currentItem == item):
			return True
	return False 

def getConstantInputsWithNameList(inputs, someInputs):
	valuesToReturn = []
	
	for someInp in someInputs:
		counter = 0
		for key in GLInputs :
			if key == someInp :
				valuesToReturn.append(inputs[counter])
			counter = int(counter) + 1
	return valuesToReturn
		
	

def getSchemeOutput(inputs, brokenName, brokenType):
	currentNodeName = ''
	calculatedValues = dict()
	usedNodes = list()
	print GLConnectionsDown
	for node, nodeInputs in GLConnectionsDown.items():
		collectedInputsOfNode = []
		
		print node
		print nodeInputs
		
		if (node == brokenName):
			calculatedValues[brokenName] = brokenType
			continue
		for currentInput in nodeInputs:
			if (isItemInList(currentInput, GLInputs)):
				usedNodes.append(currentInput)
				collectedInputsOfNode.append(currentInput)
			else:
				if (isItemInList(currentInput, calculatedValues)):
					collectedInputsOfNode.append(currentInput)
		
		if (len(nodeInputs) == len(collectedInputsOfNode)):
			print collectedInputsOfNode
			getConstantInputsWithNameList(inputs, collectedInputsOfNode)
			calculatedValues[node] = calcLogicFunctions(node, )
			
	#print calculatedValues				 

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
a ="""
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
"""
print "============================================="

#print  getConstantInputsWithNameList (GLInputs, [0, 1, 0, 1, 0, 1, 0], ['x1',  'x2',  'x4'])
allTestComboListOfLists = generateAllTestCombos()
getSchemeOutput(allTestComboListOfLists[0], "F1", 1)