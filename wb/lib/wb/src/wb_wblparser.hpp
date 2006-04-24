#ifndef INC_wb_wblparser_hpp_
#define INC_wb_wblparser_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.6 (20060211): "wb_wblparser.g" -> "wb_wblparser.hpp"$ */
#include <antlr/TokenStream.hpp>
#include <antlr/TokenBuffer.hpp>
#include "wb_wblparserTokenTypes.hpp"
#include <antlr/LLkParser.hpp>

#line 8 "wb_wblparser.g"

extern int wblparser_error_cnt;


#line 19 "wb_wblparser.hpp"
#line 4 "wb_wblparser.g"

#include "wb_wblnode.h"

#line 24 "wb_wblparser.hpp"
class CUSTOM_API wb_wblparser : public ANTLR_USE_NAMESPACE(antlr)LLkParser, public wb_wblparserTokenTypes
{
void reportError(const RecognitionException& ex);
#line 1 "wb_wblparser.g"
#line 28 "wb_wblparser.hpp"
public:
	void initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& factory );
protected:
	wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k);
public:
	wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf);
protected:
	wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k);
public:
	wb_wblparser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer);
	wb_wblparser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state);
	int getNumTokens() const
	{
		return wb_wblparser::NUM_TOKENS;
	}
	const char* getTokenName( int type ) const
	{
		if( type > getNumTokens() ) return 0;
		return wb_wblparser::tokenNames[type];
	}
	const char* const* getTokenNames() const
	{
		return wb_wblparser::tokenNames;
	}
	public: void unit();
	public: void volume();
	public: void sobject();
	public: void object();
	public: void cid();
	public: void body();
	public: void oix();
	public: void attribute();
	public: void value();
	public: void oname();
public:
	ANTLR_USE_NAMESPACE(antlr)RefAST getAST()
	{
		return ANTLR_USE_NAMESPACE(antlr)RefAST(returnAST);
	}
	
protected:
	ref_wblnode returnAST;
private:
	static const char* tokenNames[];
#ifndef NO_STATIC_CONSTS
	static const int NUM_TOKENS = 36;
#else
	enum {
		NUM_TOKENS = 36
	};
#endif
	
	static const unsigned long _tokenSet_0_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_1;
	static const unsigned long _tokenSet_2_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_2;
	static const unsigned long _tokenSet_3_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_3;
	static const unsigned long _tokenSet_4_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_4;
	static const unsigned long _tokenSet_5_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_5;
	static const unsigned long _tokenSet_6_data_[];
	static const ANTLR_USE_NAMESPACE(antlr)BitSet _tokenSet_6;
};

#endif /*INC_wb_wblparser_hpp_*/
