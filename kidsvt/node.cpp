#include "node.h"

Node::Node()
{
    this->current_value = -1;
    this->is_broken = false;
    this->is_constant = false;
    this->is_forwarded = false;
    this->is_last = false;
    this->is_under_the_broken = false;
    this->name = "Default Node";
}



/*
// -----------------------
AND
a b - c
-------
0 0 - 0
0 1 - 0
1 0 - 0
1 1 - 1

AND Value to support forwarding: 1
// -----------------------
OR

a b - c
-------
0 0 - 0
0 1 - 1
1 0 - 1
1 1 - 1

OR Value to support forwarding: 0
// -----------------------
AND-NOT

a b - c
-------
0 0 - 1
0 1 - 1
1 0 - 1
1 1 - 0

AND-NOT Value to support forwarding: 1
// -----------------------
OR-NOT
a b - c
-------
0 0 - 1
0 1 - 0
1 0 - 0
1 1 - 0

OR-NOT Value to support forwarding: 0
// -----------------------
XOR
a b - c
-------
0 0 - 0
0 1 - 1
1 0 - 1
1 1 - 0

XOR Value to support forwarding: 0
// -----------------------
*/

QList<int>* Node::valuesForGettingDeterminedValue(int valueToGet)
{
    QList<int>* valueList = new QList<int>();
    switch(this->elementType){
        case LogicType_NOT:{
            int value = 0;
            if (valueToGet == 0)
                value = 1;
                valueList->append(value);
                return valueList;
        }
        break;
        case LogicType_OR: {
            if (valueToGet == 0){
                valueList->append(0);
                valueList->append(0);
            }
            else {
                valueList->append(1);
                valueList->append(0);
            }

            return valueList;
        };
        case LogicType_AND: {
            if (valueToGet == 1){
                valueList->append(1);
                valueList->append(1);
            }
            else {
                valueList->append(0);
                valueList->append(0);
            }

            return valueList;
        };
        case LogicType_THREE_OR_NOT:{
            if (valueToGet == 1){
                valueList->append(0);
                valueList->append(0);
                valueList->append(0);
            }
            else {
                valueList->append(0);
                valueList->append(1);
                valueList->append(1);
            }

            return valueList;
        };
        case LogicType_AND_NOT: {
            if (valueToGet == 1){
                valueList->append(0);
                valueList->append(0);
                valueList->append(0);
            }
            else {
                valueList->append(1);
                valueList->append(1);
                valueList->append(1);
            }

            return valueList;
        };
        case LogicType_OR_NOT: {
            if (valueToGet == 1){
                valueList->append(0);
                valueList->append(0);
                valueList->append(0);
            }
            else {
                valueList->append(0);
                valueList->append(1);
                valueList->append(1);
            }

            return valueList;
        };
    }
}

int Node::valueForwardSupport()
{
    //enum LogicType {OR, NOT, XOR, AND, AND_NOT, OR_NOT};
    switch(this->elementType){
        case LogicType_OR: {
            return 0;
            break;
        };
        case LogicType_AND: {
            return 1;
            break;
        };
        case LogicType_THREE_OR_NOT:{
            return 0;
            break;
        };
        case LogicType_AND_NOT: {
            return 1;
            break;
        };
        case LogicType_OR_NOT: {
            return 0;
            break;
        };
    }
};
