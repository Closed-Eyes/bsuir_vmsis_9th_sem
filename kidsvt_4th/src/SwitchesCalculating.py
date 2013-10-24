def getTwoStrListFromStr(string):
    string1 = ""
    string2 = ""
    secondStringWrite = 0
    for char in string:
        if (char == "+"):
            secondStringWrite = 1
            continue
        if (secondStringWrite == 1):
            string2 = string2 + str(char)
        else:
            string1 = string1 + str(char)
    return [string1, string2]

def isEqualPreviousPart(previousPart, key):
    counter = 0
    flagOK = 1
    for char in previousPart:
        #print "key[counter] " + key[counter]
        if (key[counter] != char):
            flagOK = 0
            break
        counter = counter + 1
    if (flagOK == 0):
        return "NO"
    else:
        return "YES"

def isEqualSecondPart(secondPart, key):
    counter = 0
    flagOK = 1
    for char in secondPart:
        #print "key[counter] " + key[counter]
        if (key[counter + 8] != char):
            flagOK = 0
            break
        counter = counter + 1
    if (flagOK == 0):
        return "NO"
    else:
        return "YES"


def getMinimalSwitch(dictionary, previousCombo, usedDictionary):
    notSetted = 1
    minCount = 0
    minCountKey = 0

    print "\n"

#   first part of key - previous value must be equal previous
#   second - part must be unique
    for key, value in dictionary.items():
        twoStrList = getTwoStrListFromStr(key)
        if (twoStrList[1] in usedDictionary):

            print "\nexclude " + str(twoStrList[1])
            print "is in usedDictionary " + str(usedDictionary)
#             print "in Dict"
            continue

        if (isEqualPreviousPart(previousCombo, key) == "YES") or (previousCombo == "NULL"):
            print "has chance " + previousCombo + " " + key
            if (notSetted == 1):
                notSetted = 0
                minCount = int(value)
                minCountKey = key
            else:
                if (int(value) < minCount):
                    print "yyy " + previousCombo + " " + key
                    minCount = int(value)
                    minCountKey = key
    print "return " + str(minCountKey) + " " + str(minCount)
    return [minCountKey, minCount]

def getAngryComboSequence(dictionary):
    counter = 4
    usedCombos = []
    lastUsedCombo = 0
    result = []
    while (counter > 1):
        print "counter" + str(counter)
        if (counter == 4):
            print "first pair"
            comboPair = getMinimalSwitch(dictionary, "NULL", usedCombos)
            result.append(comboPair)

            print "combo " + str(comboPair)
            twoCombo = getTwoStrListFromStr(comboPair[0])
            usedCombos.append(twoCombo[0])
            print "usedCombos " + str(usedCombos)
            lastUsedCombo = twoCombo[1]
            counter = counter - 1
        else:
            comboPair = getMinimalSwitch(dictionary, lastUsedCombo, usedCombos)
            result.append(comboPair)

            print "next pair"
            print "combo " + str(comboPair)
            twoCombo = getTwoStrListFromStr(comboPair[0])
            usedCombos.append(twoCombo[0])
            print "usedCombos " + str(usedCombos)
            lastUsedCombo = twoCombo[1]
            counter = counter - 1
    print result
    return result