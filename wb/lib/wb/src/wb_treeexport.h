#ifndef wb_treeexport_h
#define wb_treeexport_h

class wb_treeimport;

class wb_treeexport
{
public:

  virtual bool exportTree(wb_treeimport &i, pwr_tOid oid) = 0;
};

#endif

