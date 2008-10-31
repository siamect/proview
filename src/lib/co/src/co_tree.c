/** 
 * Proview   $Id: co_tree.c,v 1.5 2008-10-31 12:51:30 claes Exp $
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pwr.h"
#include "co_tree.h"


static tree_sNode *
allocNode(tree_sTable *tp, void *key);

static void
treePrintInorder(tree_sTable *tp, tree_sNode *np, void (*printNode)(tree_sNode *));


static tree_sTable *
cloneTree(pwr_tStatus *sts, tree_sTable *tp);

static tree_sNode *
copyNodes(tree_sTable *ntp, tree_sNode *parent, tree_sTable *otp, tree_sNode *onp);

static tree_sTable *
createTree(
    pwr_tStatus  *sts,
    size_t       keySize,
    ptrdiff_t    keyOffset,
    size_t       recordSize,
    unsigned int allocCount,
    int          (*compareFunc) (tree_sTable *tp, tree_sNode *x, tree_sNode *y)
);

static void
deleteTree(pwr_tStatus *sts, tree_sTable *tp);

static void
emptyTree(pwr_tStatus *sts, tree_sTable *tp);

static void
freeNode(tree_sTable *tp, tree_sNode *np);

static tree_sNode *
findNearNode(tree_sTable *tp, void *key);

static tree_sNode *
findNode(tree_sTable *tp, void *key);

static tree_sNode *
minimumNode(tree_sTable *tp, tree_sNode *np);

static tree_sNode *
maximumNode(tree_sTable *tp, tree_sNode *np);

static tree_sNode *
successorNode(tree_sTable *tp, tree_sNode *np);

static tree_sNode *
predecessorNode(tree_sTable *tp, tree_sNode *np);

static tree_sNode *
deleteNode(tree_sTable *tp, tree_sNode *z);

static tree_sNode *
insertNode(tree_sTable *tp, tree_sNode *z);

#if 0
static void
treePrint(tree_sTable *tp, tree_sNode *np, tree_sNode *op, tree_sNode *oop,
          int level, void (*printNode)(tree_sNode *, int));
#endif

static void
treeCheck(tree_sTable *tp, tree_sNode *np, int *count, int *maxlevel, int *hight,
          int level, char *(*printKey)(tree_sNode *));



static tree_sNode *
allocNode(tree_sTable *tp, void *key)
{
    tree_sNode  *np;
    tree_sNode  *fp;
    tree_sNode  *op;
    tree_sAlloc *ap;
    int         i;

    if ((np = tp->free) == NULL) {
        tp->nMalloc++;
        ap = (tree_sAlloc *) calloc(1, tp->allocCount * tp->recordSize +
                                            sizeof(tree_sAlloc));
        ap->next = tp->firstAlloc;
        tp->firstAlloc = ap;
        np = fp = (tree_sNode *) (ap + 1);
        if (fp == NULL)
            return fp;
        fp = (tree_sNode *) (tp->recordSize + (char *) fp);
        tp->free = fp;
        tp->nFree += tp->allocCount;
        for (i = 1, op = tp->free; i < tp->allocCount - 1; i++) {
            fp = (tree_sNode *) (tp->recordSize + (char *) fp);
            op->right = fp;
            op = fp;
        }
    } else {
        tp->free = np->right;
    }
    tp->nAlloc++;
    tp->nFree--;
    np->left = np->right = np->parent = tp->null;
    if (key != NULL)
        memcpy(tp->keyOffset + (char *) np,  key, tp->keySize);
    return np;
}  


static void
treePrintInorder(tree_sTable *tp, tree_sNode *np, void (*printNode)(tree_sNode *))
{
  
    if (np->right != tp->null)
        treePrintInorder(tp, np->right, printNode);

    printNode(np);

    if (np->left != tp->null)
        treePrintInorder(tp, np->left, printNode);
}  




static tree_sTable *
cloneTree(pwr_tStatus *sts, tree_sTable *otp)
{
    tree_sTable		*ntp;
  
    ntp = (tree_sTable *) calloc(1, sizeof(*ntp));

    if (ntp == NULL)
        pwr_Return(NULL, sts, TREE__INSVIRMEM);

    ntp->keySize      = otp->keySize;
    ntp->keyOffset    = otp->keyOffset;
    ntp->recordSize   = otp->recordSize;
    ntp->allocCount   = otp->allocCount;
    ntp->null         = allocNode(ntp, NULL);
    ntp->compareFunc  = otp->compareFunc;
    ntp->key          = allocNode(ntp, NULL);
    ntp->null->bal    = 0;
    ntp->null->left   = ntp->null;
    ntp->null->right  = ntp->null;
    ntp->null->parent = ntp->null;

    ntp->root = copyNodes(ntp, ntp->null, otp, otp->root);
    ntp->nNode = otp->nNode;

    return ntp;
}


static tree_sNode *
copyNodes(tree_sTable *ntp, tree_sNode *parent, tree_sTable *otp, tree_sNode *onp)
{
    tree_sNode	*nnp;

    if (onp == otp->null)
        return ntp->null;

    nnp = allocNode(ntp, NULL);
    memcpy(nnp, onp, ntp->recordSize);

    nnp->parent = parent;
    nnp->left  = copyNodes(ntp, nnp, otp, onp->left);
    nnp->right = copyNodes(ntp, nnp, otp, onp->right);

    return nnp;
}


static tree_sTable *
createTree(
    pwr_tStatus  *sts,
    size_t       keySize,
    ptrdiff_t    keyOffset,
    size_t       recordSize,
    unsigned int allocCount,
    int          (*compareFunc) (tree_sTable *tp, tree_sNode *x, tree_sNode *y))
{
    tree_sTable *tp;
  
    tp = (tree_sTable *) calloc(1, sizeof(*tp));

    if (tp == NULL)
        pwr_Return(NULL, sts, TREE__INSVIRMEM);

    tp->keySize     = keySize;
    tp->keyOffset   = keyOffset;
    tp->recordSize  = recordSize;
    tp->allocCount  = allocCount;
    tp->compareFunc = compareFunc;
    tp->null        = allocNode(tp, NULL);
    tp->key         = allocNode(tp, NULL);
    tp->root        = tp->null;

    tp->null->bal    = 0;
    tp->null->left   = tp->null;
    tp->null->right  = tp->null;
    tp->null->parent = tp->null;

    return tp;
}


static void
deleteTree(pwr_tStatus *sts, tree_sTable *tp)
{
    tree_sAlloc *ap;
    tree_sAlloc *fap;
  
    if (tp == NULL) return;

    for (ap = tp->firstAlloc; ap != NULL;) {
        fap = ap;
        ap = ap->next;
        free((char *) fap);
    }

    free((char *) tp);
}


static void
emptyTree(pwr_tStatus *sts, tree_sTable *tp)
{
    tree_sNode *np;
    tree_sNode *mp;
  
    if (tp == NULL) return;

    for (mp = minimumNode(tp, tp->root); mp != NULL;) {
        if (mp == tp->null) return;
        np = successorNode(tp, mp);
        mp = deleteNode(tp, mp);
        freeNode(tp, mp);
	mp = np;
    }
}


static void
freeNode(tree_sTable *tp, tree_sNode *np)
{

    memset(np, 0 , tp->recordSize);
    np->right = tp->free;
    tp->free = np;
    tp->nFree++;
    return;
}  


static tree_sNode *
findNearNode(tree_sTable *tp, void *key)
{
    tree_sNode *np;
    int        comp;

    tp->nSearch++;

    /* use key node for comparisons */
    memcpy(tp->keyOffset + (char *) tp->key, key, tp->keySize);

    for (np = tp->last = tp->root; np != tp->null; tp->lastComp = comp) {
        tp->last = np;
        comp = tp->compareFunc(tp, tp->key, np);
        if (comp == 0)
            break;
        else if (comp < 0)
            np = np->left;
        else
            np = np->right;
    }

    return np;
}


