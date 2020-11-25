/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

// ******************************** new pre-define ********************************
#define PROC_ACTIVE     1   // process running
#define PROC_INACTIVE   0   // process exited
#define PAGE_VAILED     1   // page allocated
#define PAGE_INVAILED   0   // page not allocated
#define OP_PAGE_SUCCESS 1   // page [in/out] started, running or finished
#define OP_PAGE_FAILED  0   // page [in/out] cannot start, because swapping [in/out]
// ******************************** new pre-define ********************************

void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    // ******************************** new static vars ********************************
    static int proc_last; // initial: 0, changes in future calling
    // ******************************** new static vars ********************************

    /* Local vars */
    int proctmp;
    int pagetmp;

    // ******************************** new local vars ********************************
    int proc_curr;
    int page_curr;
    // ******************************** new local vars ********************************

    /* initialize static vars on first run */
    if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0; 
	    }
	}
	initialized = 1;
    }
    
    /* TODO: Implement LRU Paging */
    // fprintf(stderr, "pager-lru not yet implemented. Exiting...\n");
    // exit(EXIT_FAILURE);
    for(proc_curr = proc_last; proc_curr < MAXPROCESSES; proc_curr ++){
        proc_last = proc_last == MAXPROCESSES - 1 ? 0 : proc_last + 1;
        if (q[proc_curr].active == PROC_INACTIVE) continue;
        
        // get current page, only process invailed pages
        page_curr = q[proc_curr].pc / PAGESIZE;
        if (q[proc_curr].pages[page_curr] == PAGE_VAILED) continue;

        // record tick, if pagein operation succeed
        if (pagein(proc_curr, page_curr) == OP_PAGE_SUCCESS) {
            timestamps[proc_curr][page_curr] = tick;
            continue;
        }

        // try to find possible page to be swapped out
        for(int page_idx = q[proc_curr].npages - 1; page_idx >= 0; page_idx --) {
            if(q[proc_curr].pages[page_idx] == PAGE_INVAILED) continue;
            timestamps[proc_curr][page_idx] == tick ? timestamps[proc_curr][page_curr] = tick : pageout(proc_curr, page_idx);
            break;
        }
    }

    /* advance time for next pageit iteration */
    tick++;
} 
