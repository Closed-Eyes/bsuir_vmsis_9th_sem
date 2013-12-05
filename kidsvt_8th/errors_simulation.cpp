#pragma once
#include "constants.h"
#include "import.h"
#include "memory.cpp"
#include "file_output.cpp"

long pnpskf_found;
long anpskf_found;

long pnpskf_used;
long anpskf_used;

short** faults;
short** faults_links;
short** broken_cells;
short** broken_cells_found;

void setErrorState(long row_index, long column_index, short type)
{
    if ((broken_cells_found[row_index][column_index] == FOUND_FAULT) || (broken_cells[row_index][column_index] == type))
        return;

    broken_cells[row_index][column_index] = type;

    switch (type) {
        case PNPSFK5:
            pnpskf_used++;
            break;
        case ANPSFK9:
            anpskf_used++;
    }
}


/*
    ### Read/write bits external (as it is tested)
    ### including faults use
*/

short readBit_t(long row_index, long column_index)
{
    return readBit(row_index, column_index);
}

void writeBit_t(long row_index, long column_index, short value)
{
    if (faults[row_index][column_index] == PNPSFK5){
        short s1 = readBit(row_index - 1, column_index);
        short s2 = readBit(row_index + 1, column_index);
        short s3 = readBit(row_index, column_index - 1);
        short s4 = readBit(row_index, column_index + 1);

        if ((s1 == PNPSFK5_VALUE) && (s2 == PNPSFK5_VALUE)
            && (s3 == PNPSFK5_VALUE) && (s4 == PNPSFK5_VALUE))
            setErrorState(row_index, column_index, PNPSFK5);
            return;
        }

    if (faults_links[row_index][column_index] >= APN_TOP_RIGHT &&
        faults_links[row_index][column_index] <= APN_RIGHT_MIDDLE){

        short link_value = readBit(row_index, column_index);
        writeBit(row_index, column_index, value);
        if (link_value == value)
            return;

        short base_value;
        switch (faults_links[row_index][column_index]) {
            case APN_TOP_RIGHT:
                base_value = readBit(row_index + 1, column_index - 1);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index + 1, column_index - 1, base_value);
                setErrorState(row_index + 1, column_index - 1, ANPSFK9);
                break;
            case APN_TOP_LEFT:
                base_value = readBit(row_index + 1, column_index + 1);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index + 1, column_index + 1, base_value);
                setErrorState(row_index + 1, column_index + 1, ANPSFK9);
                break;
            case APN_TOP_MIDDLE:
                base_value = readBit(row_index + 1, column_index);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index + 1, column_index, base_value);
                setErrorState(row_index + 1, column_index, ANPSFK9);
                break;
            case APN_BOTTOM_RIGHT:
                base_value = readBit(row_index - 1, column_index - 1);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index - 1, column_index - 1, base_value);
                setErrorState(row_index - 1, column_index - 1, ANPSFK9);
                break;
            case APN_BOTTOM_LEFT:
                base_value = readBit(row_index - 1, column_index + 1);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index - 1, column_index + 1, base_value);
                setErrorState(row_index - 1, column_index + 1, ANPSFK9);
                break;
            case APN_BOTTOM_MIDDLE:
                base_value = readBit(row_index - 1, column_index);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index - 1, column_index, base_value);
                setErrorState(row_index - 1, column_index, ANPSFK9);
                break;
            case APN_LEFT_MIDDLE:
                base_value = readBit(row_index, column_index + 1);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index, column_index + 1, base_value);
                setErrorState(row_index, column_index - 1, ANPSFK9);
                break;
            case APN_RIGHT_MIDDLE:
                base_value = readBit(row_index, column_index - 1);
                base_value = (base_value == 0)? 1 : 0;
                writeBit(row_index, column_index - 1, base_value);
                setErrorState(row_index, column_index - 1, ANPSFK9);
                break;
        }
        return;
    }

    writeBit(row_index, column_index, value);
}


void allocErrorStorage()
{
    faults = new short*[RAM_CAPACITY_ROWS];
    faults_links = new short*[RAM_CAPACITY_ROWS];
    broken_cells = new short*[RAM_CAPACITY_ROWS];
    broken_cells_found = new short*[RAM_CAPACITY_ROWS];

    for (long i = 0; i < RAM_CAPACITY_ROWS; i++){
        faults[i] = new short[RAM_CAPACITY_COLUMNS];
        faults_links[i] = new short[RAM_CAPACITY_COLUMNS];
        broken_cells[i] = new short[RAM_CAPACITY_COLUMNS];
        broken_cells_found[i] = new short[RAM_CAPACITY_COLUMNS];


    }

}

void zeroResults()
{
    for (long i = 0; i < RAM_CAPACITY_ROWS; i++){
        for (long j = 0; j < RAM_CAPACITY_COLUMNS; j++){
            broken_cells[i][j] = NO_FAULT;
            broken_cells_found[i][j] = NO_EXISTING_FAULT;
        }
    }

    anpskf_found = 0;
    anpskf_used = 0;

    pnpskf_found = 0;
    pnpskf_used = 0;
}

void processErrorForIndex(long row_index, long column_index)
{
    short fault_type = broken_cells[row_index][column_index];

    if (broken_cells_found[row_index][column_index] == NO_EXISTING_FAULT){
        if (fault_type == ANPSFK9){
            anpskf_found++;
        }
        else if (fault_type == PNPSFK5){
            pnpskf_found++;
        }

        broken_cells_found[row_index][column_index] = FOUND_FAULT;
    }
}

void printStatistic()
{
    cout <<  "pnpskf5 : " << pnpskf_found << " / " << pnpskf_used << endl;
    cout <<  "anpskf9 : " << anpskf_found << " / " << anpskf_used << endl;
}