static tree_sNode *
findNode(tree_sTable *tp, void *key)
{
    tree_sNode *np;
    int        comp;

    tp->nSearch++;

        /* use sentinel */
    memcpy(tp->keyOffset + (char *) tp->null,  key, tp->keySize);

        /* use key node for comparisons */
    memcpy(tp->keyOffset + (char *) tp->key,  key, tp->keySize);

    for (np = tp->root;;) {
        comp = tp->compareFunc(tp, tp->key, np);
        if (comp == 0)
            break;
        else if (comp < 0)
            np = np->left;
        else
            np = np->right;
    }

    return np;
}


static tree_sNode *
minimumNode(tree_sTable *tp, tree_sNode *np)
{

    for (; np->left != tp->null; np = np->left)
        ;
    return np;
}


static tree_sNode *
maximumNode(tree_sTable *tp, tree_sNode *np)
{

    for (; np->right != tp->null; np = np->right)
        ;
    return np;
}


static tree_sNode *
successorNode(tree_sTable *tp, tree_sNode *np)
{
    tree_sNode *p;
  
    if (np->right != tp->null)
        return minimumNode(tp, np->right);

    for (p = np->parent; p != tp->null && np == p->right; np = p, p = p->parent)
        ;
    return p;
}


static tree_sNode*
predecessorNode(tree_sTable *tp, tree_sNode *np)
{
    tree_sNode *p;
  
    if (np->left != tp->null)
        return maximumNode(tp, np->left);

    for (p = np->parent; p != tp->null && np == p->left; np = p, p = p->parent)
        ;
    return p;
}


