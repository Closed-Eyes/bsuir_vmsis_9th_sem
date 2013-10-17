
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