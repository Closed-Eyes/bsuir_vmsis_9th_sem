from ComboMaxCover import *
from Globals import *
from LogicElements import *
from SwitchesCalculating import *
from Functions import *

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
		print "Save results"
		all_results.append(result)
		print result

min_result = all_results[0]
for current_result in all_results:
	if (min_result[1] > current_result[1]):
		min_result = current_result
print "\nBest result:"
print min_result