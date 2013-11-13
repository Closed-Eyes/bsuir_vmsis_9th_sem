#! /usr/bin/env python

from ComboMaxCover import *
from Globals import *
from LogicElements import *
from SwitchesCalculating import *
from Functions import *


allTestCombo = generateAllTestCombos()
# print allTestCombo

characteristics = getTestCover(allTestCombo)
testCover = characteristics[0]
testCoverStat = characteristics[1]
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