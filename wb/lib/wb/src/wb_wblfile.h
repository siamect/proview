#ifndef wb_wblfile_h
#define wb_wblfile_h

#include <string>

// #include "antlr/AST.hpp"
// #include "wb_wbllexer.hpp"
// #include "wb_wblparser.hpp"

class wb_wblparser;
class wb_wbllexer;
class wb_wblnode;

class wb_wblfile {
  public:
    wb_wbllexer *lexer;
    wb_wblparser *parser;
    wb_wblnode *rootAST;
    char file_name[200];
    pwr_tTime time;
};

#endif
