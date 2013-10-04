#ifndef NODE_H
#define NODE_H

#include "node.h"
#include <QList>
#include <QString>

enum LogicType {LogicType_OR, LogicType_AND, LogicType_AND_NOT,
                LogicType_OR_NOT, LogicType_THREE_OR_NOT,
                LogicType_THREE_OR, LogicType_NOT, LogicType_XOR};

#define INSERSECTION_D 9
#define INSERSECTION_NOT_D 100
#define INSERSECTION_X 10


class Node
{
public:
    QList<Node*>* input_array;
    Node* output;

    LogicType elementType;
    QString name;
    int current_value;  // value setted to and need to achieve by using inputs signals

    bool is_DType; // supporting part of nodes in the three
    bool is_last;      // top node in hierarchy
    bool is_constant;  // not node but initial values, all is_constant nodes - Test Signals - the result of the app
    bool is_broken;    // broken node all the others works around it

    int count0;
    int count1;

    QList<int>* zero_indexes;
    QList<int>* one_indexes;
    int calc_amount_DCubes;
    int amountDCubes();
    QList<int>* getDCubesWithNumber(int number);
    QList<int>* getSingularLine(int line_number);
    QList<int>* intersection(QList<int>* line1, QList<int>* line2);
    int intersectOperation(int operand1, int operand2);
public:
    Node();

    QList<int>* singularTableSize();
    QList<int>* singularTable();
};

#endif // NODE_H
