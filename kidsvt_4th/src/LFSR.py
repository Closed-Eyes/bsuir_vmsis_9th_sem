#! /usr/bin/env python

def getFirstStateToCompute():
    return [0, 0, 0, 0, 0, 1, 0, 0];

def getStringFromList(listStructure):
    string = ""
    for value in listStructure:
        string = string + str(value)
    return string

def computeNextState(currentState):
    print "... " + str(currentState)
    previousState = currentState

    counter = 7
    while (counter >= 0):
        if (counter > 0):
            currentState[counter] = currentState[counter - 1]
        counter = counter - 1
    insertedValue = int(previousState[7]) ^ int(previousState[6])
    insertedValue = int(insertedValue) ^ int(previousState[5]) ^ int(previousState[4]);
    insertedValue = int(insertedValue) ^ int(previousState[2]) ^ int(previousState[1]);
    insertedValue = int(insertedValue) ^ int(previousState[0]);
    currentState[0] = insertedValue
    print "currentState" + str(currentState) + " previous: " + str(previousState)
    return currentState


def computeAllSequences():
    allStates = list()
    firstState = getFirstStateToCompute()

    #print firstState

    currentState = firstState
    canGo = "YES"
    while (canGo == "YES"):
        if (getStringFromList(currentState) in allStates):
            break;
        else:
            allStates.append(getStringFromList(currentState))
            currentState = computeNextState(currentState)
            print "new current state: " + str(currentState)
    return allStates

allStates = computeAllSequences()
for listItem in allStates:
    print listItem