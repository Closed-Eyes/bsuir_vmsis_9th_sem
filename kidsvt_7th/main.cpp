#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <map>
#include <vector>
#include <cstdlib>

#define MEGABIT 1000000
//#define RAM_CAPACITY 2*MEGABIT
#define RAM_CAPACITY 10*1024
#define BITS_IN_BYTE 8

#define BROKEN_CELL -1

#define FOUND_FAULT 5
#define NO_EXISTING_FAULT 6

#define GALPAT_BASE_VALUE 0
#define GALPAT_SURROUND_VALUE 1

#define NO_FAULT 0

#define AF1_FAULT 10
#define AF2_FAULT 11
#define AF3_FAULT 12
#define AF4_FAULT 13

#define SAF0_FAULT 21
#define SAF1_FAULT 22

#define SAF0_VALUE 0
#define SAF1_VALUE 1

#define CFin_FAULT 31

#define CFID_FAULT 41
#define CFID_VALUE 1


using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef unsigned char u_char;

//static vector<u_char> ram;
//static vector<u_char> faults;
//static vector<u_char> found_faults;

u_char* ram;
u_char* faults;
u_char* found_faults;

long errors_used = 0;

// ### Basic memory operations
void initialize()
{
   cout << "start using memory with " << RAM_CAPACITY / MEGABIT << " Mbit" << endl;

//   ram.resize(RAM_CAPACITY);
   ram = new u_char[RAM_CAPACITY];
   srand (time(NULL));
}

void allocErrorStorage()
{
//    faults.resize(RAM_CAPACITY);
//    found_faults.resize(RAM_CAPACITY);
    faults = new u_char[RAM_CAPACITY];
    found_faults = new u_char[RAM_CAPACITY];
}

void writeBit(long index, u_char value)
{
   ram[index] = value;
}

u_char readBit(long index)
{
    return ram[index];
}

void zeroMemory()
{
   for (long index = 0; index < RAM_CAPACITY; index++)
       ram[index] = 0;
}

