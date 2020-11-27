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
        page_curr = q[proc_curr].pc / PAGESIZE;

        // pageout previous one page, pagein current & next one page
        if(q[proc_curr].pages[page_curr] == PAGE_INVAILED)
            pagein(proc_curr, page_curr);

        if(page_curr < MAXPROCPAGES - 1)
            if(q[proc_curr].pages[page_curr + 1] == PAGE_INVAILED)
                pagein(proc_curr, page_curr + 1);

        if(page_curr > 0)
            if(q[proc_curr].pages[page_curr - 1] == PAGE_VAILED)
                pageout(proc_curr, page_curr - 1);

        // additional page prediction (see program.c)
        if(page_curr == 0) {
            for(int idx = 2; idx < MAXPROCPAGES; idx ++)
                if(q[proc_curr].pages[idx] == PAGE_VAILED) pageout(proc_curr, idx);
        } else if(page_curr == 3) {     // 500 / 128 = 3.9; 1401 / 128 = 10.9; 1533 / 128 = 11.9
            if(q[proc_curr].pages[0] == PAGE_INVAILED) pagein(proc_curr, 0);
            if(q[proc_curr].pages[10] == PAGE_INVAILED) pagein(proc_curr, 10);
            if(q[proc_curr].pages[11] == PAGE_INVAILED) pagein(proc_curr, 11);
        } else if(page_curr == 4) {     // arrive 4, then no need for 10 & 11
            if(q[proc_curr].pages[10] == PAGE_VAILED) pageout(proc_curr, 10);
            if(q[proc_curr].pages[11] == PAGE_VAILED) pageout(proc_curr, 11);
        } else if(page_curr == 8) {     // 1129 / 128 = 8.8
            if(q[proc_curr].pages[0] == PAGE_INVAILED) pagein(proc_curr, 0);
        } else if(page_curr == 9) {     // 1166 / 128 = 9.1; decending, no need for 13
            if(q[proc_curr].pages[13] == PAGE_VAILED) pageout(proc_curr, 13);
            if(q[proc_curr].pages[14] == PAGE_VAILED) pageout(proc_curr, 14);
        } else if(page_curr == 10) {    // arrive 10, then no need for 3 & 4
            if(q[proc_curr].pages[3] == PAGE_VAILED) pageout(proc_curr, 3);
            if(q[proc_curr].pages[4] == PAGE_VAILED) pageout(proc_curr, 4);
        } else if(page_curr == 11) {    // 1533 / 128 = 11.9
            if(q[proc_curr].pages[0] == PAGE_INVAILED) pagein(proc_curr, 0);
        } else if(page_curr == 12) {     // 1682 / 128 = 13.1; 1166 / 128 = 9.1; 1683 / 128 = 13.1
            if(q[proc_curr].pages[0] == PAGE_INVAILED) pagein(proc_curr, 0);
            if(q[proc_curr].pages[9] == PAGE_INVAILED) pagein(proc_curr, 9);
        }
    }

    /* advance time for next pageit iteration */
    tick++;
}
