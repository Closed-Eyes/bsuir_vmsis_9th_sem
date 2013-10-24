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
    #print "... " + str(currentState)
    previousState = currentState

    counter = 6
    while (counter >= 0):
        if (counter > 0):
            currentState[counter] = currentState[counter - 1]
        counter = counter - 1
    insertedValue = int(previousState[6]) ^ int(previousState[5])
    insertedValue = int(insertedValue) ^ int(previousState[4]) ^ int(previousState[3]);
    insertedValue = int(insertedValue) ^ int(previousState[1]) ^ int(previousState[0]);
    currentState[0] = int(insertedValue)
    #print "currentState" + str(currentState) + " previous: " + str(previousState)
    return currentState

def convertFirstState(state):
    listState = list();
    for char in state:
        t = int(char)
        listState.append(t)
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
            #print "new current state: " + str(currentState)
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
    while (shiftLength != 0):
        shiftLength = shiftLength - 1
        #print "shifted\n"
        sequences = shiftSequences(sequences)
        result = getFullCoverCombosForSequence(testCover, testCoverStat, sequences)
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
