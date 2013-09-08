#include <QString>
#include <QList>
#include <iostream>
#include <QDebug>

#include "node.h"
#include "tree.h"

using namespace std;

int main(int argc, char *argv[])
{
    Tree* scheme = new Tree();
    qDebug() << scheme->getTestDataForElement(scheme->topNode);
}
