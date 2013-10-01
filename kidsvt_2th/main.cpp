#include "tree.h"
#include "node.h"
#include <QDebug>
#include <iostream>
// ------------------------------------------------------------


using namespace std;

int main(int argc, char *argv[])
{
    Tree* scheme = new Tree();
    scheme->setContantValueBrokenElement(0);
    Node* broken_element = scheme->findElement("F1");

    QList<Node*>* listOfTestData = scheme->getTestDataForElement("F6");
    for (int index = 0; index < listOfTestData->count(); index++){
        Node* n = listOfTestData->at(index);
        QString str = QString::number(n->current_value );
        qDebug() << n->name+ " " + str;
    }
    int x;
    cin >> x;
    return 0;
    return 0;
}
