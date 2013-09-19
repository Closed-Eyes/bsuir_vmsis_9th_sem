#ifndef NODE_H
#define NODE_H

#include "node.h"
#include <QList>
#include <QString>

enum LogicType {LogicType_OR, LogicType_AND, LogicType_AND_NOT,
                LogicType_OR_NOT, LogicType_THREE_OR_NOT,
                LogicType_THREE_OR, LogicType_NOT, LogicType_XOR};

class Node
{
public:
    QList<Node*>* input_array;
    Node* output;

    LogicType elementType;
    QString name;
    int current_value;  // value setted to and need to achieve by using inputs signals

    bool is_under_the_broken;
    bool is_forwarded; // supporting part of nodes in the three
    bool is_last;      // top node in hierarchy
    bool is_constant;  // not node but initial values, all is_constant nodes - Test Signals - the result of the app
    bool is_broken;    // broken node all the others works around it
public:
    Node();

    // набор значений для входов текущего элемента
    // для получения конкретного значения на выходе
    QList<int>* valuesForGettingDeterminedValue(int val);

    // это значение для множества сигналов, которое не определяет результат текущего
    // выходного логического элемента. То есть результат будет определять пробрасываемое значениe
    int valueForwardSupport();
};

#endif // NODE_H
