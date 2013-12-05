#include "import.h"
#include "memory.cpp"
#include "errors_simulation.cpp"
#include "file_output.cpp"

/*
   ### Generating faults
*/

void generateFaults()
{
    long pnpsfk = 0, anpsfk = 0;

    bool setPN = true;
    for (long row = 1; row < RAM_CAPACITY_ROWS - 1; row+=8){
        for (long column = 1; column < RAM_CAPACITY_COLUMNS - 1; column+=8){

            if ((column > 0) && (column < RAM_CAPACITY_COLUMNS - 1)){
                if ((row > 0) && (row < RAM_CAPACITY_ROWS - 1)){
                    if (setPN){
                        faults[row][column] = PNPSFK5;
                        faults_links[row - 1][column] = TOP_MIDDLE;
                        faults_links[row][column - 1] = LEFT_MIDDLE;
                        faults_links[row][column + 1] = RIGHT_MIDDLE;
                        faults_links[row + 1][column] = BOTTOM_MIDDLE;
                        pnpsfk++;
                    }
                    else {
                        faults[row][column] = ANPSFK9;
                        faults_links[row - 1][column] = APN_TOP_MIDDLE;
                        faults_links[row - 1][column - 1] = APN_TOP_LEFT;
                        faults_links[row - 1][column + 1] = APN_TOP_RIGHT;

                        faults_links[row][column + 1] = APN_RIGHT_MIDDLE;
                        faults_links[row][column - 1] = APN_LEFT_MIDDLE;

                        faults_links[row + 1][column] = APN_BOTTOM_MIDDLE;
                        faults_links[row + 1][column - 1] = APN_BOTTOM_LEFT;
                        faults_links[row + 1][column + 1] = APN_BOTTOM_RIGHT;
                        anpsfk++;
                    }
                    setPN = !setPN;
                }
            }
        }
    }


    cout << endl << " generated: anpsfk: " << anpsfk;
    cout << endl << " generated: pnpsfk: " << pnpsfk << endl;

}

/*
    ### [Test Name]     MATS
    ### [Description]   Complexity = 4N
    ...                 <-> (w0) <- (r0, w1) <-> (r1)
    ### [Explanation]   <-> - march direction doesn't matter, <-/-> reverse direction
                        w0 - write 0 to cell
                        w1 - write 1 to cell
                        r0 - read  0 from cell
                        r1 - read  1 from cell
*/

void perform_matc_line()
{
    // w0
    for (long row_index = 0; row_index < RAM_CAPACITY_ROWS; row_index++){
        for (long column_index = 0; column_index < RAM_CAPACITY_ROWS; column_index++){
            writeBit_t(row_index, column_index, 0);
        }
    }

    // r0-w1
    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            short cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);
        }
    }

    // r1-w0

    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            short cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);
        }
    }

    // r0-w1
    for (long i = 0; i < RAM_CAPACITY_ROWS; i++){
        for (long j = 0; j < RAM_CAPACITY_COLUMNS; j++){
            short cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);
        }
    }

    // r1-w0
    for (long i = 0; i < RAM_CAPACITY_ROWS; i++){
        for (long j = 0; j < RAM_CAPACITY_COLUMNS; j++){
            short cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);
        }
    }

    // r0
    for (long i = 0; i < RAM_CAPACITY_ROWS; i++){
        for (long j = 0; j < RAM_CAPACITY_COLUMNS; j++){
            short cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
        }
    }
}

void perform_march_ps()
{
    short cell_value;
    // w0
    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            writeBit_t(i, j, 0);
        }
    }

    // r0-w1-r1-w0-r0-w1
    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            // r0-w1
            cell_value = readBit_t(i,j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);

            // r1-w0
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);

            // r0-w1
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);
        }
    }

    // r1-w0-r0-w1-r1
    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            // r1-w0
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);

            // r0-w1
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);

            // r1
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
        }
    }

    // r1-w0-r0-w1-r1-w0
    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            // r1-w0
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);

            // r0-w1
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);

            // r1-w0
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);
        }
    }

    // r0-w1-r1-w0-r0-w1
    for (long i = RAM_CAPACITY_ROWS - 1; i >= 0; i--){
        for (long j = RAM_CAPACITY_COLUMNS - 1; j >= 0; j--){
            // r0-w1
            short cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 1);

            // r1-w0
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (1 != cell_value){
                processErrorForIndex(i, j);
            }
            writeBit_t(i, j, 0);

            // r0-w1
            cell_value = readBit_t(i, j);
            if (cell_value == BROKEN_CELL){
                processErrorForIndex(i, j);
            }
            else if (0 != cell_value){
                processErrorForIndex(i, j);
            }
       }
    }


}

int main()
{
    initialize();
    allocErrorStorage();
    generateFaults();

    zeroResults();

    perform_matc_line();
    printStatistic();

    zeroResults();

    perform_march_ps();
    printStatistic();

    return 0;
}

