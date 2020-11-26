/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

// ******************************** new pre-define ****************************************************************
#define PROC_ACTIVE     1   // process running
#define PROC_INACTIVE   0   // process exited
#define PAGE_VAILED     1   // page allocated
#define PAGE_INVAILED   0   // page not allocated
#define OP_PAGE_SUCCESS 1   // page [in/out] started, running or finished
#define OP_PAGE_FAILED  0   // page [in/out] cannot start, because swapping [in/out]
// ******************************** new pre-define ****************************************************************


void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time

    // ******************************** new static vars ****************************************************************
    static int proc_last; // initial: 0, changes in future calling
    // ******************************** new static vars ****************************************************************
    
    /* Local vars */
    // ******************************** new local vars ****************************************************************
    int proc_curr;
    int page_curr;
    // ******************************** new local vars ****************************************************************

    /* initialize static vars on first run */
    if(!initialized){
	/* Init complex static vars here */
	
	initialized = 1;
    }
    
    /* TODO: Implement Predictive Paging */
    // fprintf(stderr, "pager-predict not yet implemented. Exiting...\n");
    // exit(EXIT_FAILURE);
    for(proc_curr = proc_last; proc_curr < MAXPROCESSES; proc_curr ++){
        proc_last = proc_last == MAXPROCESSES - 1 ? 0 : proc_last + 1;
        if (q[proc_curr].active == PROC_INACTIVE) continue;
        
        // get current page, only process invailed pages
        page_curr = q[proc_curr].pc / PAGESIZE;
        if (q[proc_curr].pages[page_curr] == PAGE_VAILED) continue;

        if (page_curr == 0) {
            for (int i = 3; i < 15; i++)
                pageout(proc_curr, i);
        }

        // record tick, if pagein operation succeed
        if (pagein(proc_curr, page_curr) == OP_PAGE_FAILED) {
            for(int page_idx = 0; page_idx < q[proc_curr].npages; page_idx ++) {
                if(q[proc_curr].pages[page_idx] == PAGE_INVAILED) continue;
                if(page_idx == page_curr || page_idx == page_curr + 1 || page_idx == page_curr + 2) continue;
                pageout(proc_curr, page_idx); break;
            }
            pagein(proc_curr, page_curr);
        }
        
        if(q[proc_curr].pages[page_curr + 1] == PAGE_INVAILED)
            if (pagein(proc_curr, page_curr + 1) == OP_PAGE_FAILED) {
                for(int page_idx = 0; page_idx < q[proc_curr].npages; page_idx ++) {
                    if(q[proc_curr].pages[page_idx] == PAGE_INVAILED) continue;
                    if(page_idx == page_curr || page_idx == page_curr + 1 || page_idx == page_curr + 2) continue;
                    pageout(proc_curr, page_idx); break;
                }
                pagein(proc_curr, page_curr + 1);
            }

        // if(q[proc_curr].pages[page_curr + 2] == PAGE_INVAILED)
        //     if (pagein(proc_curr, page_curr + 2) == OP_PAGE_FAILED) {
        //         for(int page_idx = 0; page_idx < q[proc_curr].npages; page_idx ++) {
        //             if(q[proc_curr].pages[page_idx] == PAGE_INVAILED) continue;
        //             if(page_idx == page_curr || page_idx == page_curr + 1 || page_idx == page_curr + 2) continue;
        //             pageout(proc_curr, page_idx); break;
        //         }
        //         pagein(proc_curr, page_curr + 2);
        //     }

        break;
    }

    /* advance time for next pageit iteration */
    tick++;
}
