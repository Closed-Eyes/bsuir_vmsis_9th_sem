#ifndef TREE_H
#define TREE_H

#include <QList>
#include <QString>
#include <node.h>

class Tree
{
public:
    Node* brokenElement;

public:
    Tree();

    QList<Node*>* getTestDataForElement(QString name);
    void setContantValueBrokenElement(int brokenValue);

    Node* findElement(QString nameElement);
    Node* passTreeTopBottom();
    Node* passTree(Node* currentNode, QString nameToFind);

    void saveBrokenWayElement(Node* elementToSave);
    QList<Node*>* goUpAndMarkBrokenWay(Node* currentElement);

    Node* topNode;
    int broken_value;
    QList<Node*> initialInputs;
    QList<Node*>* brokenWayElementsList;
};

#endif // TREE_H
