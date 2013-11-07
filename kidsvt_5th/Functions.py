from ComboMaxCover import *
from Globals import *
from LogicElements import *
from SwitchesCalculating import *

def printDictionaryNice(dictionary):
	for key, value in dictionary.items():
		print key + " " + str(value)
def printListNice(l):
	for item in l:
		print item

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

# inputs - list of values 0-1, list length 7
# brokenName - name of broken node F1-F6
# brokenType - 0, 1 constant value on the output
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
		string = string + str(char)
	return string