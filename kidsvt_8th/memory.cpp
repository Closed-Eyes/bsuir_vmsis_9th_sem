#pragma once
#include "import.h"
#include "constants.h"

short** ram;

// ### Basic memory operations
void initialize()
{
   ram = new short*[RAM_CAPACITY_ROWS];
   for (long i = 0; i < RAM_CAPACITY_ROWS; i++){
       ram[i] = new short[RAM_CAPACITY_COLUMNS];
   }
   srand (time(NULL));
}

void zeroMemory()
{
   for (long i = 0; i < RAM_CAPACITY_ROWS; i++)
       for (long j = 0; j < RAM_CAPACITY_COLUMNS; j++)
           ram[i][j] = 0;
}

void writeBit(long row, long column, short value)
{
    ram[row][column] = value;
}

short readBit(long row, long column)
{
    return ram[row][column];
}
