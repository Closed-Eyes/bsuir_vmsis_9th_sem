from ComboMaxCover import *
from Globals import *
from LogicElements import *
from SwitchesCalculating import *
from Functions import *
from random import *

binary = lambda n: n>0 and [n&1]+binary(n>>1) or []

def rotate(l,n):
	return l[-n:] + l[:-n]

def generate_init_states():
	init_states = []
	for i in range(0, 128):
		num_str = bin(i)[2:].zfill(7)

		num_list = [0,0,0,0,0,0,0]
		for j in range(7):
			num_list[j] = int(num_str[j])

		init_states.append(num_list)

	return init_states

def LFSR_next_state(current_state):
	polinom = [1,1,1,1,0,1,1]

	current_state = rotate(current_state, 1)


	result = current_state[0] & polinom[0]
	for i in range(1,7):
		result = result ^ (current_state[i] & polinom[i])

	current_state[0] = result;

	return current_state;


def LFSR_generate_whole_sequence(init_state):

	for i in range(len(init_state)):
		init_state[i] = int(init_state[i])

	sequence = []
	sequence.append(init_state)

	last_element = init_state

	while True:
		last_element = LFSR_next_state(last_element)

		if last_element in sequence:
			break
		else:
			sequence.append(last_element)

	return sequence


def signature_analisator(outputs):
	outputs

init_states = generate_init_states()

characteristics = getTestCover(init_states)
testCover = characteristics[0]
testCoverStat = characteristics[1]

#print init_states

all_results = []

for init_state in init_states:
	sequence = LFSR_generate_whole_sequence(init_state)

	result = getFullCoverCombosForSequence(testCover, testCoverStat, sequence)

	if result != 0:
		#print "Save results"
		all_results.append(result)
		#print result

min_result = all_results[0]
for current_result in all_results:
	if (min_result[1] > current_result[1]):
		min_result = current_result
#print "\nBest result:"
#print min_result

# def bin_list(num,bits):
# 	num_str = bin(i)[2:].zfill(bits)
#
# 	num_list = [0]*bits
#
# 	for j in range(bits):
# 		num_list[j] = int(num_str[j])
# 	return num_list

# SIGNATURE ANALISATOR PART

def zipOutput(scheme_outputs, polinom):
	zipped = [0] * len(polinom)

	for i in range(len(scheme_outputs)):
		zipped = rotate(zipped, 1)


		result = zipped[0] & polinom[0]
		for j in range(1, len(polinom)):
			result = result ^ (zipped[j] & polinom[j])

		zipped[0] = result ^ int(scheme_outputs[i])

# 		print str(zipped), i
	return zipped

# arguments - lists
# return bool True/False
def isSignaturesEqual(correctSignature, testedSignature):
	length = len(correctSignature)
	for index in range(0, length):
		if (correctSignature[index] == testedSignature[index]):
			continue
		else:
			return False
	return True

#signature is list. xors calculated by counting 1 number
def usedXorCount(signature):
	count = 0
	for value in signature:
		if (int(value) == 1):
			count = count + 1
	return count

#L = 40

# return l_sequence
def generateLSequence():
	l_length = 40
	l_sequence = [0] * l_length
	for index in range(0, l_length):
		l_sequence[index] = randrange(0, 2)
	return l_sequence

def calculateFromSide(row_ind, v2_row, width, storage, reverted):
# 	print row_ind, " ", v2_row, " ", width, " ", storage, " ",  reverted
	operand = -1
	for j in range(0, width):
		revert_j = int ((width - 1) - j)
		use = int(j);
		if (reverted == True):
			use = revert_j

# 		print "use " + str(use)
		if (v2_row[use] == 1):
			if (operand == -1):
				operand = storage[use]
			else:
				operand = operand ^ storage[use]
# 			print "operand setVal" + str(operand)

		if ((use == row_ind - 1) and (reverted == True)):
			return operand
		else:
			if ((use == row_ind - 1) and (reverted == False)):
				return operand
	return operand

def getNextSAZipperState(V2, s1_element, s2_element, storage_m):
	storage = list(storage_m)

	channel1_index = 0
	channel2_index = 1

	width_v2 = len(V2)

	next_storage_state = list(storage)
	for i in range(0, width_v2):
		v2_row = V2[i]
# 		print "v2_row " + str(v2_row)
# 		print i

		operand1 = calculateFromSide(i, v2_row, width_v2, storage, True)
		operand2 = calculateFromSide(i, v2_row, width_v2, storage, False)
# 		print "calc_result operand1" + str(int(operand1))
# 		print "calc_result operand2" + str(int(operand2))
		result = -1
		if (operand1 == -1):
			result = int(operand2)
		else:
			if (operand2 == -1):
				result = int(operand1)
			else:
				result = int(operand1) ^ int(operand2)
# 		print "result " + str(result)
		if (i == channel1_index):
			result = result ^ s1_element
		if (i == channel2_index):
			result = result ^ s2_element
# 		print "final result " + str(result)
		next_storage_state[i] = result
	return next_storage_state

# V2 - arraym containing info about mod2 connections
# s1, s2 - 2 sequences to zip
def genSA2Channels(V2, s):
	length_of_sa_zipper = 8
	storage = [0] * length_of_sa_zipper

	next_storage_state = list(storage)
	counter = 0
	for index in range(0, len(s) / 2):
		if (counter <= (len(s) - 1) - 4):
			s1_element = s[counter]
			s2_element = s[counter + 2]
			next_storage_state = getNextSAZipperState(V2, s1_element, s2_element, next_storage_state)
			counter = counter + 4
	return next_storage_state

