#pragma once
#include "import.h"
#include "constants.h"

short* ram;

void writeBit(long index, short value)
{
   ram[index] = value;
}

short readBit(long index)
{
    return ram[index];
}
