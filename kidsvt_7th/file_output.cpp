#pragma once
#include "constants.h"
#include "import.h"

int fileOpen = 0;
ofstream myfile;

void writeFile(string s)
{
    if (fileOpen == 0){
        fileOpen = 1;
        myfile.open ("output.txt", std::ios::trunc);
    }
    myfile << s;
}

void writeFile(long i)
{
    if (fileOpen == 0){
        fileOpen = 1;
        myfile.open ("output.txt", std::ios::trunc);
    }
    myfile << i;
}

void fileClose()
{
    myfile.close();
}
