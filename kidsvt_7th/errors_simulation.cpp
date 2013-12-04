#pragma once
#include "constants.h"
#include "import.h"
#include "memory.cpp"
#include "file_output.cpp"

long af1_found;
long af2_found;
long af3_found;
long af4_found;

long saf0_found;
long saf1_found;

long cfin_found;
long cfid_found;

long af1_used;
long af2_used;
long af3_used;
long af4_used;

long saf0_used;
long saf1_used;

long cfin_used;
long cfid_used;

short* faults;
short* broken_cells;
short* broken_cells_found;

void setErrorState(long index, short type)
{
    if ((broken_cells_found[index] == FOUND_FAULT) || (broken_cells[index] == type))
        return;

    broken_cells[index] = type;

    switch (type) {
    case AF1_FAULT:
        af1_used++;
        break;
    case AF2_FAULT:
        af2_used++;
        break;
    case AF3_FAULT:
        af3_used++;
        break;
    case AF4_FAULT:
        af4_used++;
        break;
    case SAF0_FAULT:
        saf0_used++;
        break;
    case SAF1_FAULT:
        saf1_used++;
        break;
    case CFID_FAULT:
        cfid_used++;
        break;
    case CFin_FAULT:
        cfin_used++;
        break;
    }
}

// ##### Modeling faults

/* ### [Faults Name] Address Decoder Faults - AF
  ### [Variants]    1) No one element can be accessed using current address
  ### [Variants]    2) Have no access to the current element using current address
  ### [Variants]    3) Multiple elements access using current address
      [Explanation] write in one - setted in several
  ### [Variants]    4) Access to the current element can be performed using multiple adresses
      [Explanation] read/write in many - setted in one
*/

// AF1 - error on write and read 1 + 1
// AF2 - error on write and read 1 + 1
// AF3 - error on write 1 + 1 + ...
// AF4 - error on write 1 + 1 + ...

short readAF(long index_address, short type)
{
    switch (type) {
        case AF1_FAULT:
            setErrorState(index_address, type);
            return BROKEN_CELL;
        case AF2_FAULT:
            setErrorState(index_address, type);
            return BROKEN_CELL;
        case AF3_FAULT:
            return readBit(index_address);
        case AF4_FAULT:
            return readBit(index_address);
    }
    return BROKEN_CELL;
}

void write_once_to_multiple(long address, short value)
{
    long half = (long)((float)RAM_CAPACITY / 2.0);
    if (address < 30) {

        writeBit(half - 5, value);
        writeBit(half - 20, value);
        writeBit(half - 25, value);

        setErrorState(half - 5, AF3_FAULT);
        setErrorState(half - 20, AF3_FAULT);
        setErrorState(half - 25, AF3_FAULT);
    }
    else if (address > RAM_CAPACITY - 30){
        writeBit(half + 5, value);
        writeBit(half + 20, value);
        writeBit(half + 25, value);

        setErrorState(half + 5, AF3_FAULT);
        setErrorState(half + 20, AF3_FAULT);
        setErrorState(half + 25, AF3_FAULT);

    }
    else {
        // search multiple addresses faults in range [start_address, start_address + 20]
        writeBit(address - 5, value);
        writeBit(address - 7, value);
        writeBit(address - 10, value);

        setErrorState(address - 5, AF3_FAULT);
        setErrorState(address - 7, AF3_FAULT);
        setErrorState(address - 10, AF3_FAULT);
    }
    writeBit(address, value);
}

void write_redirected(long index, short value)
{
    short up_offset = 5;
    short down_offset = -5;

    long addrU = index + up_offset;
    long addrD = index + down_offset;
    if (index + up_offset < RAM_CAPACITY){
        writeBit(addrU, value);
        setErrorState(addrU, AF4_FAULT);
    }
    else {
        writeBit(addrD, value);
        setErrorState(addrD, AF4_FAULT);
    }
}

void writeAF(long index_address, short type, short value)
{
    if (type == AF1_FAULT || type == AF2_FAULT)
        return;

    if (type == AF3_FAULT)
        write_once_to_multiple(index_address, value);
    else if (type == AF4_FAULT)
        write_redirected(index_address, value);

    return;
}

/* ### [Faults Name] Stuck-At Faults - SAF
   ### [Explanation] Constant value in element SAF0/SAF1
*/

short readSAF(long index, short fault_type)
{
    setErrorState(index, fault_type);
    if (fault_type == SAF0_FAULT)
        return SAF0_VALUE;
    else
        return SAF1_VALUE;
}

void writeSAF(long index, short fault_type)
{
    setErrorState(index, fault_type);
}

/*  ### [Faults Name] Inversion Coupling Faults - CFin
   ### [Description] Inversive influence 2 elements: a[i] and a[j]
   ### [..]          a[i] - aggressor
   ### [..]          a[j] - victim
   ### [Explanation] if aggressor was setted by value - victim will be setted inversely
*/

short readCFin(long index)
{
    return readBit(index);
}

