#include <stdio.h>
#include <stdlib.h>
#include <stsdef.h>
#include <time.h>
#include "co_tree.h"

typedef struct {
  tree_sNode		node;
  unsigned int	id;
  unsigned int	cnt;
} sTest;    

static char *
printKey (
  tree_sNode  *node
)
{
  static char string[80];
  sTest *n = (sTest *) node;  

  sprintf(string, "%d", n->id);
  return string;
}

static void
printNode (
  tree_sNode  *node
)
{
  static char string[80];
  sTest *n = (sTest *) node;  

  printf("Key: %d, count: %d\n", n->id, n->cnt);
}

static int
compare (
  void *x,
  void *y
)
{
  unsigned int ix = *(unsigned int *) x; 
  unsigned int iy = *(unsigned int *) y;

  ix %= 100;
  iy %= 100;

  if (ix < iy)
    return -1;
  else if (ix == iy)
    return 0;
  else
    return 1;
}

main ()
{

  tree_sTable		*test, *clone;
  sTest			*Node, *Node1;
  int			i;
  clock_t		kl;

  test = tree_CreateTable(sizeof(unsigned int),
    offsetof(sTest, id), sizeof(sTest), 100,
    tree_eComp_user, compare);

  for (i=10000; i>0; i--) {
    kl = i;
    Node = (sTest *) tree_Find(test, &kl);
    if (Node == NULL)
      Node = (sTest *) tree_Insert(test, &kl);
    Node->cnt++;
  }

  tree_PrintTable(test, printNode, printKey);
  clone = tree_CloneTable(test);
  tree_PrintTable(clone, printNode, printKey);

}
