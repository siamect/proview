#ifndef wb_export_h
#define wb_export_h

class wb_import;

class wb_export
{
public:

  virtual bool exportVolume(wb_import &e) = 0;
    
  virtual bool exportHead(wb_import &e) = 0;

  virtual bool exportRbody(wb_import &e) = 0;
    
  virtual bool exportDbody(wb_import &e) = 0;
    
  virtual bool exportDocBlock(wb_import &e) = 0;
    
  virtual bool exportMeta(wb_import &e) = 0;
};

#endif
