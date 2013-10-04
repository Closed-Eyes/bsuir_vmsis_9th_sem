#include "node.h"
#define INSERSECTION_D 9
#define INSERSECTION_NOT_D 100
#define INSERSECTION_X 10
#define INTERSECTION_NULL -100

Node::Node()
{
    this->current_value = -1;
    this->is_broken = false;
    this->is_constant = false;
    this->is_DType = false;
    this->is_last = false;
    this->name = "Default Node";
    this->calc_amount_DCubes = -1;
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

0 x   0
x 0   0
1 1   1

// -----------------------
OR

a b - c
-------
0 0 - 0
0 1 - 1
1 0 - 1
1 1 - 1

0 0   0
1 x   1
x 1   1
// -----------------------
AND-NOT

a b - c
-------
0 0 - 1
0 1 - 1
1 0 - 1
1 1 - 0

x 0   1
0 x   1
1 1   0
// -----------------------
OR-NOT
a b - c
-------

0 0 - 1
0 1 - 0
1 0 - 0
1 1 - 0

0 0   1
x 1   0
1 x   0

OR-NOT Value to support forwarding: 0
// -----------------------
XOR
a b - c
-------
0 0 - 0
0 1 - 1
1 0 - 1
1 1 - 0

same
// -----------------------
THREE_OR

1 x x    1
x 1 x    1
x x 1    1
0 0 0    0


*/
int Node::amountDCubes()
{
    if (this->calc_amount_DCubes == -1){
        QList<int>* singular_table_size = this->singularTableSize();
        QList<int>* singular_table = this->singularTable();

        int x = singular_table_size->at(0);
        int y = singular_table_size->at(1);

        this->count0 = 0;
        this->count1 = 0;
        this->zero_indexes = new QList<int>();
        this->one_indexes = new QList<int>();
        for (int i = 0; i < x; i++){
            if (singular_table->at((i + 1) * (y - 1)) == 0){
                this->zero_indexes->append(i);
                this->count0++;
            }
            else {
                this->one_indexes->append(i);
                this->count1++;
            }
        }

        int amount = 0;
        for (int index1 = 0; index1 < this->count0; index1++){
            for (int index2 = 0; index2 < this->count1; index2++){
                amount += 2;
            }
        }
        this->calc_amount_DCubes = amount;
    }
    return this->calc_amount_DCubes;
}

QList<int>* Node::getSingularLine(int line_number)
{
    QList<int>* singular_table_size = this->singularTableSize();
    QList<int>* singular_table = this->singularTable();

    int str = singular_table_size->at(0);
    int col = singular_table_size->at(1);

    QList<int>* singular_line = new QList<int>();
    for (int item_index = 0; item_index < col; item_index++){
        int item = singular_table->at(line_number * col + item_index);
        singular_line->append(item);
    }
    return singular_line;
}

int Node::singularIntersectOperation(int operand1, int operand2)
{
    if (operand1 == 0 && operand2 == 0){
        return 0;
    }
    else if (operand1 == 0 && operand2 == INSERSECTION_X){
        return 0;
    }
    else if (operand1 == INSERSECTION_X && operand2 == 0){
        return 0;
    }
    // ------------------------------
    else if (operand1 == 1 && operand2 == 1){
        return 1;
    }
    else if (operand1 == 1 && operand2 == INSERSECTION_X){
        return 1;
    }
    else if (operand1 == INSERSECTION_X && operand2 == 1){
        return 1;
    }
    else if (operand1 == INSERSECTION_X && operand2 == INSERSECTION_X){
        return INSERSECTION_X;
    }
    // ------------------------------
    else if (operand1 == 1 && operand2 == 0){
        return INSERSECTION_D;
    }
    else if (operand1 == 0 && operand2 == 1){
        return INSERSECTION_NOT_D;
    }
    // ------------------------------
}

int  Node::cubeIntersect(int operand1, int operand2){
    if (operand1 == operand2)
        return operand1;
    else if (operand1 == INSERSECTION_X)
        return operand2;
    else if (operand2 == INSERSECTION_X)
        return operand1;
    return INTERSECTION_NULL;
}

QList<int>* Node::cubeIntersection(QList<int>* line1, QList<int>* line2)
{
    QList<int>* intersection_result = new QList<int>();
    for (int index = 0; index < line1->count(); index++){
        int item = this->singularIntersectOperation(line1->at(index), line2->at(index));
        if (item == INTERSECTION_NULL)
            return NULL;
        intersection_result->append(item);
    }
    return intersection_result;
}

QList<int>* Node::singularIntersection(QList<int>* line1, QList<int>* line2)
{
    QList<int>* intersection_result = new QList<int>();
    for (int index = 0; index < line1->count(); index++){
        int item = this->singularIntersectOperation(line1->at(index), line2->at(index));
        intersection_result->append(item);
    }
    return intersection_result;
}

QList<int>* Node::getDCubesWithNumber(int number)
{
    int index_number = 0;
    if (this->calc_amount_DCubes == -1){
        this->amountDCubes();
    }
    for (int indexZero = 0; indexZero < this->count0; indexZero++){
        for (int indexOne = 0; indexOne < this->count1; indexOne++){
            if (index_number == number){
                int oneIndex = this->one_indexes->at(indexOne);
                int zeroIndex = this->zero_indexes->at(zeroIndex);


                QList<int>* singular_line1 = singular_table->at(zeroIndex);
                QList<int>* singular_line2 = singular_table->at(oneIndex);
                QList<int>* dcube = this->intersection(singular_line1, singular_line2);
                return dcube;
            }
            index_number++;
        }
    }
    return NULL;
}
QList<int>* Node::singularTableSize()
{
    QList<int>* singularTableSizePair = new QList<int>();
    switch (this->elementType){
        case LogicType_OR: {
            singularTableSizePair->append(3);
            singularTableSizePair->append(3);
            break;
        };
        case LogicType_NOT: {
            singularTableSizePair->append(2);
            singularTableSizePair->append(2);
            break;
        };
        case LogicType_XOR: {
            singularTableSizePair->append(4);
            singularTableSizePair->append(3);
            break;
        };

        case LogicType_THREE_OR: {
            singularTableSizePair->append(4);
            singularTableSizePair->append(4);
            break;
        };

        case LogicType_AND_NOT: {
            singularTableSizePair->append(3);
            singularTableSizePair->append(3);
            break;
        };
        case LogicType_OR_NOT: {
            singularTableSizePair->append(3);
            singularTableSizePair->append(3);
            break;
        };
        default:{
            return NULL;
        }
    }
    return singularTableSizePair;
}

QList<int>* Node::singularTable()
{
    QList<int>* singularTable = new QList<int>();
    switch (this->elementType){
        case LogicType_OR: {
            singularTable->append(1);
            singularTable->append(10);
            singularTable->append(1);

            singularTable->append(10);
            singularTable->append(1);
            singularTable->append(1);

            singularTable->append(0);
            singularTable->append(0);
            singularTable->append(0);
            break;
        };
        case LogicType_NOT: {
            singularTable->append(1);
            singularTable->append(0);
            singularTable->append(0);
            singularTable->append(1);

            break;
        };
        case LogicType_XOR: {
            singularTable->append(0);
            singularTable->append(0);
            singularTable->append(0);

            singularTable->append(0);
            singularTable->append(1);
            singularTable->append(1);

            singularTable->append(1);
            singularTable->append(0);
            singularTable->append(1);

            singularTable->append(1);
            singularTable->append(1);
            singularTable->append(0);

            break;
        };

        case LogicType_THREE_OR: {
            singularTable->append(1);
            singularTable->append(10);
            singularTable->append(10);
            singularTable->append(1);


            singularTable->append(10);
            singularTable->append(1);
            singularTable->append(10);
            singularTable->append(1);

            singularTable->append(10);
            singularTable->append(10);
            singularTable->append(1);
            singularTable->append(1);


            singularTable->append(0);
            singularTable->append(0);
            singularTable->append(0);
            singularTable->append(1);

            break;
        };

        case LogicType_AND_NOT: {
            singularTable->append(10);
            singularTable->append(0);
            singularTable->append(1);

            singularTable->append(0);
            singularTable->append(10);
            singularTable->append(1);

            singularTable->append(1);
            singularTable->append(1);
            singularTable->append(0);

            break;
        };
        case LogicType_OR_NOT: {
            singularTable->append(0);
            singularTable->append(0);
            singularTable->append(1);

            singularTable->append(10);
            singularTable->append(1);
            singularTable->append(0);

            singularTable->append(1);
            singularTable->append(10);
            singularTable->append(0);

            break;
        };
        default:{
            singularTable = NULL;
        }
    }
    return singularTable;
}
