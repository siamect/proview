/** 
 * Proview   $Id: co_tree.h,v 1.4 2006-12-10 14:33:34 lw Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#ifndef co_tree_h
#define co_tree_h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __STDDEF_LOADED
#include <stddef.h>
#endif

#ifndef co_tree_msg_h
#include "co_tree_msg.h"
#endif

typedef struct tree_s_Table tree_sTable;
typedef struct tree_s_Alloc tree_sAlloc;
typedef struct tree_s_Node tree_sNode;

struct tree_s_Alloc {
    tree_sAlloc *next;
};

struct tree_s_Node {
    tree_sNode *parent;
    tree_sNode *left;
    tree_sNode *right;
    int        bal;
};

struct tree_s_Table {
    tree_sNode  *root;
    tree_sNode  *null;
    tree_sNode  *key;
    tree_sNode  *free;
    tree_sNode  *last;
    tree_sAlloc *firstAlloc;
    ptrdiff_t   keyOffset;
    size_t      keySize;
    size_t		recordSize;
    int			lastComp;
    int			allocCount;
    int         (*compareFunc) (tree_sTable *tp, tree_sNode *x, tree_sNode *y);
    int			nNode;
    int			nError;
    int			nHZ;
    int			nHN;
    int			nHP;
    int			maxDepth;
    int			nLL;
    int			nLR;
    int			nRL;
    int			nRR;
    int			nMalloc;
    int			nAlloc;
    int			nFree;
    int			nInsert;
    int			nDelete;
    int			nSearch;
};

/*
  Function prototypes to exported functions.
*/

int
tree_Cardinality(pwr_tStatus *sts, tree_sTable *tp);

tree_sTable *
tree_CloneTable(pwr_tStatus *sts, tree_sTable *tp);

tree_sTable *
tree_CreateTable(
    pwr_tStatus  *sts,
    size_t       keySize,
    ptrdiff_t    keyOffset,
    size_t       recordSize,
    unsigned int allocCount,
    int          (*compareFunc) (tree_sTable *tp, tree_sNode *x, tree_sNode *y)
    );

void
tree_DeleteTable(pwr_tStatus *sts, tree_sTable *tp);

void *
tree_Find(pwr_tStatus *sts, tree_sTable *tp, void *key);

void *
tree_FindPredecessor(pwr_tStatus *sts, tree_sTable *tp, void *key);

void *
tree_FindSuccessor(pwr_tStatus *sts, tree_sTable *tp, void *key);

int
tree_TableIsEmpty(pwr_tStatus *sts, tree_sTable *tp);

void
tree_EmptyTable(pwr_tStatus *sts, tree_sTable *tp);

void *
tree_Insert(pwr_tStatus *sts, tree_sTable *tp, void *key);

void *
tree_Maximum(pwr_tStatus *sts, tree_sTable *tp);

void *
tree_Minimum(pwr_tStatus *sts, tree_sTable *tp);

void *
tree_Predecessor(pwr_tStatus *sts, tree_sTable *tp, void *np);

void
tree_PrintTable(
    pwr_tStatus *sts,
    tree_sTable *tp,
    void        (*printNode)(tree_sNode *),
    char        *(*printKey)(tree_sNode *)
);

void
tree_Remove(pwr_tStatus *sts, tree_sTable *tp, void *key);

void *
tree_Successor(pwr_tStatus *sts, tree_sTable *tp, void *np);

int
tree_Comp_cid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

int
tree_Comp_nid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

int
tree_Comp_vid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

int
tree_Comp_uint32(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

int
tree_Comp_oix(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

int
tree_Comp_tid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y);

int
tree_Comp_oid(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

int
tree_Comp_time(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

int
tree_Comp_memcmp(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

int
tree_Comp_strncmp(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

int
tree_Comp_int32(tree_sTable *tp, tree_sNode *x, tree_sNode *y);

#ifdef __cplusplus
}
#endif

#endif
