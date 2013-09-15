#include "tree.h"
#include "qdebug.h"

Tree::Tree()
{
    Node* F1 = new Node();
    F1->name = "F1";
    Node* F2 = new Node();
    F2->name = "F2";
    Node* F3 = new Node();
    F3->name = "F3";
    Node* F4 = new Node();
    F4->name = "F4";
    Node* F5 = new Node();
    F5->name = "F5";
    Node* F6 = new Node();
    F6->name = "F6";

    this->topNode = F6;


    Node* x1 = new Node();
    x1->is_constant = true;
    x1->name = "x1";
    x1->output_array = new QList<Node*>();

    Node* x2 = new Node();
    x2->is_constant = true;
    x2->name = "x2";
    x2->output_array = new QList<Node*>();

    Node* x3 = new Node();
    x3->is_constant = true;
    x3->name = "x3";
    x3->output_array = new QList<Node*>();

    Node* x4 = new Node();
    x4->is_constant = true;
    x4->name = "x4";
    x4->output_array = new QList<Node*>();

    Node* x5 = new Node();
    x5->is_constant = true;
    x5->name = "x5";
    x5->output_array = new QList<Node*>();

    Node* x6 = new Node();
    x6->is_constant = true;
    x6->name = "x6";
    x6->output_array = new QList<Node*>();

    Node* x7 = new Node();
    x7->is_constant = true;
    x7->name = "x7";
    x7->output_array = new QList<Node*>();

//    4	ИЛИ-НЕ	И-НЕ	3ИЛИ-НЕ	ИЛИ	И

    F1->input_array = new QList<Node*>();
    F1->output_array = new QList<Node*>();
    F1->input_array->append(x1);
    F1->input_array->append(x2);
    F1->elementType = LogicType_OR_NOT;
    F1->is_last = false;

    F2->input_array = new QList<Node*>();
    F2->output_array = new QList<Node*>();
    F2->input_array->append(x3);
    F2->elementType= LogicType_NOT;
    F2->is_last = false;

    F3->input_array = new QList<Node*>();
    F3->output_array = new QList<Node*>();
    F3->input_array->append(x5);
    F3->input_array->append(x6);
    F3->elementType= LogicType_AND_NOT;
    F3->is_last = false;

    F4->input_array = new QList<Node*>();
    F4->output_array = new QList<Node*>();
    F4->input_array->append(x4);
    F4->input_array->append(F3);
    F4->input_array->append(x7);
    F4->elementType= LogicType_THREE_OR_NOT;
    F4->is_last = false;

    F5->input_array = new QList<Node*>();
    F5->output_array = new QList<Node*>();
    F5->input_array->append(F2);
    F5->input_array->append(F4);
    F5->elementType= LogicType_OR;
    F5->is_last = false;

    F6->input_array = new QList<Node*>();
    F6->output_array = new QList<Node*>();
    F6->input_array->append(F1);
    F6->input_array->append(F5);
    F6->elementType= LogicType_AND;
    F6->is_last = false;

    F1->output_array->append(F6);
    F2->output_array->append(F5);
    F3->output_array->append(F4);
    F4->output_array->append(F5);
    F5->output_array->append(F6);

    x1->output_array->append(F1);
    x2->output_array->append(F1);

    x3->output_array->append(F2);
    x4->output_array->append(F4);

    x5->output_array->append(F3);
    x6->output_array->append(F3);

    x7->output_array->append(F4);


    this->topNode->is_last = true;

    this->broken_value = 0;
}

void Tree::setContantValueBrokenElement(int brokenValue)
{
    this->broken_value = brokenValue;
}

QList<Node*>* Tree::getTestDataForElement(QString name)
{
    Node* brokenElement = this->findElement(name);
    brokenElement->is_broken = true;


    int reverse_value = 0;
    if (this->broken_value == 0)
        reverse_value = 1;
    QList<int>* values = brokenElement->valuesForGettingDeterminedValue(reverse_value);

    for (int i = 0; i < brokenElement->input_array->count(); i++){
        this->goDownAndSetValues(brokenElement->input_array->at(i), values->at(i));
    }
    this->goUpAndMark(brokenElement);
    return fillTheConstants(new QList<Node*>(), this->topNode);
}

void Tree::goToChildsAndCalc()
{

}

