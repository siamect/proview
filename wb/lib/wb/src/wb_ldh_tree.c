/* wb_ldh_tree.c -- <short description>

   PROVIEW/R
   Copyright (C) 1994-1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Intrinsic.h>
#include <assert.h>
#include "pwr.h"
#include "wb_ldh_tree.h"

static ldh_sTreeNode *
AllocNode (
  ldh_sTree *tp,
  void *Key
);

static void
TreePrintInorder (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

static long
CompareKey (
  ldh_sTree *tp,
  ldh_sTreeNode *x,
  ldh_sTreeNode *y
);

static ldh_sTree *
CreateTree (
  size_t KeySize,
  ptrdiff_t KeyOffset,
  size_t RecordSize,
  unsigned long AllocCount,
  ldh_eTreeComp TreeComp,
  char *PrintKey()
);

static void
DeleteTree (
  ldh_sTree *tp
);

static void
EmptyTree (
  ldh_sTree *tp
);

static void
FreeNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

static ldh_sTreeNode *
FindNode (
  ldh_sTree *tp,
  void *Key
);

static ldh_sTreeNode *
MinimumNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

static ldh_sTreeNode *
MaximumNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

static ldh_sTreeNode *
SuccessorNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

static ldh_sTreeNode *
PredecessorNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
);

static ldh_sTreeNode *
DeleteNode (
  ldh_sTree *tp,
  ldh_sTreeNode *z
);

static ldh_sTreeNode *
InsertNode (
  ldh_sTree *tp,
  ldh_sTreeNode *z
);

static void
TreePrint (
  ldh_sTree *tp,
  ldh_sTreeNode *np,
  ldh_sTreeNode *op,
  ldh_sTreeNode *oop,
  int level
);

static void
TreeCheck (
  ldh_sTree *tp,
  ldh_sTreeNode *np,
  int *count,
  int *maxlevel,
  int *hight,
  int level
);



static ldh_sTreeNode *
AllocNode (
  ldh_sTree *tp,
  void *Key
)
{
  ldh_sTreeNode *np;
  ldh_sTreeNode *fp;
  ldh_sTreeNode *op;
  ldh_sTreeAlloc *ap;
  int i;

  if ((np = tp->Free) == NULL) {
    tp->nMalloc++;
    ap = (ldh_sTreeAlloc *) XtCalloc(1, tp->AllocCount * tp->RecordSize +
      sizeof(ldh_sTreeAlloc));
    ap->Next = tp->FirstAlloc;
    tp->FirstAlloc = ap;
    np = fp = (ldh_sTreeNode *) (ap + 1);
    if (fp == NULL)
      return fp;
    fp = (ldh_sTreeNode *) (tp->RecordSize + (char *) fp);
    tp->Free = fp;
    tp->nFree += tp->AllocCount;
    for (i = 1, op = tp->Free; i < tp->AllocCount - 1; i++) {
      fp = (ldh_sTreeNode *) (tp->RecordSize + (char *) fp);
      op->Right = fp;
      op = fp;
    }
  } else {
    tp->Free = np->Right;
  }
  tp->nAlloc++;
  tp->nFree--;
  np->Left = np->Right = np->Parent = tp->Null;
  if (Key != NULL)
    memcpy(tp->KeyOffset + (char *) np,  Key, tp->KeySize);
  return np;
}  


static void
TreePrintInorder (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{
  
  if (np->Right != tp->Null) TreePrintInorder(tp, np->Right);  

  printf(" %s\n", tp->PrintKey(np));

  if (np->Left != tp->Null) TreePrintInorder(tp, np->Left);  
}  


static long
CompareKey (
  ldh_sTree *tp,
  ldh_sTreeNode *x,
  ldh_sTreeNode *y
)
{

  switch (tp->TreeComp) {
  case ldh_eTreeComp_ClassId:
  case ldh_eTreeComp_TypeId:
  case ldh_eTreeComp_VolumeId:
  case ldh_eTreeComp_UInt32:
  case ldh_eTreeComp_ObjectIx:
    {
      pwr_tUInt32 *xKey = (pwr_tUInt32 *) (tp->KeyOffset + (char *) x);
      pwr_tUInt32 *yKey = (pwr_tUInt32 *) (tp->KeyOffset + (char *) y);

      if (*xKey < *yKey)
	return -1;
      else if (*xKey == *yKey)
	return 0;
      else
	return 1;
    }
    break;
  case ldh_eTreeComp_Objid:
    {
      pwr_tObjid *xKey = (pwr_tObjid *) (tp->KeyOffset + (char *) x);
      pwr_tObjid *yKey = (pwr_tObjid *) (tp->KeyOffset + (char *) y);

      if (xKey->vid == yKey->vid) {
	if (xKey->oix == yKey->oix)
	  return 0;
	else if (xKey->oix < yKey->oix)
	  return -1;
	else
	  return 1;
      } else if (xKey->vid < yKey->vid)
	return -1;
      else
	return 1;
    }
    break;
  case ldh_eTreeComp_Memcmp:
    {
      void *xKey = (void *) (tp->KeyOffset + (char *) x);
      void *yKey = (void *) (tp->KeyOffset + (char *) y);

      return memcmp(xKey, yKey, tp->KeySize);
    }
    break;
  case ldh_eTreeComp_Strncmp:
    {
      char *xKey = (char *) (tp->KeyOffset + (char *) x);
      char *yKey = (char *) (tp->KeyOffset + (char *) y);

      return strncmp(xKey, yKey, tp->KeySize);
    }
    break;
  default:
    printf("ldh_Tree: unknown compare method: %d\n", tp->TreeComp);
    exit(1);
  }

}


static ldh_sTree *
CreateTree (
  size_t KeySize,
  ptrdiff_t KeyOffset,
  size_t RecordSize,
  unsigned long AllocCount,
  ldh_eTreeComp TreeComp,
  char *PrintKey()
)
{
  ldh_sTree *tp;
  
  tp = (ldh_sTree *) XtCalloc(1, sizeof(*tp));
  tp->KeySize = KeySize;
  tp->KeyOffset = KeyOffset;
  tp->RecordSize = RecordSize;
  tp->AllocCount = AllocCount;
  tp->TreeComp = TreeComp;
  tp->PrintKey = PrintKey;
  tp->Null = AllocNode(tp, NULL);
  tp->Key = AllocNode(tp, NULL);
  tp->Root = tp->Null;
  tp->Null->Bal = 0;
  tp->Null->Left = tp->Null;
  tp->Null->Right = tp->Null;
  tp->Null->Parent = tp->Null;

  return tp;
}


static void
DeleteTree (
  ldh_sTree *tp
)
{
  ldh_sTreeAlloc *ap;
  ldh_sTreeAlloc *fap;
  
  if (tp == NULL) return;

  for (ap = tp->FirstAlloc; ap != NULL;) {
    fap = ap;
    ap = ap->Next;
    XtFree((char *) fap);
  }

  XtFree((char *) tp);
}


static void
EmptyTree (
  ldh_sTree *tp
)
{
  ldh_sTreeNode *np;
  ldh_sTreeNode *mp;
  
  if (tp == NULL) return;

  for (mp = MinimumNode(tp, tp->Root); mp != NULL;) {
    if (mp == tp->Null) return;
    np = SuccessorNode(tp, mp);
    mp = DeleteNode(tp, mp);
    FreeNode(tp, mp);
  }
}


static void
FreeNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{

  memset(np, 0 , tp->RecordSize);
  np->Right = tp->Free;
  tp->Free = np;
  tp->nFree++;
  return;
}  


static ldh_sTreeNode *
FindNode (
  ldh_sTree *tp,
  void *Key
)
{
  ldh_sTreeNode *np;
  int comp;

  tp->nSearch++;

  /* use sentinel */
  memcpy(tp->KeyOffset + (char *) tp->Null,  Key, tp->KeySize);

  /* use key node for comparisons */
  memcpy(tp->KeyOffset + (char *) tp->Key,  Key, tp->KeySize);

  for(np = tp->Root;;) {
    comp = CompareKey(tp, tp->Key, np);
    if (comp == 0)
      break;
    else if (comp < 0)
      np = np->Left;
    else
      np = np->Right;
  }

  return np;
}


