#include "import.h"
#include "memory.cpp"
#include "errors_simulation.cpp"
#include "file_output.cpp"

// ### Basic memory operations
void initialize()
{
//   cout << "start using memory with " << RAM_CAPACITY / MEGABIT << " Mbit" << endl;

//   ram.resize(RAM_CAPACITY);
   ram = new short[RAM_CAPACITY];
   srand (time(NULL));
}

void zeroMemory()
{
   for (long index = 0; index < RAM_CAPACITY; index++)
       ram[index] = 0;
}

/*
   ### Generating faults
*/

void generateFaults()
{
    short counter = 8;
    long af1 = 0, af2 = 0, af4 = 0, af3 = 0, saf0 = 0, saf1 = 0, cfin = 0, cfid = 0;

    long index = 20;
    while (index < RAM_CAPACITY){

        if (counter == 0){
            af1++;
            faults[index] = AF1_FAULT;
        }
        else if (counter == 1){
            af2++;
            faults[index] = AF2_FAULT;
        }
        else if (counter == 2){
            faults[index] = AF3_FAULT;
            af3++;
        }
        else if (counter == 3){
            faults[index] = AF4_FAULT;
            af4++;
        }
        else if (counter == 4){
            saf0++;
            faults[index] = SAF0_FAULT;
        }
        else if (counter == 5){
            saf1++;
            faults[index] = SAF1_FAULT;
        }
        else if (counter == 6){
            cfin++;
            faults[index] = CFin_FAULT;
        }
        else if (counter == 7){
            cfid++;
            faults[index] = CFID_FAULT;
        }
        else
            faults[index] = NO_FAULT;

        if (counter == -1)
            counter = 8;
        else
            counter--;
        index += 20;
        \
    }

    cout << endl << " generated: af1: " << af1 << " af2: " << af2 << endl;
    cout << endl << " generated: af4: " << af4 << " af3: " << af3 << endl;

    cout << endl << " generated: saf1: " << saf1 << " saf0: " << saf0 << endl;
    cout << endl << " generated: cfin: " << cfin << " cfid: " << cfid << endl;

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

void perform_mats()
{
    // w0
    for (long index = 0; index < RAM_CAPACITY; index++){
        writeBit_t(index, 0);
    }

    // r0-w1
    for (long index = RAM_CAPACITY - 1; index >= 0; index--){
        short cell_value = readBit_t(index);
        if (cell_value == BROKEN_CELL){
            processErrorForIndex(index);
        }
        else if (0 != cell_value){
            processErrorForIndex(index);
        }
        writeBit_t(index, 1);
    }

    // r1
    for (long index = RAM_CAPACITY - 1; index >= 0; index--){
        short cell_value = readBit_t(index);
        if (cell_value == BROKEN_CELL){
            processErrorForIndex(index);
        }
        else if (1 != cell_value){
            processErrorForIndex(index);
        }
    }
}


/*
   ### [Test Name]     GALPAT
   ### [Explanation]   1) selecting base element.
                       2) other than base set in 1
                          after each setted value check base element value
                       3) check all other than base elements values
                       4) go to 1) with other base
*/

void surround(long base_index)
{
   for (long surround_index = 0; surround_index < RAM_CAPACITY; surround_index ++){
       if (base_index == surround_index)
           continue;
       else {
           writeBit_t(surround_index, GALPAT_SURROUND_VALUE);
           short cell_value = readBit_t(surround_index);
           if (cell_value == BROKEN_CELL){
               processErrorForIndex(surround_index);
               continue;
           }else if (GALPAT_SURROUND_VALUE != cell_value){
               processErrorForIndex(surround_index);
               continue;
           }

           if (readBit_t(base_index) != GALPAT_BASE_VALUE){
               processErrorForIndex(base_index);
               writeBit(base_index, GALPAT_BASE_VALUE);
               continue;
           }
       }
   }
}

void perform_gaplat()
{
   for (long base_index = 0; base_index < RAM_CAPACITY; base_index++){
       writeBit_t(base_index, GALPAT_BASE_VALUE);
       short cell_value = readBit_t(base_index);
       if (cell_value == BROKEN_CELL){
            processErrorForIndex(base_index);
            continue;
       } else if (GALPAT_BASE_VALUE != cell_value){
            processErrorForIndex(base_index);
            continue;
       }
       surround(base_index);
   }
}


int main()
{
    initialize();
    allocErrorStorage();
    generateFaults();
    zeroResults();

    perform_gaplat();

    cout << endl << "GAPLAT testing." << endl << endl;
    printStatistic();

    fileClose();

    zeroResults();
    perform_mats();

    cout << endl << "MATS testing." << endl << endl;
    printStatistic();
    return 0;
}