void Tree::goUpAndMark(Node* currentNode)
{
    Node* nodeToWork = currentNode;
    bool canGoUp = true;
    while (canGoUp){
        if (nodeToWork->is_broken){
            if (nodeToWork->is_last){
                QList<Node*>* inputs = nodeToWork->input_array;
                int reverse_value = 0;
                if (this->broken_value == 0)
                    reverse_value = 1;
                nodeToWork->current_value = this->broken_value;
                for (int index = 0; index < inputs->count(); index++){
                    Node* downNode = inputs->at(index);
                    this->goDownAndSetValues(downNode, reverse_value);
                }
                canGoUp = false;
            }
            else {
                nodeToWork->current_value = this->broken_value;
                int reverse_value = 0;
                if (this->broken_value == 0)
                    reverse_value = 1;
                for (int index = 0; index < inputs->count(); index++){
                    Node* downNode = inputs->at(index);
                    this->goDownAndSetValues(downNode, reverse_value);
                }

                nodeToWork = nodeToWork->output;
            }
        }
        else {
            if (nodeToWork->is_last){
                for (int i = 0; i < nodeToWork->input_array->count(); i++)
                    if (!nodeToWork->is_broken && !nodeToWork->is_forwarded)
                        this->goDownMarkAndCalculate(nodeToWork->input_array->at(i));
                canGoUp = false;
            }
            else if (nodeToWork->is_forwarded){
                for (int i = 0; i < nodeToWork->input_array->count(); i++)
                    if (!nodeToWork->is_broken && !nodeToWork->is_forwarded)
                        this->goDownMarkAndCalculate(nodeToWork->input_array->at(i));
                nodeToWork = nodeToWork->output;
            }

            //
        }
    }
}

// under the broken element tree part
void Tree::goDownAndSetValues(Node* currentNode, int valueToSet)
{
    // not node but initial value to the first nodes
    if (currentNode->is_constant){
        currentNode->is_under_the_broken = true;
        currentNode->current_value = valueToSet;
        return;
    }

    QList<int>* values;
    if (currentNode->is_broken){
        int reverseValue = (this->broken_value == 0) ? 1 : 0;
        values = currentNode->valuesForGettingDeterminedValue(reverseValue);
    }
    else{
        currentNode->is_under_the_broken = true;
        currentNode->current_value = valueToSet;
        values = currentNode->valuesForGettingDeterminedValue(valueToSet);
    }

    for (int index = 0; index < currentNode->input_array->count(); index++){
        Node* oneOfTheChilds = currentNode->input_array->at(index);
        goDownAndSetValues(oneOfTheChilds, values->at(index));
    }
}

//supporting values - for forwarding
Node* Tree::goDownMarkAndCalculate(Node* currentNode)
{
    if (currentNode->is_constant){
        return NULL;
    }

    qDebug() << "goDownMarkAndCalculate currentNode " + currentNode->name;
    if (currentNode->is_forwarded){
        int value = currentNode->valueForwardSupport();
        for (int index = 0; index < currentNode->input_array->count(); index++){
            Node* childNode = currentNode->input_array->at(index);
            if (!childNode->is_broken && !childNode->is_forwarded){
                childNode->current_value = value;
                qDebug() << "goDownMarkAndCalculate child_value " + value;
                this->goDownMarkAndCalculate(childNode);
            }
        }
    }
    // not forwarded
    else {
        qDebug() << "goDownMarkAndCalculate not_forwarded " + currentNode->name;
        QList<int>* values = currentNode->valuesForGettingDeterminedValue(currentNode->current_value);
        for (int index = 0; index < currentNode->input_array->count(); index++){
            Node* childNode = currentNode->input_array->at(index);
            if (!childNode->is_broken && !childNode->is_forwarded){
                childNode->current_value = values->at(index);
                qDebug() << "goDownMarkAndCalculate valuesForGettingDeterminedValue " + values->at(index);
                if (!childNode->is_constant)
                    this->goDownMarkAndCalculate(childNode);
            }
        }
    }
}

Node* Tree::passTreeTopBottom()
{
    this->passTree(this->topNode, "");
}

Node* Tree::findElement(QString nameElement)
{
    return this->passTree(this->topNode, nameElement);
}

Node* Tree::passTree(Node* currentNode, QString nameToFind)
{
    qDebug() << "current_node " + currentNode->name;
    if (currentNode->is_constant){
        qDebug() << "constant ";
        return NULL;
    }
    else {
        Node* iteratingNode;
        for (int i = 0; i < currentNode->input_array->count(); i++){
            Node* nodeToCheck = currentNode->input_array->at(i);

            qDebug() << "inputNode " + nodeToCheck->name;
            if (nodeToCheck->name.compare(nameToFind) == 0){
                qDebug() << "finded " + nodeToCheck->name;
                return nodeToCheck;
            }

            iteratingNode = this->passTree(nodeToCheck, nameToFind);
        }
        return iteratingNode;
    }
}

QList<Node*>* Tree::fillTheConstants(QList<Node*>* constantList, Node* currentNode)
{
    if (currentNode->is_constant){
        constantList->append(currentNode);
        return NULL;
    }
    else {
        for (int i = 0; i < currentNode->input_array->count(); i++){
            Node* nodeGoDown = currentNode->input_array->at(i);
            this->fillTheConstants(constantList, nodeGoDown);
        }
    }
    return constantList;
}