static tree_sNode*
deleteNode(tree_sTable *tp, tree_sNode *z)
{
    tree_sNode *p;
    tree_sNode *x;
    tree_sNode *y;
    tree_sNode *p1;
    tree_sNode *p2;
    int        h;
    int        b1;
    int        b2;
  
    tp->nDelete++;
    tp->nNode--;

    if (z->left == tp->null || z->right == tp->null)
        y = z;
    else
        y = successorNode(tp, z);

    if (y->left != tp->null)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent;
    if (y->parent == tp->null) {
        tp->root = x;
        h = 0;
    } else if (y == y->parent->left) {
        y->parent->left = x;
        h = 1; /* left branch has shrunk */
    } else {
        y->parent->right = x;
        h = -1;  /* right branch has shrunk */
    }

    if (y->parent == z)
        p = y;
    else
        p = y->parent;

    if (z != y) { /* Replace z with y */
        y->bal = z->bal;
        y->parent = z->parent;
        if (z->parent == tp->null) {
            tp->root = y;
        } else if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
        y->left = z->left;
        if (z->left != tp->null)
            z->left->parent = y;
        y->right = z->right;
        if (z->right != tp->null)
            z->right->parent = y;
    }

    for (; p != tp->null && h != 0; p = p->parent) {
        if (h == -1) { /* right branch has shrunk */
            switch (p->bal) {
            case 1:
                p->bal = 0;
                break;
            case 0:
                p->bal = -1;
                h = 0;
                break;
            case -1:
                p1 = p->left;
                b1 = p1->bal;
                if (b1 < 1) { /* single LL rotation */
                    tp->nLL++;
                    p->left = p1->right;
                    p1->right->parent = p;
                    p1->right = p;
                    if (b1 == 0) {
                        p->bal = -1;
                        p1->bal = 1;
                        h = 0;
                    } else {
                        p->bal = p1->bal = 0;
                    }
                    p1->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p1;
                    else if (p == p->parent->left)
                        p->parent->left = p1;
                    else
                        p->parent->right = p1;
                    p->parent = p1;
                    p = p1;
                } else { /* double LR rotation */
                    tp->nLR++;
                    p2 = p1->right;
                    b2 = p2->bal;
                    p1->right = p2->left;
                    p2->left->parent = p1;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right->parent = p;
                    p2->right = p;
                    p2->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p2;
                    else if (p == p->parent->left)
                        p->parent->left = p2;
                    else
                        p->parent->right = p2;
                    p->parent = p1->parent = p2;
                    if (b2 == -1)
                        p->bal = 1;
                    else
                        p->bal = 0;
                    if (b2 == 1)
                        p1->bal = -1;
                    else
                        p1->bal = 0;
                    p2->bal = 0;
                    p = p2;
                }
                break;
            }
        } else { /* left branch has grown */
            switch (p->bal) {
            case -1:
                p->bal = 0;
                break;
            case 0:
                p->bal = 1;
                h = 0;
                break;
            case 1:
                p1 = p->right;
                b1 = p1->bal;
                if (b1 > -1) { /* single RR rotation */
                    tp->nRR++;
                    p->right = p1->left;
                    p1->left->parent = p;
                    p1->left = p;
                    if (b1 == 0) {
                        p->bal = 1;
                        p1->bal = -1;
                        h = 0;
                    } else {
                        p->bal = p1->bal = 0;
                    }
                    p1->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p1;
                    else if (p == p->parent->left)
                        p->parent->left = p1;
                    else
                        p->parent->right = p1;
                    p->parent = p1;
                    p = p1;
                } else { /* double RL rotation */
                    tp->nRL++;
                    p2 = p1->left;
                    b2 = p2->bal;
                    p1->left = p2->right;
                    p2->right->parent = p1;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left->parent = p;
                    p2->left = p;
                    p2->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p2;
                    else if (p == p->parent->left)
                        p->parent->left = p2;
                    else
                        p->parent->right = p2;
                    p->parent = p1->parent = p2;
                    if (b2 == 1)
                        p->bal = -1;
                    else
                        p->bal = 0;
                    if (b2 == -1)
                        p1->bal = 1;
                    else
                        p1->bal = 0;
                    p2->bal = 0;
                    p = p2;
                }
                break;
            }
        }
        if (h!= 0 && p->parent != tp->null && p == p->parent->left)
            h = 1; /* left branch has shrunk */
        else if (h!= 0 && p->parent != tp->null && p == p->parent->right)
            h = -1;
    }    

    return z;
}


