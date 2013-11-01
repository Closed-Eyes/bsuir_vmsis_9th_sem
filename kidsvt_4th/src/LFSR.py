#! /usr/bin/env python

from ComboMaxCover import *
from Globals import *
from LogicElements import *
from SwitchesCalculating import *
from Functions import *

def getStringFromList(listStructure):
    string = ""
    for value in listStructure:
        string = string + str(value)
    return string

def shiftSequences(sequences):
    newSequences = list()
    skipFirst = 1
    for item in sequences:
        if (skipFirst == 1):
            skipFirst = 0
            continue
        newSequences.append(item)
    newSequences.append(sequences[0])
    return newSequences

def computeNextState(currentState):
    print "... " + str(currentState)
    previousState = currentState

    polinom = [1, 1, 1, 1, 0, 1, 1]
    
    #polinom.reverse()

    temp = currentState[6];

    currentState[6] = int(currentState[5])
    currentState[5] = int(currentState[4])
    currentState[4] = int(currentState[3])
    currentState[3] = int(currentState[2])
    currentState[2] = int(currentState[1])
    currentState[1] = int(currentState[0])
    currentState[0] = int(temp)

    for i in range(7):
        currentState[i] = currentState[i] & polinom[i]

    result = currentState[0]

    for i in range(1, 7):
        result = result ^ currentState[i]

    currentState[0] = result;
    return currentState

def convertFirstState(state):
    listState = [0,0,0,0,0,0,0];
    listState[0] = int(state[0])
    listState[1] = int(state[1])
    listState[2] = int(state[2])
    listState[3] = int(state[3])
    listState[4] = int(state[4])
    listState[5] = int(state[5])
    listState[6] = int(state[6])
    return listState

def computeAllSequences(firstState):
    allStates = list()
    #print firstState

    currentState = convertFirstState(firstState)

    canGo = "YES"
    while (canGo == "YES"):
        if (getStringFromList(currentState) in allStates):
            break;
        else:
            allStates.append(getStringFromList(currentState))
            currentState = computeNextState(currentState)
            print "new current state: " + str(currentState)
    return allStates

print "Generating all test combinations"
allTestCombo = generateAllTestCombos()
#print allTestCombo

print "Calculating test cover by each combination"
characteristics = getTestCover(allTestCombo)
testCover = characteristics[0]
testCoverStat = characteristics[1]



skipZeroCombo = 1
allSequences = list()

print "Generating sequences based on the initialState."
allResults = list()
for testCombo in allTestCombo:
    if (skipZeroCombo == 1):
        skipZeroCombo = 0
        continue
    sequences = computeAllSequences(testCombo)

    print "\nInitialState " + str(testCombo) + ""
    #print "initial sequences\n"
    #printListNice(sequences)

    shiftLength = len(sequences)
    print shiftLength;
    while (shiftLength != 0):
        shiftLength = shiftLength - 1
        #print "shifted\n"
        sequences = shiftSequences(sequences)
        result = getFullCoverCombosForSequence(testCover, testCoverStat, sequences)
        exit()
        if (result != 0):
            print "Save results"
            allResults.append(result)
            print result

print "\n\nAll sequences with full cover"
printListNice(allResults)

minResult = allResults[0]
for currentResult in allResults:
    if (minResult[1] > currentResult[1]):
        minResult = currentResult
print "\nBest result:"
print minResult
