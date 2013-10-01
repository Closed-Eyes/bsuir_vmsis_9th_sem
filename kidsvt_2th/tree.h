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
    void goDownAndSetValues(Node* currentNode, int valueToSet);
    Node* goDownMarkAndCalculate(Node* currentNode);
    QList<Node*>* fillTheConstants(QList<Node*>* constantList, Node* currentNode);

    Node* topNode;
    int broken_value;
    QList<Node*> initialInputs;
    void goUpAndMark(Node* currentNode);
};

#endif // TREE_H
