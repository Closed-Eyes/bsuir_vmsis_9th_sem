#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QString>


enum LogicType {OR, AND, AND_NOT, OR_NOT, THREE_OR_NOT};

class Node
{
public:
    QList<Node*> input_array;
    QList<Node*> output_array;

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
    QList valuesForGettingDeterminedValue(int valueToGet);

    // это значение для множества сигналов, которое не определяет результат текущего
    // выходного логического элемента. То есть результат будет определять пробрасываемое значениe
    int valueForwardSupport();
};

#endif // NODE_H
