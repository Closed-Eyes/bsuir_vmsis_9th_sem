#include "tree.h"

Tree::Tree()
{
    this->topNode = new Node();

    Node* node11 = new Node();
    Node* node12 = new Node();
    Node* node121 = new Node();
    Node* node122 = new Node();

    this->topNode->name = "topNode";
    this->topNode->is_last = true;
    node11->name = "level2_left";
    node12->name = "level2_right";
    node121->name = "level2_right_level3_left";
    node122->name = "level2_right_level3_right";

    node11->input_array.append(node121);
    node11->input_array.append(node122);

    this->topNode->input_array.append(node11);
    this->topNode->input_array.append(node12);

    this->broken_value = 0;
}

void Tree::setContantValueBrokenElement(int brokenValue)
{
    this->broken_value = brokenValue;
}

QList Tree::getTestDataForElement(QString name)
{
    Node* brokenElement = this->findElement(name);

    // set needed value for broken element in under broken children tree
    goDownAndSetValues(brokenElement, 0);

    goUpAndMark(brokenElement);
}

void Tree::goUpAndMark(Node* currentNode)
{
    // mark
    if (!currentNode->is_broken){
        currentNode->is_forwarded = true;
        currentNode->current_value  = -1;
        // pass down tree from this node
        goDownMarkAndCalculate(currentNode);
    }

    // go up until you can
    if (currentNode->is_last){
        return;
    }
    else {
        for (int index = 0; index < currentNode->output_array.count(); index++){
            Node* outputNode = currentNode->output_array.at(index);
            goUpAndMark(outputNode);
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

    QList values;
    if (currentNode->is_broken){
        int reverseValue = (this->broken_value == 0) ? 1 : 0;
        values = currentNode->valuesForGettingDeterminedValue(reverseValue);
    }
    else{
        currentNode->is_under_the_broken = true;
        currentNode->current_value = valueToSet;
        values = currentNode->valuesForGettingDeterminedValue(valueToSet);
    }

    for (int index = 0; index < currentNode->input_array.count(); index++){
        Node* oneOfTheChilds = currentNode->input_array.at(index);
        goDownAndSetValues(oneOfTheChilds, values.at(index));
    }
}

//supporting values - for forwarding
Node* goDownMarkAndCalculate(Node* currentNode)
{
    if (currentNode->is_forwarded){
        int value = currentNode->valueForwardSupport();
        for (int index = 0; index < currentNode->input_array.count(); index++){
            Node* childNode = currentNode->input_array.at(index);
            if (!childNode->is_broken && !childNode->is_forwarded){
                childNode->current_value = value;
                goDownMarkAndCalculate(childNode);
            }
        }
    }
    // not forwarded
    else {
        QList values = currentNode->valuesForGettingDeterminedValue(currentNode->current_value);
        for (int index = 0; index < currentNode->input_array.count(); index++){
            Node* childNode = currentNode->input_array.at(index);
            if (!childNode->is_broken && !childNode->is_forwarded){
                childNode->current_value = values.at(index);
                if (!childNode->is_constant)
                    goDownMarkAndCalculate(childNode);
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
    qDebug() << currentNode->name;
    if (currentNode->input_array.count() == 0){
        return NULL;
    }
    else {
        Node* iteratingNode;
        for (int i = 0; i < currentNode->input_array.count(); i++){
            Node* nodeToCheck = currentNode->input_array.at(i);

            if (nodeToCheck->name == nameToFind){
                return currentNode;
            }

            iteratingNode = this->passTree(currentNode->input_array[i], "");
        }
        return iteratingNode;
    }
}

