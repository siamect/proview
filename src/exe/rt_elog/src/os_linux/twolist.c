/* 
 * Proview   $Id: twolist.c,v 1.2 2005-09-01 14:57:48 claes Exp $
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
 */

/* Implementation av tvåvägslistrutiner -- twolist.c */

#include "twolist.h"
#include <stdlib.h>

void newhead(headtyp **hpp)
/* Skapar en ny tom lista */
{
   *hpp = malloc(sizeof(headtyp));

   (*hpp)->kind = head;
   (*hpp)->befo = (*hpp)->next = *hpp;
}


void newlink(linktyp **lpp)
/* Skapar en ny tom länk */
{
   *lpp = malloc(sizeof(linktyp));
   (*lpp)->kind = lank;
   (*lpp)->befo = (*lpp)->next = NULL;
}


void putlink(datatyp d, linktyp *lp)
/* Sätter in data i en länk */
{
   if (lp != NULL)
      lp->data = d;
}

datatyp getlink(linktyp *lp)
/* Returnerar data från en länk */
{
   datatyp d;

   if (lp != NULL)
      d = lp->data;
   return d;
}

void inlast(linktyp *lp, headtyp *hp)
/* Sätter in länk sist i lista */
{
   hp->befo->next = lp;
   lp->befo = hp->befo;
   hp->befo = lp;
   lp->next = hp;
}

void infirst(linktyp *lp, headtyp *hp)
/* Sätter in länk först i lista */
{
   hp->next->befo = lp;
   lp->next = hp->next;
   hp->next = lp;
   lp->befo = hp;
}

void inpred(linktyp *lp, linktyp *ep)
/* Sätter in första länken före den andra */
{
  ep->befo->next = lp;
  lp->befo = ep->befo;
  ep->befo = lp;
  lp->next = ep;
}

void insucc(linktyp *lp, linktyp *ep)
/* Sätter in första länken efter den andra */
{
   ep->next->befo = lp;
   lp->next = ep->next;
   ep->next = lp;
   lp->befo = ep;
}

void insort(linktyp *lp, headtyp *hp, int (*is_less)(datatyp d1, datatyp d2))
/* Sätter in länken sorterad enligt is_less */
{
   linktyp *sp, *ep;

   newlink(&sp);
   *sp = *lp;
   infirst(sp, hp);
   ep = lastlink(hp);
//   if(ep == NULL) return;
   while ( is_less(lp->data, ep->data) )
      ep = predlink(ep);
   insucc(lp, ep);
   elimlink(&sp);
}

linktyp *firstlink(headtyp *hp)
/* Returnerar pekare till första länken i listan */
{
   linktyp *ep;

   if ( !empty(hp) )
      ep = hp->next;
   else
      ep = NULL;
   return ep;
}

linktyp *lastlink(headtyp *hp)
/* Returnerar pekare till sista länken i listan */
{
   linktyp *ep;

   if ( !empty(hp) )
      ep = hp->befo;
   else
      ep = NULL;
   return ep;

}

linktyp *predlink(linktyp *lp)
/* Returnerar pekare till länken före  */
{
   linktyp *ep;

   if ( is_link(lp->befo) )
      ep = lp->befo;
   else
      ep = NULL;
   return ep;
}

linktyp *succlink(linktyp *lp)
/* Returnerar pekare till länken efter */
{
   linktyp *ep;

   if ( is_link(lp->next) )
      ep = lp->next;
   else
      ep = NULL;
   return ep;
}

int is_link(linktyp *lp)
/* Returnerar 1 om länk annars 0 */
{
   return (lp->kind == lank);
}

int empty(headtyp *hp)
/* Returnerar 1 om listan tom annars 0 */
{
    return ( hp->next->kind == head );
}

int nrlinks(headtyp *hp)
/* Returnerar antalet länkar i listan */
{
   int sum = 0;
   linktyp *ep;

   ep = firstlink(hp);
   while ( ep != NULL )
   {
      sum++;
      ep = succlink(ep);
   }
   return sum;
}

void outlist(linktyp *lp)
/* Tar bort länken från listan */
{
   if (lp->befo != NULL && lp->next !=NULL)
   {
      lp->befo->next = lp->next;
      lp->next->befo = lp->befo;
      lp->befo = NULL;
      lp->next = NULL;
   }
}

void elimlink(linktyp **lpp)
/* Tar bort, avallokerar och NULL-ställer länken */
{
   outlist(*lpp);
   free(*lpp);
   *lpp = NULL;
}

void clearhead(headtyp *hp)
/* Eliminerar alla länkar från listan */
{
   linktyp *ep;

   while ( !empty(hp) )
   {
      ep = firstlink(hp);
      elimlink(&ep);
   }
}

void elimhead(headtyp **hpp)
/* Eliminerar och NULL-ställer listan */
{
   clearhead(*hpp);
   free(*hpp);
   *hpp = NULL;
}


