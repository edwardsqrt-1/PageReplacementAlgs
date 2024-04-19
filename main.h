/*
 * Header file for main.cpp; contains the Page Table Class
 */

// Including any C++ libraries
#include <iostream> 
#include <queue>

// Make an unallocated flag
static const int UNALLOCATED_INT = -1; // -1 will demonstrate that the page is not loaded

// Page Table Class
struct PageTable {

    // Variable Declarations
    int* table;                      // Page Table itself
    unsigned int table_length;       // Page Table Length

    // Null Page Table Constructor
    PageTable() : table_length(0), table(nullptr) {} 

    // Page Table of a given length constructor
    PageTable(unsigned int len) {
        table_length = len;
        table = (int*)malloc(table_length*sizeof(unsigned int));
        for (int i = 0; i < table_length; i++) table[i] = UNALLOCATED_INT;
    }

    void clear() {
        for (int i = 0; i < table_length; i++) table[i] = UNALLOCATED_INT;
    }
 
    // Page Table Destructor
    ~PageTable() { free(table); }

};