static tree_sNode *
insertNode(tree_sTable *tp, tree_sNode *z)
{
    tree_sNode *p;
    tree_sNode *p1;
    tree_sNode *p2;
    tree_sNode *y;
    tree_sNode *x;
    int        comp;
    int        h;
  
    for (y = tp->null, x = tp->root; x != tp->null;) {
        y = x;
        comp = tp->compareFunc(tp, z, x);
        if (comp == 0)
            return x;  /* already exists */
        if (comp < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == tp->null) {
        tp->root = z;
    } else if (comp < 0) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->bal = 0;

    tp->nNode++;
    tp->nInsert++;

    for (h = 1, y = z, p = z->parent; p != tp->null && h != 0; y = p, p = p->parent) {
        if (y == p->left) { /* left branch has grown */
            switch (p->bal) {
            case 1:
                p->bal = 0;
                h = 0;
                break;
            case 0:
                p->bal = -1;
                break;
            case -1:
                p1 = p->left;
                if (p1->bal == -1) { /* single LL rotation */
                    tp->nLL++;
                    p->left = p1->right;
                    p1->right->parent = p;
                    p1->right = p;
                    p1->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p1;
                    else if (p == p->parent->left)
                        p->parent->left = p1;
                    else
                        p->parent->right = p1;
                    p->parent = p1;
                    p->bal = p1->bal = 0;
                    p = p1;
                    h = 0;
                } else { /* double LR rotation */
                    tp->nLR++;
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left->parent = p1;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right->parent = p;
                    p2->right = p;
                    p2->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p2;
                    else if (p == p->parent->left)
                        p->parent->left = p2;
                    else
                        p->parent->right = p2;
                    p->parent = p1->parent = p2;
                    if (p2->bal == -1)
                        p->bal = 1;
                    else
                        p->bal = 0;
                    if (p2->bal == 1)
                        p1->bal = -1;
                    else
                        p1->bal = 0;
                    p2->bal = 0;
                    p = p2;
                    h = 0;
                }
                break;
            }
        } else { /* right branch has grown */
            switch (p->bal) {
            case -1:
                p->bal = 0;
                h = 0;
                break;
            case 0:
                p->bal = 1;
                break;
            case 1:
                p1 = p->right;
                if (p1->bal == 1) { /* single RR rotation */
                    tp->nRR++;
                    p->right = p1->left;
                    p1->left->parent = p;
                    p1->left = p;
                    p1->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p1;
                    else if (p == p->parent->left)
                        p->parent->left = p1;
                    else
                        p->parent->right = p1;
                    p->parent = p1;
                    p->bal = p1->bal = 0;
                    p = p1;
                    h = 0;
                } else { /* double RL rotation */
                    tp->nRL++;
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right->parent = p1;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left->parent = p;
                    p2->left = p;
                    p2->parent = p->parent;
                    if (p->parent == tp->null)
                        tp->root = p2;
                    else if (p == p->parent->left)
                        p->parent->left = p2;
                    else
                        p->parent->right = p2;
                    p->parent = p1->parent = p2;
                    if (p2->bal == 1)
                        p->bal = -1;
                    else
                        p->bal = 0;
                    if (p2->bal == -1)
                        p1->bal = 1;
                    else
                        p1->bal = 0;
                    p2->bal = 0;
                    p = p2;
                    h = 0;
                }
                break;
            }
        }
    }    

    return z;
}


#if 0

static void
treePrint(
    tree_sTable *tp,
    tree_sNode  *np,
    tree_sNode  *op,
    tree_sNode  *oop,
    int         level,
    void        (*printNode)(tree_sNode *, int)
    )
{

    if (np == tp->null)
        return;

    if (level > tp->maxDepth)
        tp->maxDepth = level;

    treePrint(tp, np->left, np, np->parent, level+1, printNode);  

    printNode(np, level);

    switch (np->bal) {
    case 1:
        tp->nHP++;
        break;
    case 0:
        tp->nHZ++;
        break;
    case -1:
        tp->nHN++;
        break;
    }

    treePrint(tp, np->right, np, np->parent, level+1, printNode);
}  
#endif


static void
treeCheck(tree_sTable *tp, tree_sNode *np, int *count, int *maxlevel,
          int *hight, int level, char *(*printKey)(tree_sNode *))
{
    int comp;
    int hleft;
    int hright;

    if (np == tp->null) {
        *hight = 0;
        return;
    }

    if (level > *maxlevel)
        *maxlevel = level;

    treeCheck(tp, np->left, count, maxlevel, &hleft, level+1, printKey);  
    if (np->left != tp->null) {
        if (np->left->parent != np) {
            printf("leftLinkerror: Node key: %s not linked to parent key: %s\n",
                   printKey(np->left), printKey(np));
        }
        comp = tp->compareFunc(tp, np, np->left);
        if (comp < 1) {
            printf("leftLink sort error: Node key: %s not less than key: %s\n",
                   printKey(np->left), printKey(np));
        }
    }

    (*count)++;
    treeCheck(tp, np->right, count, maxlevel, &hright, level+1, printKey); 
    if (np->right != tp->null) {
        if (np->right->parent != np) {
            printf("rightLinkerror: Node key: %s not linked to parent key: %s\n",
                   printKey(np->right), printKey(np));
        }
        comp = tp->compareFunc(tp, np, np->right);
        if (comp > -1) {
            printf("rightLink sort error: Node key: %s not greater than key: %s\n",
                   printKey(np->right), printKey(np));
        }
    }

    if ((hright - hleft) != np->bal) {
        printf("balerror key: %s, level: %d, hr: %d, hl: %d, bal: %d\n",
               printKey(np), level, hright, hleft, np->bal);
    }
    *hight = (hright > hleft ? hright : hleft) + 1;
}


#if 0
static tree_sTable *
buildTree(tree_sTable *tp, tree_sNode *fp, int count, int *hight, int *key)
{
    int        hleft;
    int        hright;
    tree_sNode *np;

    if (count == 0) {
        return tp->null;
    }

    np = allocNode(tp, 0);
    np->parent = fp;
    np->left = buildTree(tp, np, count/2, &hleft, key);
    np->Key = (*key)++;
    np->right = buildTree(tp, np, count - count/2 - 1, &hright, key);
    np->bal = hright - hleft;

    return np;
}
#endif


int
tree_Cardinality(pwr_tStatus *sts, tree_sTable *tp)
{
    return tp->nNode;
}

tree_sTable *
tree_CloneTable(pwr_tStatus *sts, tree_sTable *tp)
{
    
    return cloneTree(sts, tp);
}

tree_sTable *
tree_CreateTable(
    pwr_tStatus  *sts,
    size_t       keySize,
    ptrdiff_t    keyOffset,
    size_t       recordSize,
    unsigned int allocCount,
    int          (*compareFunc) (tree_sTable *tp, tree_sNode *x, tree_sNode *y))
{
  
    return createTree(sts, keySize, keyOffset, recordSize, allocCount, compareFunc);
}

void
tree_DeleteTable(pwr_tStatus *sts, tree_sTable *tp)
{
  
    deleteTree(sts, tp);
}


void
tree_EmptyTable(pwr_tStatus *sts, tree_sTable *tp)
{

    emptyTree(sts, tp);
}

void *
tree_Insert(pwr_tStatus *sts, tree_sTable *tp, void *key)
{
    tree_sNode *np;
    tree_sNode *op;

    np = findNode(tp, key);
    if (np != tp->null)
        pwr_Return((void *)np, sts, TREE__FOUND);

    np = allocNode(tp, key);
    if (np == NULL) return NULL;
    op = insertNode(tp, np);
    if (np == op) {
        pwr_Return((void *)np, sts, TREE__INSERTED);
    } else {
        freeNode(tp, np);
        pwr_Return(NULL, sts, TREE__ERROR);
    }
}

void *
tree_Find(pwr_tStatus *sts, tree_sTable *tp, void *key)
{
    tree_sNode *np;

    np = findNode(tp, key);
    if (np == tp->null) {
        pwr_Return(NULL, sts, TREE__NOTFOUND);
    } else {
        pwr_Return((void *)np, sts, TREE__FOUND);
    }
}

void *
tree_FindPredecessor(pwr_tStatus *sts, tree_sTable *tp, void *key)
{
    tree_sNode *np;

    np = findNearNode(tp, key);
    if (tp->last == tp->null)
        pwr_Return(NULL, sts, TREE__NOTFOUND);
  
    if (np != tp->null) {
        np = predecessorNode(tp, np);
    } else if (tp->lastComp < 0) {
        np = predecessorNode(tp, tp->last);
    } else {
        np = tp->last;
    }

    if (np == tp->null) {
        pwr_Return(NULL, sts, TREE__NOTFOUND);
    } else {
        pwr_Return((void *)np, sts, TREE__FOUND);
    }

}

void *
tree_FindSuccessor(pwr_tStatus *sts, tree_sTable *tp, void *key)
{
    tree_sNode *np;

    np = findNearNode(tp, key);
    if (tp->last == tp->null)
        pwr_Return(NULL, sts, TREE__NOTFOUND);
  
    if (np != tp->null) {
        np = successorNode(tp, np);
    } else if (tp->lastComp < 0) {
        np = tp->last;
    } else {
        np = successorNode(tp, tp->last);
    }

    if (np == tp->null) {
        pwr_Return(NULL, sts, TREE__NOTFOUND);
    } else {
        pwr_Return((void *)np, sts, TREE__FOUND);
    }

}

int
tree_TableIsEmpty(pwr_tStatus *sts, tree_sTable *tp)
{

    return tp->nNode == 0;
}

void *
tree_Maximum(pwr_tStatus *sts, tree_sTable *tp)
{
    tree_sNode *np;

    np = maximumNode(tp, tp->root);
    if (np == tp->null) {
        pwr_Return(NULL, sts, TREE__NOTFOUND);
    } else {
        pwr_Return((void *)np, sts, TREE__FOUND);
    }
}


void *
tree_Minimum(pwr_tStatus *sts, tree_sTable*tp)
{
    tree_sNode *np;

    np = minimumNode(tp, tp->root);
    if (np == tp->null) {
        pwr_Return(NULL, sts, TREE__NOTFOUND);
    } else {
        pwr_Return((void *)np, sts, TREE__FOUND);
    }
}


void *
tree_Predecessor(pwr_tStatus *sts, tree_sTable *tp, void *np)
{

    np = predecessorNode(tp, np);
    if (np == tp->null) {
        pwr_Return(NULL, sts, TREE__NOTFOUND);
    } else {
        pwr_Return(np, sts, TREE__FOUND);
    }
}


void
tree_PrintTable(pwr_tStatus *sts,
                tree_sTable *tp,
                void (*printNode)(tree_sNode *),
                char *(*printKey)(tree_sNode *))
{
    int count = 0;
    int maxlevel = 0;
    int hight = 0;

#if 1
    treePrintInorder(tp, tp->root, printNode);
#endif
    treeCheck(tp, tp->root, &count, &maxlevel, &hight, 0, printKey);
    printf("count: %d, maxlevel: %d, hight: %d\n", count, maxlevel, hight);
    printf("nNode.......: %d\n", tp->nNode);
    printf("nError......: %d\n", tp->nError);
    printf("nHZ.........: %d\n", tp->nHZ);
    printf("nHN.........: %d\n", tp->nHN);
    printf("nHP.........: %d\n", tp->nHP);
    printf("maxDepth....: %d\n", tp->maxDepth);
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
tree_Remove(pwr_tStatus *sts, tree_sTable *tp, void *key)
{
    tree_sNode *np;

    np = findNode(tp, key);
    if (np != tp->null) {
        np = deleteNode(tp, np);
        freeNode(tp, np);
    }
    return;
}

void *
tree_Successor(pwr_tStatus *sts, tree_sTable *tp, void *np)
{

    np = successorNode(tp, np);
    if (np == tp->null) {
        return NULL;
    } else {
        return np;
    }
}

int
tree_Comp_cid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    pwr_tCid *xKey = (pwr_tCid *) (tp->keyOffset + (char *) x);
    pwr_tCid *yKey = (pwr_tCid *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}

int
tree_Comp_nid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    pwr_tNid *xKey = (pwr_tNid *) (tp->keyOffset + (char *) x);
    pwr_tNid *yKey = (pwr_tNid *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}

int
tree_Comp_vid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    pwr_tVid *xKey = (pwr_tVid *) (tp->keyOffset + (char *) x);
    pwr_tVid *yKey = (pwr_tVid *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}

int
tree_Comp_tid(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    pwr_tTid *xKey = (pwr_tTid *) (tp->keyOffset + (char *) x);
    pwr_tTid *yKey = (pwr_tTid *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}

int
tree_Comp_oix(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    pwr_tOix *xKey = (pwr_tOix *) (tp->keyOffset + (char *) x);
    pwr_tOix *yKey = (pwr_tOix *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}

int
tree_Comp_uint32(tree_sTable *tp, tree_sNode  *x, tree_sNode  *y)
{
    pwr_tUInt32 *xKey = (pwr_tUInt32 *) (tp->keyOffset + (char *) x);
    pwr_tUInt32 *yKey = (pwr_tUInt32 *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}

int
tree_Comp_oid(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
    pwr_tOid *xKey = (pwr_tOid *) (tp->keyOffset + (char *) x);
    pwr_tOid *yKey = (pwr_tOid *) (tp->keyOffset + (char *) y);

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


int
tree_Comp_time(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
    pwr_tTime *xKey = (pwr_tTime *) (tp->keyOffset + (char *) x);
    pwr_tTime *yKey = (pwr_tTime *) (tp->keyOffset + (char *) y);

    if (xKey->tv_sec == yKey->tv_sec) {
        if (xKey->tv_nsec == yKey->tv_nsec)
            return 0;
        else if ((int)xKey->tv_nsec < (int)yKey->tv_nsec)
            return -1;
        else
            return 1;
    } else if ((int)xKey->tv_sec < (int)yKey->tv_sec)
        return -1;
    else
        return 1;
}

int
tree_Comp_memcmp(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
    void *xKey = (void *) (tp->keyOffset + (char *) x);
    void *yKey = (void *) (tp->keyOffset + (char *) y);

    return memcmp(xKey, yKey, tp->keySize);
}


int
tree_Comp_strncmp(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
    char *xKey = (char *) (tp->keyOffset + (char *) x);
    char *yKey = (char *) (tp->keyOffset + (char *) y);

    return strncmp(xKey, yKey, tp->keySize);
}


int
tree_Comp_int32(tree_sTable *tp, tree_sNode *x, tree_sNode *y)
{
    pwr_tInt32 *xKey = (pwr_tInt32 *) (tp->keyOffset + (char *) x);
    pwr_tInt32 *yKey = (pwr_tInt32 *) (tp->keyOffset + (char *) y);

    if (*xKey < *yKey)
        return -1;
    else if (*xKey == *yKey)
        return 0;
    else
        return 1;
}
