/** 
 * Proview   $Id: wb_ldh_tree.h,v 1.2 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_ldh_tree_h
#define wb_ldh_tree_h

#ifndef __STDDEF_LOADED
#include <stddef.h>
#endif

typedef struct ldh_s_Tree ldh_sTree;
typedef struct ldh_s_TreeAlloc ldh_sTreeAlloc;
typedef struct ldh_s_TreeNode ldh_sTreeNode;

typedef enum {
  ldh_eTreeComp__ = 0,
  ldh_eTreeComp_Memcmp,
  ldh_eTreeComp_Int32,
  ldh_eTreeComp_UInt32,
  ldh_eTreeComp_Objid,
  ldh_eTreeComp_ObjectIx,
  ldh_eTreeComp_Strncmp,
  ldh_eTreeComp_ClassId,
  ldh_eTreeComp_TypeId,
  ldh_eTreeComp_VolumeId,
  ldh_eTreeComp_
} ldh_eTreeComp;

struct ldh_s_TreeAlloc {
  ldh_sTreeAlloc *Next;
};

struct ldh_s_TreeNode {
  ldh_sTreeNode *Parent;
  ldh_sTreeNode *Left;
  ldh_sTreeNode *Right;
  long Bal;
};

struct ldh_s_Tree {
  ldh_sTreeNode *Root;
  ldh_sTreeNode *Null;
  ldh_sTreeNode *Key;
  ldh_sTreeNode *Free;
  ldh_sTreeAlloc *FirstAlloc;
  ptrdiff_t KeyOffset;
  size_t KeySize;
  size_t RecordSize;
  ldh_eTreeComp TreeComp;
  int AllocCount;
  char *(*PrintKey)();
  int nNode;
  int nError;
  int nHZ;
  int nHN;
  int nHP;
  int MaxDepth;
  int nLL;
  int nLR;
  int nRL;
  int nRR;
  int nMalloc;
  int nAlloc;
  int nFree;
  int nInsert;
  int nDelete;
  int nSearch;
};

/*
  Function prototypes to exported functions.
*/

ldh_sTree *
ldh_TreeCreate (
  size_t KeySize,
  ptrdiff_t KeyOffset,
  size_t RecordSize,
  unsigned long AllocCount,
  ldh_eTreeComp TreeComp,
  char *(*PrintKey)()
);

void
ldh_TreeDelete (
  ldh_sTree *tp
);

ldh_sTreeNode *
ldh_TreeFind (
  ldh_sTree *tp,
  void *Key
);

int
ldh_TreeIsEmpty (
  ldh_sTree *tp
);

void
ldh_TreeEmpty (
  ldh_sTree *tp
);

ldh_sTreeNode *
ldh_TreeInsert (
  ldh_sTree *tp,
  void *Key
);

ldh_sTreeNode *
ldh_TreeMaximum (
  ldh_sTree *tp
);

ldh_sTreeNode *
ldh_TreeMinimum (
  ldh_sTree *tp
);

ldh_sTreeNode *
ldh_TreePredecessor (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

void
ldh_TreePrint (
  ldh_sTree *tp
);

void
ldh_TreeRemove (
  ldh_sTree *tp,
  void *Key
);

ldh_sTreeNode *
ldh_TreeSuccessor (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

#endif
