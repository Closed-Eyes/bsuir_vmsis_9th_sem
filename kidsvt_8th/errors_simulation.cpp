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
            ;;;


        broken_cells_found[row_index][column_index] = FOUND_FAULT;
    }
}

void printStatistic()
{
    cout <<  "pnpskf5 : " << pnpskf_found << " / " << pnpskf_used << endl;
    cout <<  "anpskf9 : " << anpskf_found << " / " << anpskf_used << endl;
}