void writeCFin(long index, short value)
{
    int offset = index % 20;
    short value_write = (value == 0) ? 1:0 ;


    if (index + offset > RAM_CAPACITY){
        writeBit(index - offset, value_write);
        setErrorState(index - offset, CFin_FAULT);
    }
    else if (index - offset < 0){
        writeBit(index + offset, value_write);
        setErrorState(index + offset, CFin_FAULT);
    }
    else {
        writeBit(index + offset, value_write);
        setErrorState(index + offset, CFin_FAULT);
    }
}

/*  ### [Faults Name] Idempotent Coupling Faults - CFid
   ### [Explanation] if aggressor was setted by value - victim will be setted constantly 0/1
*/

short readCFid(long index)
{
    return readBit(index);
}

void writeCFid(long index, short value)
{
    int offset = index % 20;
    short value_write = CFID_VALUE;


    if (index + offset > RAM_CAPACITY){
        writeBit(index - offset, value_write);
        setErrorState(index - offset, CFID_FAULT);
    }
    else if (index - offset < 0){
        writeBit(index + offset, value_write);
        setErrorState(index + offset, CFID_FAULT);
    }
    else {
        writeBit(index + offset, value_write);
        setErrorState(index + offset, CFID_FAULT);
    }
}

/*
    ### Read/write bits external (as it is tested)
    ### including faults use
*/

short readBit_t(long index)
{
    short fault_type = faults[index];
    switch (fault_type) {
        case AF1_FAULT:
            return readAF(index, fault_type);
        case AF2_FAULT:
            return readAF(index, fault_type);
        case AF3_FAULT:
            return readAF(index, fault_type);
        case AF4_FAULT:
            return readAF(index, fault_type);
        case SAF0_FAULT:
            return readSAF(index, fault_type);
        case SAF1_FAULT:
            return readSAF(index, fault_type);
        case CFin_FAULT:
            return readCFin(index);
        case CFID_FAULT:
            return readCFid(index);
        case NO_FAULT:
            return readBit(index);
    }
}

void writeBit_t(long index, short value)
{
    short fault_type = faults[index];
    switch (fault_type) {
        case AF1_FAULT:
            writeAF(index, fault_type, value);
            break;
        case AF2_FAULT:
            writeAF(index, fault_type, value);
            break;
        case AF3_FAULT:
            writeAF(index, fault_type, value);
            break;
        case AF4_FAULT:
            writeAF(index, fault_type, value);
            break;
        case SAF0_FAULT:
            writeSAF(index, fault_type);
            break;
        case SAF1_FAULT:
            writeSAF(index, fault_type);
            break;
        case CFin_FAULT:
            writeCFin(index, value);
            break;
        case CFID_FAULT:
            writeCFid(index, value);
            break;
        case NO_FAULT:
            writeBit(index, value);
            break;
    }
}


void allocErrorStorage()
{
    faults = new short[RAM_CAPACITY];
    broken_cells = new short[RAM_CAPACITY];
    broken_cells_found = new short[RAM_CAPACITY];
}

void zeroResults()
{
    for (long i = 0; i < RAM_CAPACITY; i++){
        broken_cells[i] = NO_FAULT;
        broken_cells_found[i] = NO_EXISTING_FAULT;
    }

    af1_found = 0;
    af2_found = 0;
    af3_found = 0;
    af4_found = 0;

    saf0_found = 0;
    saf1_found = 0;

    cfin_found = 0;
    cfid_found = 0;

    af1_used = 0;
    af2_used = 0;
    af3_used = 0;
    af4_used = 0;

    saf0_used = 0;
    saf1_used = 0;

    cfin_used = 0;
    cfid_used = 0;
}

void processErrorForIndex(long index)
{
    short fault_type = broken_cells[index];

    if (broken_cells_found[index] == NO_EXISTING_FAULT){
        switch (fault_type) {
            case AF1_FAULT:
                af1_found++;
                break;
            case AF2_FAULT:
                af2_found++;
                break;

            case AF3_FAULT:
                af3_found++;
                break;
            case AF4_FAULT:
                af4_found++;
                break;
            case SAF0_FAULT:
                saf0_found++;
                break;
            case SAF1_FAULT:
                saf1_found++;
                break;
            case CFID_FAULT:
                cfid_found++;
                break;
            case CFin_FAULT:
                cfin_found++;
                break;
        }

        broken_cells_found[index] = FOUND_FAULT;
    }
}

void printStatistic()
{
    cout <<  "af1 : " << af1_found << " / " << af1_used << endl;
    cout <<  "af2 : " << af2_found << " / " << af2_used<< endl;
    cout <<  "af3 : " << af3_found << " / " << af3_used<< endl;
    cout <<  "af4 : " << af4_found << " / " << af4_used<< endl;

    cout <<  "saf0 :" << saf0_found << " / " << saf0_used<< endl;
    cout <<  "saf1 :" << saf1_found << " / " << saf1_used<< endl;

    cout <<  "cfin :" << cfin_found << " / " << cfin_used<< endl;
    cout <<  "cfid :" << cfid_found << " / " << cfid_used<< endl;
}