void processErrorForIndex(int index, int correctValue)
{
//   found_faults[index] = FOUND_FAULT;/
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



u_char readAF(long index_address, short type)
{
    switch (type) {
        case AF1_FAULT:
            return -1;
        case AF2_FAULT:
            return -1;
        case AF3_FAULT:
            return readBit(index_address);
        case AF4_FAULT:
            return readBit(index_address);
    }
    return -1;
}

void write_once_to_multiple(long address, u_char value)
{

    int start_address = abs(address - 20);

    int finish_address = start_address + 40;
    if (finish_address > RAM_CAPACITY)
        finish_address = RAM_CAPACITY;

    // search multiple addresses faults in range [start_address, start_address + 20]
    writeBit(finish_address - 5, value);
    writeBit(finish_address - 20, value);
    writeBit(finish_address - 30, value);
}

void write_redirected(long index, u_char value)
{
    u_char error_type = faults[index];

    if (error_type != AF4_FAULT){
        return;
    }

    /* generate secret number between 1 and 10: */
    short up_offset = 5;
    short down_offset = -5;

    if (index + up_offset < RAM_CAPACITY)
        writeBit(up_offset, value);
    else
        writeBit(down_offset, value);
}

void writeAF(long index_address, u_char type, u_char value)
{
    switch (type) {
        case AF1_FAULT:
            return;
        case AF2_FAULT:
            return;
        case AF3_FAULT:
            write_once_to_multiple(index_address, value);
            return;
        case AF4_FAULT:
            write_redirected(index_address, value);
            return;
    }
}

/* ### [Faults Name] Stuck-At Faults - SAF
   ### [Explanation] Constant value in element SAF0/SAF1
*/

u_char readSAF(long index, u_char fault_type)
{
    if (fault_type == SAF0_FAULT)
        return SAF0_VALUE;
    else
        return SAF1_VALUE;
}

void writeSAF(long index, u_char fault_type)
{
    return;
}

/*  ### [Faults Name] Inversion Coupling Faults - CFin
   ### [Description] Inversive influence 2 elements: a[i] and a[j]
   ### [..]          a[i] - aggressor
   ### [..]          a[j] - victim
   ### [Explanation] if aggressor was setted by value - victim will be setted inversely
*/

u_char readCFin(long index)
{
    return readBit(index);
}

void writeCFin(long index, u_char value)
{
    int offset = index % 20;
    int start_address = index + offset;

    int finish_address = start_address + 20;
    if (finish_address > RAM_CAPACITY)
        finish_address = RAM_CAPACITY;

    u_char value_write = (value == 0) ? 1:0 ;
    writeBit(index, value_write);


}

/*  ### [Faults Name] Idempotent Coupling Faults - CFid
   ### [Explanation] if aggressor was setted by value - victim will be setted constantly 0/1
*/

u_char readCFid(long index)
{
    return readBit(index);
}

void writeCFid(long index, u_char value)
{
    int offset = index % 20;
    int start_address = index + offset;

    int finish_address = start_address + 20;
    if (finish_address > RAM_CAPACITY)
        finish_address = RAM_CAPACITY;

    u_char value_write = CFID_VALUE;
    writeBit(index, value_write);
}

/*
    ### Read/write bits external (as it is tested)
    ### including faults use
*/

u_char readBit_t(long index)
{
    u_char fault_type = faults[index];
    if (found_faults[index] == FOUND_FAULT) {
        return -1;
    }
    switch (fault_type) {
        case AF1_FAULT:
            errors_used++;
            return readAF(index, fault_type);
        case AF2_FAULT:
            errors_used++;
            return readAF(index, fault_type);
        case AF3_FAULT:
            errors_used++;
            return readAF(index, fault_type);
        case AF4_FAULT:
            errors_used++;
            return readAF(index, fault_type);
        case SAF0_FAULT:
            errors_used++;
            return readSAF(index, fault_type);
        case SAF1_FAULT:
            errors_used++;
            return readSAF(index, fault_type);
        case CFin_FAULT:
            errors_used++;
            return readCFin(index);
        case CFID_FAULT:
            errors_used++;
            return readCFid(index);
        case NO_FAULT:
            return readBit(index);
    }
}

void writeBit_t(long index, u_char value)
{
    u_char fault_type = faults[index];
    if (found_faults[index] == FOUND_FAULT) {
        return;
    }
    switch (fault_type) {
        case AF1_FAULT:
            errors_used++;
            writeAF(index, fault_type, value);
            break;
        case AF2_FAULT:
            errors_used++;
            writeAF(index, fault_type, value);
            break;
        case AF3_FAULT:
            errors_used+=3;
            writeAF(index, fault_type, value);
            break;
        case AF4_FAULT:
            errors_used++;
            writeAF(index, fault_type, value);
            break;
        case SAF0_FAULT:
            errors_used++;
            writeSAF(index, fault_type);
            break;
        case SAF1_FAULT:
            errors_used++;
            writeSAF(index, fault_type);
            break;
        case CFin_FAULT:
            errors_used++;
            writeCFin(index, value);
            break;
        case CFID_FAULT:
            errors_used++;
            writeCFid(index, value);
            break;
        case NO_FAULT:
            writeBit(index, value);
            break;
    }
}

/*
   ### Generating faults
*/

long generateFaults()
{
    long errors_generated = 0;
    for (int index = 0; index < RAM_CAPACITY; index ++){
        //creating fault in 40 percent
        if (rand() % 100 < 6){
            errors_generated++;
            u_char fault_num = rand() % 8;
            switch (fault_num){
                case 0:
                    faults[index] = AF1_FAULT;
                    break;
                case 1:
                    faults[index] = AF2_FAULT;
                    break;
                case 2:
                    faults[index] = AF3_FAULT;
                    errors_generated+=2;
                    break;
                case 3:
                    faults[index] = AF4_FAULT;
                    break;
                case 4:
                    faults[index] = SAF0_FAULT;
                    break;
                case 5:
                    faults[index] = SAF1_FAULT;
                    break;
                case 6:
                    faults[index] = CFin_FAULT;
                    break;
                case 7:
                    faults[index] = CFID_FAULT;
                    break;
            default:
                faults[index] = NO_FAULT;
            }
        }
    }
    return errors_generated;
}

/*
   ### [Test Name]     GALPAT
   ### [Explanation]   1) selecting base element.
                       2) other than base set in 1
                          after each setted value check base element value
                       3) check all other than base elements values
                       4) go to 1) with other base
*/

long surround(long base_index)
{
   int errors_found = 0;
   for (long surround_index = 0; surround_index < RAM_CAPACITY; surround_index ++){
       if (base_index == surround_index)
           continue;
       else {
           writeBit_t(surround_index, GALPAT_SURROUND_VALUE);
           u_char cell_value = readBit_t(surround_index);
           if (cell_value == BROKEN_CELL){
               continue;
           }
           if (GALPAT_SURROUND_VALUE != cell_value){
               errors_found++;
               processErrorForIndex(surround_index, GALPAT_SURROUND_VALUE);
           }
       }
   }
   return errors_found;
}

long perform_gaplat()
{
   long errors_found = 0;
   for (long base_index = 0; base_index < RAM_CAPACITY; base_index++){
       cout << "base_index check:" << base_index << endl;
       cout << "\terrors:" << errors_found << endl;
       writeBit_t(base_index, GALPAT_BASE_VALUE);
       errors_found += surround(base_index);
       u_char cell_value = readBit_t(base_index);
       if (cell_value == BROKEN_CELL){
           continue;
       }
       if (GALPAT_BASE_VALUE != cell_value){
            errors_found++;
            processErrorForIndex(base_index, GALPAT_BASE_VALUE);
       }
   }
   return errors_found;
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

long perform_mats()
{
    long errors_found = 0;
    // w0
    for (long index = 0; index < RAM_CAPACITY; index++){
        writeBit_t(index, 0);
    }

    // r0-w1
    for (long index = RAM_CAPACITY - 1; index >= 0; index--){
        u_char cell_value = readBit_t(index);
        if (cell_value == BROKEN_CELL){
            continue;
        }
        if (0 != cell_value){
            errors_found++;
        }
        writeBit_t(index, 1);
    }

    // r1
    for (long index = RAM_CAPACITY - 1; index >= 0; index--){
        u_char cell_value = readBit_t(index);
        if (cell_value == BROKEN_CELL){
            continue;
        }
        if (1 != cell_value){
            errors_found++;
        }
    }
    return errors_found;
}

int main()
{
    initialize();
    allocErrorStorage();
    long errors_generated = generateFaults();
    long errors_found_gaplat = perform_gaplat();
    long errors_found_mats = perform_mats();
    cout << endl << "errors gen count: " << errors_generated << ", errors found MATS: " << errors_found_mats;
    cout << endl << "errors gen count: " << errors_generated << ", errors found GAPLAT: " << errors_found_gaplat;
    cout << endl << "errors used: " << errors_used;
}
