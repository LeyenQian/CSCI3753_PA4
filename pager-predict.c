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

typedef struct _PAGE_PRED
{
    int next[2];
    // ...
}PAGE_PRED, *P_PAGE_PRED;

int prev_pred_point[MAXPROCESSES];
PAGE_PRED page_pred[MAXPROCPAGES] = {-1};
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

        if (q[proc_curr].active == PROC_INACTIVE) {
            for(int idx = 0; idx < q[proc_curr].npages; idx ++)
                if(q[proc_curr].pages[idx] == PAGE_VAILED) pageout(proc_curr, idx);
            continue;
        }
        
        // get current page, only process invailed pages
        page_curr = q[proc_curr].pc / PAGESIZE;
        if (q[proc_curr].pages[page_curr] == PAGE_INVAILED)
        {
            if(prev_pred_point[proc_curr] == -1) {
                prev_pred_point[proc_curr] = page_curr;
                pagein(proc_curr, page_curr);

                if(page_curr >= 0 && page_curr < MAXPROCPAGES - 2) {
                    pagein(proc_curr, page_curr + 1);
                    pagein(proc_curr, page_curr + 2);
                    page_pred[prev_pred_point[proc_curr]].next[0] = page_curr + 1;
                    page_pred[prev_pred_point[proc_curr]].next[0] = page_curr + 2;
                } else {
                    pagein(proc_curr, page_curr - 1);
                    pagein(proc_curr, page_curr - 2);
                    page_pred[prev_pred_point[proc_curr]].next[0] = page_curr - 1;
                    page_pred[prev_pred_point[proc_curr]].next[0] = page_curr - 2;
                }
            } else {
                for (int idx = 0; idx < 2; idx ++) {
                    if (page_curr == page_pred[prev_pred_point[proc_curr]].next[idx]) continue;
                    page_pred[prev_pred_point[proc_curr]].next[idx] = page_curr; break;
                }
                
                for (int idx = 0; idx < 2; idx ++) {
                    if(pagein(proc_curr, page_pred[prev_pred_point[proc_curr]].next[idx]) == OP_PAGE_SUCCESS) continue;
                    for(int page_idx = 0; page_idx < q[proc_curr].npages; page_idx ++) {
                        if(q[proc_curr].pages[page_idx] == PAGE_INVAILED) continue;
                        if(page_curr == page_idx) continue;
                        if(page_idx == page_pred[prev_pred_point[proc_curr]].next[0]) continue;
                        if(page_idx == page_pred[prev_pred_point[proc_curr]].next[1]) continue;
                        pageout(proc_curr, page_idx); break;
                    }
                    pagein(proc_curr, page_pred[prev_pred_point[proc_curr]].next[idx]);
                }
            }
        }
 
    }

    /* advance time for next pageit iteration */
    tick++;
}
