#ifndef glow_array_h
#define glow_array_h

#include <iostream.h>

#include "glow_array_elem.h"

typedef void *glow_tAddress;

typedef enum {
	glow_eElemTypeRectangle,
	glow_eElemTypeCircle,
	glow_eElemTypeLine,
	glow_eElemTypePoint,
	glow_eElemTypeNode,
	glow_eElemTypeConnection
	} glow_eElemType;

class GlowArray {
  public:
    GlowArray( int allocate = 10, int incr = 10);
    GlowArrayElem *operator[] ( int idx);
    friend ostream& operator<< ( ostream& o, const GlowArray a);
    int size() { return a_size;};
    void insert( GlowArrayElem *element);
    void zoom();
    friend class GlowNodeClass;
    friend class GlowCtx;
  private:
    int	allocated;
    int alloc_incr;
    int a_size;
    GlowArrayElem **a;
};

#endif
