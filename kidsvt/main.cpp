#include "tree.h"
#include <QDebug>
#include <iostream>
// ------------------------------------------------------------


using namespace std;

int main(int argc, char *argv[])
{
    Tree* scheme = new Tree();
    scheme->passTree(scheme->topNode, "");

    QList<Node*>* listOfTestData =  scheme->getTestDataForElement("F5");
    for (int index = 0; index < listOfTestData->count(); index++){
        Node* n = listOfTestData->at(index);
        QString str = QString::number( n->current_value );
        qDebug() << n->name+ " " + str;
    }
    int x;
    cin >> x;
    return 0;
    return 0;
}
