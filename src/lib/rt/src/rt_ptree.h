#ifndef rt_ptree_h
#define rt_ptree_h

#ifdef __cplusplus
extern "C" {
#endif

/* rt_ptree.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef __STDDEF_LOADED
#include <stddef.h>
#endif

#ifndef rt_pool_h
#include "rt_pool.h"
#endif

#ifndef co_tree_msg_h
#include "co_tree_msg.h"
#endif

typedef struct {
    pool_tRef next;
} ptree_sAlloc;

typedef struct {
    pool_tRef parent;
    pool_tRef left;
    pool_tRef right;
    int        bal;
} ptree_sNode;

typedef struct {
    int 	created;
    pool_tRef   root;
    pool_tRef   null;
    pool_tRef   key;
    pool_tRef   free;
    pool_tRef   firstAlloc;
    ptrdiff_t   keyOffset;
    size_t      keySize;
    size_t		recordSize;
    int			allocCount;
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
} ptree_sGtable;

typedef struct ptree_s_Table ptree_sTable;

struct ptree_s_Table {
    ptree_sGtable  *g;			/**< Part of table stored in pool */
    pool_sHead	   *php;		/**< Pool to use for storage.*/
    ptree_sNode	   *nullp;
    ptree_sNode	   *keyp;
    pool_tRef      last;
    int		   lastComp;
    int         (*compareFunc) (ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y);
};

/*
  Function prototypes to exported functions.
*/
void ptree_Init( pool_sHead   *php,
		 ptree_sGtable *gttp,
		 size_t       keySize,
		 ptrdiff_t    keyOffset,
		 size_t       recordSize,
		 unsigned int allocCount);

ptree_sTable *
ptree_CloneTable(pwr_tStatus *sts, ptree_sTable *tp);

ptree_sTable *ptree_Create( pwr_tStatus  *sts,
			    pool_sHead   *php,
			    ptree_sTable *ttp,
			    ptree_sGtable *gttp,
			    int (*compareFunc) (ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y));

void
ptree_DeleteTable(pwr_tStatus *sts, ptree_sTable *tp);

void *
ptree_Find(pwr_tStatus *sts, ptree_sTable *tp, void *key);

void *
ptree_FindPredecessor(pwr_tStatus *sts, ptree_sTable *tp, void *key);

void *
ptree_FindSuccessor(pwr_tStatus *sts, ptree_sTable *tp, void *key);

int
ptree_TableIsEmpty(pwr_tStatus *sts, ptree_sTable *tp);

void
ptree_EmptyTable(pwr_tStatus *sts, ptree_sTable *tp);

pool_tRef ptree_Insert( pwr_tStatus *sts, 
			ptree_sTable *tp, 
			void *key);

void *
ptree_Maximum(pwr_tStatus *sts, ptree_sTable *tp);

void *
ptree_Minimum(pwr_tStatus *sts, ptree_sTable *tp);

void *
ptree_Predecessor(pwr_tStatus *sts, ptree_sTable *tp, void *np);

void
ptree_PrintTable(
    pwr_tStatus *sts,
    ptree_sTable *tp,
    void        (*printNode)(ptree_sNode *, pool_tRef),
    char        *(*printKey)(ptree_sNode *)
);

void
ptree_Remove(pwr_tStatus *sts, ptree_sTable *tp, void *key);

void *
ptree_Successor(pwr_tStatus *sts, ptree_sTable *tp, void *np);

int
ptree_Comp_cid( ptree_sTable *tp, ptree_sNode  *x, ptree_sNode  *y);

int
ptree_Comp_nid( ptree_sTable *tp, ptree_sNode  *x, ptree_sNode  *y);

int
ptree_Comp_vid( ptree_sTable *tp, ptree_sNode  *x, ptree_sNode  *y);

int
ptree_Comp_uint32( ptree_sTable *tp, ptree_sNode  *x, ptree_sNode  *y);

int
ptree_Comp_oix( ptree_sTable *tp, ptree_sNode  *x, ptree_sNode  *y);

int
ptree_Comp_tid( ptree_sTable *tp, ptree_sNode  *x, ptree_sNode  *y);

int
ptree_Comp_oid( ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y);

int
ptree_Comp_time( ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y);

int
ptree_Comp_memcmp( ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y);

int
ptree_Comp_strncmp( ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y);

int
ptree_Comp_int32( ptree_sTable *tp, ptree_sNode *x, ptree_sNode *y);

#ifdef __cplusplus
}
#endif

#endif
