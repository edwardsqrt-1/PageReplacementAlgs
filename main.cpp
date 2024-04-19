/* 
 * Creator: Edward Bierens
 * Date Due: 18 April 2024 
 * Language: C++
 * 
 * Description: This program aims to get a good understanding of page replacement algorithms
 * by implementing the three most common choices: FIFO, LRU, and OPT. 
 *	
 */

// Include the header file
#include "main.h"

// Implementation of First In, First Out Algorithm for Page Replacement
void FIFOAlgorithm(PageTable& pt, int* pages, unsigned int string_len) {

    // Declaring variables for hits and faults
    unsigned int hits = 0, faults = 0;

    // We don't need queues here, rather I will use a "counter"
    // Goal: For every page fault, use this number mod the page table length to get the index needed to change
    // This emulates a first in, first out system
    int selected_idx = 0;

    // Iterate through all pages in the list
    for (int cur_page_idx = 0; cur_page_idx < string_len; cur_page_idx++) {

        // Prints out the current iteration level
        std::cout << "Iteration " << cur_page_idx + 1 << ": ";

        // Tries to find a hit 
        bool is_hit = 0;
        for (int k = 0; k < pt.table_length; k++) if (pages[cur_page_idx] == pt.table[k]) {is_hit = 1; break;}
        
        // If it is a miss, then it is a page fault, otherwise simply increment hits
        if (!is_hit) {

            // If frame table is full, replace the first one (which was the first in)
            if (selected_idx >= pt.table_length) pt.table[selected_idx % pt.table_length] = pages[cur_page_idx];
            
            // Otherwise just add it to the next empty index (update faults variable too)
            else pt.table[selected_idx] = pages[cur_page_idx];
            faults++;

            // Print out that there's a page fault with this page
            std::cout << "Page Fault at " << pages[cur_page_idx];

            // Increment the counter (selected index)
            selected_idx++;

        // Print out that there's a page hit with this page and update hits
        } else {std::cout << "Page Hit at " << pages[cur_page_idx]; hits++; }

        // Print out the pages that were accessed, the current frame table, and the fault count
        std::cout << " - Pages Accessed: { ";
        for (int k = 0; k <= cur_page_idx; k++) std::cout << pages[k] << " ";
        std::cout << "}, Current Frame Table: [ ";
        int k;
        for (k = 0; k < pt.table_length; k++) if (pages[k] != UNALLOCATED_INT) std::cout << pt.table[k] << " ";
        std::cout << "]; Fault Count = " << faults << "\n";
        
        // Prints overall hits and faults
        std::cout << "Overall Hits: " << hits << "; Overall Page Faults: " << faults << "\n";
        
    }
}

// Implementation of Least Recently Used Algorithm
void LRUAlgorithm(PageTable& pt, int* pages, unsigned int string_len) {
    
    // Declaring variables for hits and faults
    unsigned int hits = 0, faults = 0;

    // Create a corresponding list of timers to correspond to each block in Page Table
    int timers[pt.table_length];
    for (int i = 0; i < pt.table_length; i++) timers[i] = UNALLOCATED_INT;
    
    // Iterate through page list
    for (int cur_page_idx = 0; cur_page_idx < string_len; cur_page_idx++) {

        // Prints out the current iteration level
        std::cout << "Iteration " << cur_page_idx + 1 << ": ";

        // Tries to find a hit, unlike with FIFO this will contain the index of the hit
        int is_hit = -1;
        for (int k = 0; k < pt.table_length; k++) if (pages[cur_page_idx] == pt.table[k]) {is_hit = k; break;}
        
        // If it is a miss (stays at -1), then it is a page fault, otherwise increase timer at the hit and increment hits
        if (is_hit == -1) {

            // Find the minimum of the timers (this will be the one that hasn't been recently used)
            // Note: If there's a tie, the one closer to the left will be replaced
            unsigned int minimum_index = 0;
            for (int k = 1; k < pt.table_length; k++) if (timers[minimum_index] > timers[k]) minimum_index = k;

            // Take that minimum index, replace the page table entry with the new page number, increment the timer
            pt.table[minimum_index] = pages[cur_page_idx];
            timers[minimum_index]++;
            
            // Increase faults and let user know of a fault
            faults++;
            std::cout << "Page Fault at " << pages[cur_page_idx];

        // otherwise increment timer and increase hits by 1, letting user know about a hit
        } else {
            timers[is_hit]++; 
            std::cout << "Page Hit at " << pages[cur_page_idx];
            hits++;
        }

        // Print out the pages that were accessed, the current frame table, and the fault count
        std::cout << " - Pages Accessed: { ";
        for (int k = 0; k <= cur_page_idx; k++) std::cout << pages[k] << " ";
        std::cout << "}, Current Frame Table: [ ";
        int k;
        for (k = 0; k < pt.table_length; k++) if (pages[k] != UNALLOCATED_INT) std::cout << pt.table[k] << " ";
        std::cout << "]; Fault Count = " << faults << "\n";

    }

    // Prints overall hits and faults
    std::cout << "Overall Hits: " << hits << "; Overall Page Faults: " << faults << "\n";
}

