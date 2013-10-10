#! /usr/bin/env python
#from array import *

node_type_list = {"F1":"OR", "F2":"NOT", "F3":"XOR", "F4":"THREE_OR",
                  "F5": "AND_NOT", "F6": "OR_NOT"}

def calc_logic_functions(node_name, args):
	real_logic_functions = {"OR" : calc_or,
           "NOT" : calc_not,
           "XOR" : calc_xor,
           "THREE_OR" : calc_three_or,
           "AND_NOT" : calc_and_not,
           "OR_NOT" : calc_or_not
	}
	element_type = node_type_list[node_name]
	return real_logic_functions[element_type](args)

def calc_or(args):
	return args[0] | args[1]
	#return args.index(0) | args.index(1)  
def calc_not(args):
        return ~args[0]
def calc_three_or(args):
        return args[0] | args[1] | args[2]
def calc_xor(args):
        return args[0] ^ args[1]
def calc_and_not(args):
        return ~(args[0] & args[1])
def calc_or_not(args):
        return ~(args[0] | args[1])

nodes = ['F1', "F2", "F3", "F4", "F5", "F6"]
inputs = ['x1', "x2", "x3", "x4", "x5", "x6", "x7"]; 

connections_down = {	"F1": ["x1", "x2"], "F2": "x3", "F3":["x5", "x6"],
			"F4": ["x4", "F3", "x7"], "F5": ["F2", "F4"],
			"F6": ["F1", "F5"]}

connections_up = {"x1": "F1", "x2":"F1", "x3":"F2", "x4":"F4", "x5":"F3",
		  "x6":"F3", "x7":"F4", "F1":"F6", "F2":"F5", "F3":"F4",
		  "F4":"F5", "F5":"F6", "F6": "LAST"}

last_node = "F6";

args = [0]
print calc_logic_functions("F2", args)
