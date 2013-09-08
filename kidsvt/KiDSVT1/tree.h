#ifndef TREE_H
#define TREE_H

#include <QList>
#include "node.h"

class Tree
{
public:
    Node* brokenElement;

public:
    Tree();

    QList getTestDataForElement(QString name);


    Node* findElement(QString nameElement);
    Node* passTreeTopBottom();
private:
    Node* topNode;

    int broken_value;
    QList<Node*> initialInputs;
};

#endif // TREE_H
