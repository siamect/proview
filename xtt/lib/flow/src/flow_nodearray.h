#ifndef flow_array_h
#define flow_array_h

#include <iostream.h>

#include "flow_array_elem.h"

typedef void *flow_tAddress;

typedef enum {
	flow_eElemTypeRectangle,
	flow_eElemTypeCircle,
	flow_eElemTypeLine,
	flow_eElemTypePoint,
	flow_eElemTypeNode,
	flow_eElemTypeConnection
	} flow_eElemType;

class FlowArray {
  public:
    FlowArray( int allocate = 10, int incr = 10);
    FlowArrayElem *operator[] ( int idx);
    friend ostream& operator<< ( ostream& o, const FlowArray a);
    int size() { return a_size;};
    void insert( FlowArrayElem *element);
    void zoom();
    friend class FlowNodeClass;
    friend class FlowCtx;
  private:
    int	allocated;
    int alloc_incr;
    int a_size;
    FlowArrayElem **a;
};

#endif