#moving one in zeros list
def generateNextLevelMask(l_length, mask):
	masks_next_level = list()
	for index in range(0, l_length):
		if (mask[index] == 1):
			continue
		else:
			temp_mask = list(mask)
			temp_mask[index] = 1
			masks_next_level.append(temp_mask)
	return masks_next_level

def putMistakesWithMask(sequence, mask):
	broken_sequence = list(sequence)
	for index in range(0, len(sequence)):
		if (mask[index] == 1):
			broken_sequence[index] = 1 - sequence[index]
	return broken_sequence

def mask(breaks_num, l_length):
	### because of great time need to perform 4th level
	### we set limit in 5000 results for checkings

	LIMIT = 5000

	mask = [0] * l_length

	single_breaks = generateNextLevelMask(l_length, mask)

	if (breaks_num == 1):
		return single_breaks

	level = 1
	current_masks = single_breaks
	next_level_mask = list()

	while (breaks_num > level):
		new_mask_count = 0
		for c_mask in current_masks:
			new_masks = generateNextLevelMask(l_length, c_mask)
			next_level_mask = next_level_mask + new_masks
			new_mask_count = new_mask_count + len(new_masks)
			if (new_mask_count > LIMIT):
				break;

		current_masks = list(next_level_mask)
		print "level: " + str(level + 1) + " " + str(len(current_masks))
		if (breaks_num - 1 == level):
			break
		else:
			next_level_mask = list()
			level = level + 1
	return next_level_mask

def check2chSAOnErrorLevel(error_level, v2, masks, l_sequence):
	mask_mistakes = 0

	for m_item in masks:
		broken_sequence = putMistakesWithMask(l_sequence, m_item)

		right_sa = genSA2Channels(v2, l_sequence)
		wrong_sa = genSA2Channels(v2, broken_sequence)
	# 	print str(right_sa) + " " + str(wrong_sa)
		if (isSignaturesEqual(right_sa, wrong_sa) == True):
			mask_mistakes = mask_mistakes + 1
	print "Errors in sequence " + str(error_level) + ": " + str(mask_mistakes) + "/" + str(len(masks))

def checkSAOnErrorLevel(error_level, polynom, masks, l_sequence):
	mask_mistakes = 0
	for m_item in masks:
		broken_sequence = putMistakesWithMask(l_sequence, m_item)

		right_sa = zipOutput(l_sequence, polynom)
		wrong_sa = zipOutput(broken_sequence, polynom)
	# 	print str(right_sa) + " " + str(wrong_sa)
		if (isSignaturesEqual(right_sa, wrong_sa) == True):
			mask_mistakes = mask_mistakes + 1
	print "Errors in sequence " + str(error_level) + ": " + str(mask_mistakes) + "/" + str(len(masks))

def getColumn(matrix, index):
	column_result = []
	for i in range(0, len(matrix)):
		row = matrix[i]
		column_result.append(row[index])
	return column_result

def power2(matrix):
	result_matrix = []
	for i in range(0, len(matrix)):
		row_in_matrix = []
		row = matrix[i]
		for j in range(0, len(matrix)):
			column = getColumn(matrix, j)
			item = 0
			for v in range(0, len(matrix)):
				item = item + column[v] * row[v]
			row_in_matrix.append(item)
		result_matrix.append(row_in_matrix)
	return result_matrix

poly =  [0, 1, 1, 0, 1, 0, 0, 1]
row2 =  [1, 0, 0, 0, 0, 0, 0, 0]
row3 =  [0, 1, 0, 0, 0, 0, 0, 0]
row4 =  [0, 0, 1, 0, 0, 0, 0, 0]
row5 =  [0, 0, 0, 1, 0, 0, 0, 0]
row6 =  [0, 0, 0, 0, 1, 0, 0, 0]
row7 =  [0, 0, 0, 0, 0, 1, 0, 0]
row8 =  [0, 0, 0, 0, 0, 0, 1, 0]

matrix = []
matrix.append(poly)
matrix.append(row2)
matrix.append(row3)
matrix.append(row4)
matrix.append(row5)
matrix.append(row6)
matrix.append(row7)
matrix.append(row8)

# generate V^2. V2 related to mod2 location and connections
v2 = power2(matrix)
# for row in v2:
# 	print row

l_sequence = generateLSequence()
l_length = len(l_sequence)

sa_2ch = [0] * 8
for index in range(0, len(l_sequence)):
	item = l_sequence[index]
	sa_2ch = getNextSAZipperState(v2, item, item, sa_2ch)
	print sa_2ch

# polynom = [1, 0, 0, 1, 0, 1, 1, 0]

# right sequence to compare with

masks1 = mask(1, l_length)
masks2 = mask(2, l_length)
masks3 = mask(3, l_length)
masks4 = mask(4, l_length)

print "\n1 channel"
checkSAOnErrorLevel(1, poly, masks1, l_sequence)
checkSAOnErrorLevel(2, poly, masks2, l_sequence)
checkSAOnErrorLevel(3, poly, masks3, l_sequence)
checkSAOnErrorLevel(4, poly, masks4, l_sequence)

print "\n2 channels"
check2chSAOnErrorLevel(1, v2, masks1, l_sequence)
check2chSAOnErrorLevel(2, v2, masks2, l_sequence)
check2chSAOnErrorLevel(3, v2, masks3, l_sequence)
check2chSAOnErrorLevel(4, v2, masks4, l_sequence)

exit()