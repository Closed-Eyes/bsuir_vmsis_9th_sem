#include <QtCore/QCoreApplication>
#include <iostream>

enum LogicType {OR, NOT, XOR, EXC_XOR, AND_NOT, OR_NOT};


using std::cin;
using std::cout;

class Node
{
    QList<Node*> input_array;
    QList<Node*> output_array;

    LogicType elementType;
    QString name;
    int current_value;

    bool is_forwarded;
    bool is_last;
};

Node* passTree(Node* currentNode, QString nameToFind)
{
    cout << currentNode->name;
    if (currentNode->is_last){
        return NULL;
    }
    else {
        for (int i = 0; i < currentNode->input_array.count(); i++){
            Node* nodeToCheck = currentNode->input_array.at(i);

            if (nodeToCheck->name == nameToFind){
                return currentNode;
            }

            passTree(currentNode->input_array[i]);
        }

    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Node topNode;

    topNode.name = "top";

    Node topNode1;
    Node topNode2;
    Node topNode3;
    Node topNode4;
    Node topNode5;
    topNode.name = "node1";
    topNode.name = "node2";
    topNode.name = "node3";
    topNode.name = "node4";
    topNode.name = "node5";

    topNode.input_array.append(&topNode1);
    topNode.input_array.append(&topNode2);

    passTree(topNode, )

    return a.exec();
}
