from array import *
from Globals import *

def calcLogicFunctions(nodeName, args):
	realLogicFunctions = {"OR" : calcOr,
           "NOT" : calcNot,
           "XOR" : calcXor,
           "THREE_OR" : calcThreeOr,
           "AND_NOT" : calcAndNot,
           "OR_NOT" : calcOrNot
	}
	elementType = GLNodesTypeList[nodeName]
	myprint ("ElementType " + elementType)
	return realLogicFunctions[elementType](args)
#element_type = names_to_types[node_name]
#return real_logic_functions[element_type](args)

def calcOr(args):
	myprint ("calcOr " + str(args))
	return (int(args[0]) | int(args[1]))
def calcNot(args):
	myprint ("calcNot " + str(args))
	if (int(args[0]) == 0):
		return 1
	else:
		return 0
def calcThreeOr(args):
	myprint ("calcThreeOr " + str(args))
	return (int(args[0]) | int(args[1]) | int(args[2]))
def calcXor(args):
	myprint ("calcXor " + str(args))
	return (int(args[0]) ^ int(args[1]))
def calcAndNot(args):
	myprint ("calcAndNot " + str(args))
	result = (int(args[0]) & int(args[1]))
	if (result == 0):
		return 1
	else:
		return 0

def calcOrNot(args):
	myprint ("calcOrNot " + str(args))
        result = (int(args[0]) | int(args[1]))
	if (result == 0):
                return 1
        else:
                return 0