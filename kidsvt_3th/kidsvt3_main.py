#! /usr/bin/env python

from ComboMaxCover import *
from Globals import *
from LogicElements import *
from SwitchesCalculating import *

def getStringFromBinary(bin_number):
	stringBin = str(bin_number)
	stringRes = ""
	counter = 0
	for char in stringBin:
		if (counter >= 2):
			stringRes = stringRes + char
		counter = counter + 1
	return stringRes

def getTestInputListFromBinary(string):
	comboList = []
	for char in string:
		comboList.append(char)
	return comboList

def generateAllTestCombos():
	allTestList = []
	for index in range(0, 128):
		stringRepresentation = getStringFromBinary(bin(index))
		#print stringRepresentation
		additionalZeroCount = int(7) - len(stringRepresentation)
		#print additionalZeroCount
		if (additionalZeroCount > 0):
			for zeroNumber in range(0, int(additionalZeroCount)):
				stringRepresentation = str('0') + str(stringRepresentation)
		#print stringRepresentation
		currentList = getTestInputListFromBinary(stringRepresentation)
		allTestList.append(currentList)
	return allTestList

############# Get scheme output
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

	myprint (GLConnectionsDown)
	for node, nodeInputs in GLConnectionsDown.items():
		collectedConstants = []
		collectedValues = []
		myprint ("\n\ncurrentNode" + str(node))
		#print nodeInputs
		if (node == brokenName):
			calculatedValues[brokenName] = brokenType
			myprint( "brokenName " + brokenName)
			continue
		for currentInput in nodeInputs:
			#print "currentInput " + currentInput
			if (currentInput in GLInputs):
				#print "   YES inList of GLInputs"
				usedNodes.append(currentInput)
				collectedConstants.append(currentInput)
			else:
				if (currentInput in calculatedValues):
					valueToCollect = calculatedValues[currentInput]
					collectedValues.append(valueToCollect)
		myprint ("collectedValues" + str(collectedConstants))
		if (len(nodeInputs) == len(collectedConstants) + len(collectedValues)):
			# collectedInputsOfNode - has names of inputs
			#print collectedConstants
			#print collectedValues
			args = getConstantInputsWithNameList(inputs, collectedConstants)
			#print "args before merge: " + str(args)
			if (len(collectedValues) > 0):
				args = args + collectedValues
			#print "args after merge " + str(args)
			#print calcLogicFunctions(node, args)
			calculatedValues[node] = calcLogicFunctions(node, args)
			myprint ("calculatedValues[node] " + str(calculatedValues[node]))
			#print calculatedValues
	#print calculatedValues
	return calculatedValues[GLLastNode]

def getSchemeNodesValues(inputs):
	currentNodeName = ''
	calculatedValues = dict()
	usedNodes = list()

	myprint (GLConnectionsDown)
	for node, nodeInputs in GLConnectionsDown.items():
		collectedConstants = []
		collectedValues = []
		myprint ("\n\ncurrentNode" + str(node))
		#print nodeInputs
		for currentInput in nodeInputs:
			#print "currentInput " + currentInput
			if (currentInput in GLInputs):
				#print "   YES inList of GLInputs"
				usedNodes.append(currentInput)
				collectedConstants.append(currentInput)
			else:
				if (currentInput in calculatedValues):
					valueToCollect = calculatedValues[currentInput]
					collectedValues.append(valueToCollect)
		myprint ("collectedValues" + str(collectedConstants))
		if (len(nodeInputs) == len(collectedConstants) + len(collectedValues)):
			# collectedInputsOfNode - has names of inputs
			#print collectedConstants
			#print collectedValues
			args = getConstantInputsWithNameList(inputs, collectedConstants)
			#print "args before merge: " + str(args)
			if (len(collectedValues) > 0):
				args = args + collectedValues
			#print "args after merge " + str(args)
			#print calcLogicFunctions(node, args)
			calculatedValues[node] = calcLogicFunctions(node, args)
			myprint ("calculatedValues[node] " + str(calculatedValues[node]))
			#print calculatedValues
	#print calculatedValues
	return calculatedValues

def switchesDifference(previousInput, currentInput):
	previousValues = getSchemeNodesValues(previousInput)
	currentValues = getSchemeNodesValues(currentInput)
	print "previousValues " + str(previousValues)
	print "currentValues " + str(currentValues)


	differences = 0
	for key, previousValue in previousValues.items():
		currentValue = currentValues[key]
		if (int(previousValue) != int(currentValue)):
			differences = differences + 1
	return differences


def listToString(listToChange):
	string = ""
	for char in listToChange:
		string = string + char
	return string
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
allTestCombo = generateAllTestCombos()
print allTestCombo

#print allTestCombo[0]

testCover = dict()
testCoverStat = dict()
for currentCombo in allTestCombo:
	rightOutput = getSchemeOutput(currentCombo, "R", 0)
	currentComboCover = dict()
	coverCounter = 0
	for currentNode in GLNodes:
		incorrect1 = getSchemeOutput(currentCombo, currentNode, 1)
		incorrect0 = getSchemeOutput(currentCombo, currentNode, 0)
		currentNodeCover = list()
		if (int(incorrect1) != int(rightOutput)):
			currentNodeCover.append(1)
		if (int(incorrect0) != int(rightOutput)):
			currentNodeCover.append(0)
		if (len(currentNodeCover) > 0):
			currentComboCover[currentNode] = currentNodeCover
			coverCounter = coverCounter + 1
		else:
			currentComboCover[currentNode] = "-"
	if (coverCounter > 0):
		key = listToString(currentCombo)
		testCover[key] = currentComboCover
		testCoverStat[key] = coverCounter

# print len(testCover.items())
for item in testCover.items():
 	print item
######################
comboListToCover = getFullCoverCombos(testCover, testCoverStat)
print "\n result full cover tests"
print comboListToCover

for combination in comboListToCover:
	print testCover[combination]


allSwitchesDictionary = dict()

counter = 0
previous = ''
current = ''
for combination1 in comboListToCover:
	for combination2 in comboListToCover:
		if (combination1 == combination2):
			continue
		previous = getTestInputListFromBinary(combination1)
		current = getTestInputListFromBinary(combination2)
		diff = switchesDifference(previous, current)
		key = combination1 + "+" + combination2
		allSwitchesDictionary[key] = diff

print "Resul combination table pair - switches count"
for key, value in allSwitchesDictionary.items():
 	print "key " + key + " value " + str(value)

counter = 0
switches = 0
indexVar = 1
result = getAngryComboSequence(allSwitchesDictionary)
lastPair = []
previous = 0

print "\n\nVuala! See result below!\n"

for item in result:
	twoPairList = getTwoStrListFromStr(item[0])
	if (previous != twoPairList[0]):
		print "combination: " + twoPairList[0]
	print "combination: " + twoPairList[1]
	previous = twoPairList[1]
	switches = switches + int(item[1])
print "\n Switch's amount = " + str(switches)