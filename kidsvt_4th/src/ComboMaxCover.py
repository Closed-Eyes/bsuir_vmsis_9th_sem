from Globals import *
from LogicElements import *
from SwitchesCalculating import *
from Functions import *

def getCoverOfCombo(globalHigh, globalLow, testCover, combination, coverCount):
                currentCoverCount = coverCount
#                 print "combination " + combination
# #                 print "testCover" + str(testCover[combination])
#                 print "globalBreakUpsHigh " + str(globalHigh)
#                 print "globalBreakUpsLow " + str(globalLow)
                for nodeName, breakTypeList in testCover[combination].items():
                    for breakType in breakTypeList:
                        if (str(breakType) == str(1)):
                            if (globalHigh[nodeName] > 0):

                                currentCoverCount = currentCoverCount - 1
                        if (str(breakType) == str(0)):
                            if (globalLow[nodeName] > 0):
                                currentCoverCount = currentCoverCount - 1
#                 print "currentCoverCount " + str(currentCoverCount)
#                 print "\n"
                return currentCoverCount

def getTestCover(allTestCombo):

    for i in range(len(allTestCombo)):
        for j in range(len(allTestCombo[i])):
            allTestCombo[i][j] = str(allTestCombo[i][j])

    #print allTestCombo
    #exit()

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
    return [testCover, testCoverStat]

def getFullCoverCombosForSequence(testCover, testCoverStat, sequences):
    globalBreakUpsHigh = {"F1" : 0, "F2": 0, "F3" : 0, "F4": 0, "F5" : 0, "F6": 0 }
    globalBreakUpsLow = {"F1" : 0, "F2": 0, "F3" : 0, "F4": 0, "F5" : 0, "F6": 0 }
    globalBreakCount = 12

    counter = 0
    sequenceCover = list()

    for combination in sequences:
        #print "combination " + combination
        combination = listToString(combination)
        coverCount = testCoverStat[combination]
        if (globalBreakCount == 0):
            break
        currentCoverCount = getCoverOfCombo(globalBreakUpsHigh, globalBreakUpsLow, testCover, combination, coverCount)

        if (currentCoverCount > 0):
            globalBreakCount = globalBreakCount - currentCoverCount
            for nodeName, breakTypeList in testCover[combination].items():
                for breakType in breakTypeList:
                    if (str(breakType) == '1'):
                        globalBreakUpsHigh[nodeName] = 1
                    if (str(breakType) == '0'):
                        globalBreakUpsLow[nodeName] = 1
        sequenceCover.append(combination)
        counter = counter + 1
    if (globalBreakCount == 0):
        return [sequenceCover, counter]
    return 0

def getFullCoverCombos(testCover, testCoverStat):
    globalBreakUpsHigh = {"F1" : 0, "F2": 0, "F3" : 0, "F4": 0, "F5" : 0, "F6": 0 }
    globalBreakUpsLow = {"F1" : 0, "F2": 0, "F3" : 0, "F4": 0, "F5" : 0, "F6": 0 }
    globalBreakCount = 12

    maxBreakCount = 0
    maxBreakCombo = 0
    counter = 0

    comboListToCover = list()
    while (globalBreakCount > 0):
        for combination, coverCount in testCoverStat.items():
#             print str(combination) + " " + str(coverCount)
            if (counter == 0):
                if (globalBreakCount == 12):
                    maxBreakCount = coverCount
                    maxBreakCombo = combination
                else:
#                     print "@@@ First combo"
                    currentCoverCount = getCoverOfCombo(globalBreakUpsHigh, globalBreakUpsLow, testCover, combination, coverCount)
                    maxBreakCount = currentCoverCount
                    maxBreakCombo = combination
            else:
                #print "12312coverCount " + str(coverCount)
                if (globalBreakCount == 12):
                    if (maxBreakCount < coverCount):
                        maxBreakCount = coverCount
                        maxBreakCombo = combination
                else:
                    currentCoverCount = getCoverOfCombo(globalBreakUpsHigh, globalBreakUpsLow, testCover, combination, coverCount)
                    #print "currentCover " + str(currentCoverCount) + " combo " + combination
                    if (maxBreakCount < currentCoverCount):
#                         print "combination" + combination
                        maxBreakCount = currentCoverCount
                        maxBreakCombo = combination
            counter = counter + 1
        counter = 0
        comboListToCover.append(maxBreakCombo)
        globalBreakCount = globalBreakCount - maxBreakCount
#         print "globalBreakCount " + str(globalBreakCount)
#         print "testCover[maxBreakCombo]" + str(testCover[maxBreakCombo])
        for nodeName, breakTypeList in testCover[maxBreakCombo].items():
            #print nodeName + " " + str(breakTypeList)
            for breakType in breakTypeList:
                if (str(breakType) == '1'):
                    globalBreakUpsHigh[nodeName] = 1
                if (str(breakType) == '0'):
                    globalBreakUpsLow[nodeName] = 1
        maxBreakComboCount = 0
        maxBreakCombo = 0
    return comboListToCover