void OPTAlgorithm(PageTable& pt, int* pages, unsigned int string_len) {
    
    // Declare the hit and fault trackers, as well as counter
    unsigned int hits = 0, faults = 0, counter = 0;

    // Create a list of distances until the next time a page will see a page with the same number
    unsigned int distances[pt.table_length];

    // Iterates through the page list
    for (int cur_page_idx = 0; cur_page_idx < string_len; cur_page_idx++) {

        // Prints out the current iteration level
        std::cout << "Iteration " << cur_page_idx + 1 << ": ";

        // Initialize the distance table with a very large integer
        for (int i = 0; i < pt.table_length; i++) distances[i] = 65535;

        // Tries to find a hit, will also use a boolean value for now
        bool is_hit = 0;
        for (int k = 0; k < pt.table_length; k++) if (pages[cur_page_idx] == pt.table[k]) {is_hit = 1; break;}
        
        // If it is a miss (stays at -1), then it is a page fault. Apply OPT algorithm
        if (!is_hit) {

            int maximum_idx;

            if (counter >= pt.table_length) {

                // The OPT algorithm is based on when the next page of the same number will appear again
                // Replace the one that is furthest away (or non-existent)
                for (int k = 0; k < string_len; k++) {
                    for (int chk_pt = pt.table_length; chk_pt > cur_page_idx; chk_pt--) {
                        if (pages[k] == pt.table[chk_pt]) distances[chk_pt] = k;
                    }
                }

                // Find the maximum of all the distances
                maximum_idx = 0;
                for (int k = 1; k < pt.table_length; k++) if (distances[k] > distances[maximum_idx]) maximum_idx = k;
            } else { maximum_idx = counter; counter++; }
            pt.table[maximum_idx] = pages[cur_page_idx];

            // Increase faults and let user know of a fault. Also clear distances
            faults++;
            std::cout << "Page Fault at " << pages[cur_page_idx];


        // otherwise increment hits by 1, letting user know about a hit
        } else {
            std::cout << "Page Hit at " << pages[cur_page_idx];
            hits++;
        }

        // Print out the pages that were accessed, the current frame table, and the fault count
        std::cout << " - Pages Accessed: { ";
        for (int k = 0; k <= cur_page_idx; k++) std::cout << pages[k] << " ";
        std::cout << "}, Current Frame Table: [ ";
        int k;
        for (k = 0; k < pt.table_length; k++) if (pages[k] != UNALLOCATED_INT) std::cout << pt.table[k] << " ";
        std::cout << "]; Fault Count = " << faults << "\n";

    }

    // Prints overall hits and faults
    std::cout << "Overall Hits: " << hits << "; Overall Page Faults: " << faults << "\n";
}

//Driver program
int main() {

	// Initialize inputs
    const int TABLE_LENGTH = 4;
    int pages_list[] = {1,2,3,4,1,2,5,1,2,3,4,5};
    unsigned int pages_size = sizeof(pages_list)/sizeof(pages_list[0]);
    
    PageTable x(TABLE_LENGTH);


	// Testing First In First Out
    std::cout << "------------------------------------------\n";
    std::cout << "-- First In, First Out (FIFO) Algorithm --\n";
    std::cout << "------------------------------------------\n";
    FIFOAlgorithm(x, pages_list, pages_size);
    std::cout << "------------------------------------------\n\n";
    x.clear();

	// Testing Least Recently Used
    std::cout << "-----------------------------------------\n";
    std::cout << "-- Least Recently Used (LRU) Algorithm --\n";
    std::cout << "-----------------------------------------\n";
    LRUAlgorithm(x, pages_list, pages_size);
    std::cout << "-----------------------------------------\n\n";
    x.clear();

	// Testing Optimal Page Replacement
    std::cout << "----------------------------------------------\n";
    std::cout << "-- Optimal Page Replacement (OPT) Algorithm --\n";
    std::cout << "----------------------------------------------\n";
    OPTAlgorithm(x, pages_list, pages_size);
    std::cout << "----------------------------------------------\n\n";


}