static ldh_sTreeNode *
MinimumNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{

  for (; np->Left != tp->Null; np = np->Left)
    ;
  return np;
}


static ldh_sTreeNode *
MaximumNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{

  for (; np->Right != tp->Null; np = np->Right)
    ;
  return np;
}


static ldh_sTreeNode *
SuccessorNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{
  ldh_sTreeNode *p;
  
  if (np->Right != tp->Null)
    return MinimumNode(tp, np->Right);

  for (
    p = np->Parent;
    p != tp->Null && np == p->Right;
    np = p, p = p->Parent
  )
    ;
  return p;
}


static ldh_sTreeNode *
PredecessorNode (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{
  ldh_sTreeNode *p;
  
  if (np->Left != tp->Null)
    return MaximumNode(tp, np->Left);

  for (
    p = np->Parent;
    p != tp->Null && np == p->Left;
    np = p, p = p->Parent
  )
    ;
  return p;
}


static ldh_sTreeNode *
DeleteNode (
  ldh_sTree *tp,
  ldh_sTreeNode *z
)
{
  ldh_sTreeNode *p, *x, *y, *p1, *p2;
  int h, b1, b2;
  
  tp->nDelete++;
  tp->nNode--;

  if (z->Left == tp->Null || z->Right == tp->Null)
    y = z;
  else
    y = SuccessorNode(tp, z);

  if (y->Left != tp->Null)
    x = y->Left;
  else
    x = y->Right;

  x->Parent = y->Parent;
  if (y->Parent == tp->Null) {
    tp->Root = x;
    h = 0;
  } else if (y == y->Parent->Left) {
    y->Parent->Left = x;
    h = 1; /* left branch has shrunk */
  } else {
    y->Parent->Right = x;
    h = -1;  /* right branch has shrunk */
  }

  if (y->Parent == z)
    p = y;
  else
    p = y->Parent;

  if (z != y) { /* Replace z with y */
    y->Bal = z->Bal;
    y->Parent = z->Parent;
    if (z->Parent == tp->Null) {
      tp->Root = y;
    } else if (z == z->Parent->Left) {
      z->Parent->Left = y;
    } else {
      z->Parent->Right = y;
    }
    y->Left = z->Left;
    if (z->Left != tp->Null)
      z->Left->Parent = y;
    y->Right = z->Right;
    if (z->Right != tp->Null)
      z->Right->Parent = y;
  }

  for (; p != tp->Null && h != 0; p = p->Parent) {
    if (h == -1) { /* right branch has shrunk */
      switch (p->Bal) {
      case 1:
	p->Bal = 0;
	break;
      case 0:
	p->Bal = -1;
	h = 0;
	break;
      case -1:
	p1 = p->Left;
	b1 = p1->Bal;
	if (b1 < 1) { /* single LL rotation */
	  tp->nLL++;
	  p->Left = p1->Right;
	  p1->Right->Parent = p;
	  p1->Right = p;
	  if (b1 == 0) {
	    p->Bal = -1;
	    p1->Bal = 1;
	    h = 0;
	  } else {
	    p->Bal = p1->Bal = 0;
	  }
	  p1->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p1;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p1;
	  else
	    p->Parent->Right = p1;
	  p->Parent = p1;
	  p = p1;
	} else { /* double LR rotation */
	  tp->nLR++;
	  p2 = p1->Right;
	  b2 = p2->Bal;
	  p1->Right = p2->Left;
	  p2->Left->Parent = p1;
	  p2->Left = p1;
	  p->Left = p2->Right;
	  p2->Right->Parent = p;
	  p2->Right = p;
	  p2->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p2;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p2;
	  else
	    p->Parent->Right = p2;
	  p->Parent = p1->Parent = p2;
	  if (b2 == -1)
	    p->Bal = 1;
	  else
	    p->Bal = 0;
	  if (b2 == 1)
	    p1->Bal = -1;
	  else
	    p1->Bal = 0;
	  p2->Bal = 0;
	  p = p2;
	}
	break;
      }
    } else { /* left branch has grown */
      switch (p->Bal) {
      case -1:
	p->Bal = 0;
	break;
      case 0:
	p->Bal = 1;
	h = 0;
	break;
      case 1:
	p1 = p->Right;
	b1 = p1->Bal;
	if (b1 > -1) { /* single RR rotation */
	  tp->nRR++;
	  p->Right = p1->Left;
	  p1->Left->Parent = p;
	  p1->Left = p;
	  if (b1 == 0) {
	    p->Bal = 1;
	    p1->Bal = -1;
	    h = 0;
	  } else {
	    p->Bal = p1->Bal = 0;
	  }
	  p1->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p1;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p1;
	  else
	    p->Parent->Right = p1;
	  p->Parent = p1;
	  p = p1;
	} else { /* double RL rotation */
	  tp->nRL++;
	  p2 = p1->Left;
	  b2 = p2->Bal;
	  p1->Left = p2->Right;
	  p2->Right->Parent = p1;
	  p2->Right = p1;
	  p->Right = p2->Left;
	  p2->Left->Parent = p;
	  p2->Left = p;
	  p2->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p2;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p2;
	  else
	    p->Parent->Right = p2;
	  p->Parent = p1->Parent = p2;
	  if (b2 == 1)
	    p->Bal = -1;
	  else
	    p->Bal = 0;
	  if (b2 == -1)
	    p1->Bal = 1;
	  else
	    p1->Bal = 0;
	  p2->Bal = 0;
	  p = p2;
	}
	break;
      }
    }
    if (h!= 0 && p->Parent != tp->Null && p == p->Parent->Left)
      h = 1; /* left branch has shrunk */
    else if (h!= 0 && p->Parent != tp->Null && p == p->Parent->Right)
      h = -1;
  }    

  return z;
}


static ldh_sTreeNode *
InsertNode (
  ldh_sTree *tp,
  ldh_sTreeNode *z
)
{
  ldh_sTreeNode *p, *p1, *p2, *y, *x;
  int comp;
  int h;
  
  for (y = tp->Null, x = tp->Root; x != tp->Null; ) {
    y = x;
    comp = CompareKey(tp, z, x);
    if (comp == 0)
      return x;  /* already exists */
    if (comp < 0) {
      x = x->Left;
    } else {
      x = x->Right;
    }
  }
  z->Parent = y;
  if (y == tp->Null) {
    tp->Root = z;
  } else if (comp < 0) {
    y->Left = z;
  } else {
    y->Right = z;
  }

  z->Bal = 0;

  tp->nNode++;
  tp->nInsert++;

  for (h = 1, y = z, p = z->Parent; p != tp->Null && h != 0; y = p, p = p->Parent) {
    if (y == p->Left) { /* left branch has grown */
      switch (p->Bal) {
      case 1:
	p->Bal = 0;
	h = 0;
	break;
      case 0:
	p->Bal = -1;
	break;
      case -1:
	p1 = p->Left;
	if (p1->Bal == -1) { /* single LL rotation */
	  tp->nLL++;
	  p->Left = p1->Right;
	  p1->Right->Parent = p;
	  p1->Right = p;
	  p1->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p1;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p1;
	  else
	    p->Parent->Right = p1;
	  p->Parent = p1;
	  p->Bal = p1->Bal = 0;
	  p = p1;
	  h = 0;
	} else { /* double LR rotation */
	  tp->nLR++;
	  p2 = p1->Right;
	  p1->Right = p2->Left;
	  p2->Left->Parent = p1;
	  p2->Left = p1;
	  p->Left = p2->Right;
	  p2->Right->Parent = p;
	  p2->Right = p;
	  p2->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p2;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p2;
	  else
	    p->Parent->Right = p2;
	  p->Parent = p1->Parent = p2;
	  if (p2->Bal == -1)
	    p->Bal = 1;
	  else
	    p->Bal = 0;
	  if (p2->Bal == 1)
	    p1->Bal = -1;
	  else
	    p1->Bal = 0;
	  p2->Bal = 0;
	  p = p2;
	  h = 0;
	}
	break;
      }
    } else { /* right branch has grown */
      switch (p->Bal) {
      case -1:
	p->Bal = 0;
	h = 0;
	break;
      case 0:
	p->Bal = 1;
	break;
      case 1:
	p1 = p->Right;
	if (p1->Bal == 1) { /* single RR rotation */
	  tp->nRR++;
	  p->Right = p1->Left;
	  p1->Left->Parent = p;
	  p1->Left = p;
	  p1->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p1;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p1;
	  else
	    p->Parent->Right = p1;
	  p->Parent = p1;
	  p->Bal = p1->Bal = 0;
	  p = p1;
	  h = 0;
	} else { /* double RL rotation */
	  tp->nRL++;
	  p2 = p1->Left;
	  p1->Left = p2->Right;
	  p2->Right->Parent = p1;
	  p2->Right = p1;
	  p->Right = p2->Left;
	  p2->Left->Parent = p;
	  p2->Left = p;
	  p2->Parent = p->Parent;
	  if (p->Parent == tp->Null)
	    tp->Root = p2;
	  else if (p == p->Parent->Left)
	    p->Parent->Left = p2;
	  else
	    p->Parent->Right = p2;
	  p->Parent = p1->Parent = p2;
	  if (p2->Bal == 1)
	    p->Bal = -1;
	  else
	    p->Bal = 0;
	  if (p2->Bal == -1)
	    p1->Bal = 1;
	  else
	    p1->Bal = 0;
	  p2->Bal = 0;
	  p = p2;
	  h = 0;
	}
	break;
      }
    }
  }    

  return z;
}



static void
TreePrint (
  ldh_sTree *tp,
  ldh_sTreeNode *np,
  ldh_sTreeNode *op,
  ldh_sTreeNode *oop,
  int level
)
{

  if (np == tp->Null)
    return;

  if (level > tp->MaxDepth) tp->MaxDepth = level;

  TreePrint(tp, np->Left, np, np->Parent, level+1);  

  printf("%5.5d %3.3lu %s", level, np->Bal, tp->PrintKey(np));

  switch (np->Bal) {
  case 1: tp->nHP++; break;
  case 0: tp->nHZ++; break;
  case -1: tp->nHN++; break;
  }

  TreePrint(tp, np->Right, np, np->Parent, level+1);  
}  


static void
TreeCheck (
  ldh_sTree *tp,
  ldh_sTreeNode *np,
  int *count,
  int *maxlevel,
  int *hight,
  int level
)
{
  int comp;
  int hleft, hright;

  if (np == tp->Null) {
    *hight = 0;
    return;
  }

  if (level > *maxlevel) *maxlevel = level;

  TreeCheck(tp, np->Left, count, maxlevel, &hleft, level+1);  
  if (np->Left != tp->Null) {
    if (np->Left->Parent != np) {
      printf("LeftLinkerror: Node key: %s not linked to parent key: %s\n",
	tp->PrintKey(np->Left), tp->PrintKey(np));
    }
    comp = CompareKey(tp, np, np->Left);
    if (comp < 1) {
      printf("LeftLink sort error: Node key: %s not less than key: %s\n",
	tp->PrintKey(np->Left), tp->PrintKey(np));
    }
  }

  (*count)++;
  TreeCheck(tp, np->Right, count, maxlevel, &hright, level+1); 
  if (np->Right != tp->Null) {
    if (np->Right->Parent != np) {
      printf("RightLinkerror: Node key: %s not linked to parent key: %s\n",
	tp->PrintKey(np->Right), tp->PrintKey(np));
    }
    comp = CompareKey(tp, np, np->Right);
    if (comp > -1) {
      printf("RightLink sort error: Node key: %s not greater than key: %s\n",
	tp->PrintKey(np->Right), tp->PrintKey(np));
    }
  }

  if ((hright - hleft) != np->Bal) {
    printf("Balerror key: %s, level: %d, hr: %d, hl: %d, bal: %lu\n",
      tp->PrintKey(np), level, hright, hleft, np->Bal);
  }
  *hight = (hright > hleft ? hright : hleft) + 1;
}



#if 0
static ldh_sTree *
BuildTree (
  ldh_sTree *tp,
  ldh_sTreeNode *fp,
  int count,
  int *hight,
  int *key
)
{
  int hleft, hright;
  ldh_sTreeNode *np;

  if (count == 0) {
    return tp->Null;
  }

  np = AllocNode(tp, 0);
  np->Parent = fp;
  np->Left = BuildTree(tp, np, count/2, &hleft, key);
  np->Key = (*key)++;
  np->Right = BuildTree(tp, np, count - count/2 - 1, &hright, key);
  np->Bal = hright - hleft;

  return np;
}
#endif

ldh_sTree *
ldh_TreeCreate (
  size_t KeySize,
  ptrdiff_t KeyOffset,
  size_t RecordSize,
  unsigned long AllocCount,
  ldh_eTreeComp TreeComp,
  char *(*PrintKey)()
)
{
  
  return CreateTree(KeySize, KeyOffset, RecordSize, AllocCount,
    TreeComp, PrintKey);
}


void
ldh_TreeDelete (
  ldh_sTree *tp
)
{
  
  DeleteTree(tp);
}


void
ldh_TreeEmpty (
  ldh_sTree *tp
)
{

  EmptyTree(tp);
}

ldh_sTreeNode *
ldh_TreeInsert (
  ldh_sTree *tp,
  void *Key
)
{
  ldh_sTreeNode *np;
  ldh_sTreeNode *op;

  np = FindNode(tp, Key);
  if (np != tp->Null)
    return (ldh_sTreeNode *) 1;	/* Duplicate key.  */

  np = AllocNode(tp, Key);
  if (np == NULL) return NULL;
  op = InsertNode(tp, np);
  if (np == op) {
    return np;
  } else {
    FreeNode(tp, np);
    return NULL;
  }
}

ldh_sTreeNode *
ldh_TreeFind (
  ldh_sTree *tp,
  void *Key
)
{
  ldh_sTreeNode *np;

  np = FindNode(tp, Key);
  if (np == tp->Null) {
    return NULL;
  } else {
    return np;
  }
}

int
ldh_TreeIsEmpty (
  ldh_sTree *tp
)
{
  return tp->nNode == 0;
}

ldh_sTreeNode *
ldh_TreeMaximum (
  ldh_sTree *tp
)
{
  ldh_sTreeNode *np;

  np = MaximumNode(tp, tp->Root);
  if (np == tp->Null) {
    return NULL;
  } else {
    return np;
  }
}


ldh_sTreeNode *
ldh_TreeMinimum (
  ldh_sTree *tp
)
{
  ldh_sTreeNode *np;

  np = MinimumNode(tp, tp->Root);
  if (np == tp->Null) {
    return NULL;
  } else {
    return np;
  }
}


ldh_sTreeNode *
ldh_TreePredecessor (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{

  np = PredecessorNode(tp, np);
  if (np == tp->Null) {
    return NULL;
  } else {
    return np;
  }
}


void
ldh_TreePrint (
  ldh_sTree *tp
)
{
  int count = 0;
  int maxlevel = 0;
  int hight = 0;

#if 0
  TreePrintInorder(tp, tp->Root);
#endif
  TreeCheck(tp, tp->Root, &count, &maxlevel, &hight, 0);
  printf("count: %d, maxlevel: %d, hight: %d\n", count, maxlevel, hight);
  printf("nNode.......: %d\n", tp->nNode);
  printf("nError......: %d\n", tp->nError);
  printf("nHZ.........: %d\n", tp->nHZ);
  printf("nHN.........: %d\n", tp->nHN);
  printf("nHP.........: %d\n", tp->nHP);
  printf("MaxDepth....: %d\n", tp->MaxDepth);
  printf("nLL.........: %d\n", tp->nLL);
  printf("nLR.........: %d\n", tp->nLR);
  printf("nRL.........: %d\n", tp->nRL);
  printf("nRR.........: %d\n", tp->nRR);
  printf("nMalloc.....: %d\n", tp->nMalloc);
  printf("nAlloc......: %d\n", tp->nAlloc);
  printf("nFree.......: %d\n", tp->nFree);
  printf("nInsert.....: %d\n", tp->nInsert);
  printf("nDelete.....: %d\n", tp->nDelete);
  printf("nSearch.....: %d\n", tp->nSearch);
}



void
ldh_TreeRemove (
  ldh_sTree *tp,
  void *Key
)
{
  ldh_sTreeNode *np;

  np = FindNode(tp, Key);
  if (np != tp->Null) {
    np = DeleteNode(tp, np);
    FreeNode(tp, np);
  }
  return;
}


ldh_sTreeNode *
ldh_TreeSuccessor (
  ldh_sTree *tp,
  ldh_sTreeNode *np
)
{

  np = SuccessorNode(tp, np);
  if (np == tp->Null) {
    return NULL;
  } else {
    return np;
  }
}

