#ifndef INC_wb_wbllexer_hpp_
#define INC_wb_wbllexer_hpp_

#include <antlr/config.hpp>
/* $ANTLR 2.7.5 (20050128): "wb_wbllexer.g" -> "wb_wbllexer.hpp"$ */
#include <antlr/CommonToken.hpp>
#include <antlr/InputBuffer.hpp>
#include <antlr/BitSet.hpp>
#include "wb_wblvocabTokenTypes.hpp"
#include <antlr/CharScanner.hpp>
class CUSTOM_API wb_wbllexer : public ANTLR_USE_NAMESPACE(antlr)CharScanner, public wb_wblvocabTokenTypes
{
#line 1 "wb_wbllexer.g"
#line 15 "wb_wbllexer.hpp"
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const
	{
		return true;
	}
public:
	wb_wbllexer(ANTLR_USE_NAMESPACE(std)istream& in);
	wb_wbllexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib);
	wb_wbllexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state);
	ANTLR_USE_NAMESPACE(antlr)RefToken nextToken();
	public: void mEQ(bool _createToken);
	public: void mOREQ(bool _createToken);
	public: void mWS(bool _createToken);
	public: void mCOMMENT(bool _createToken);
	public: void mDOCBLOCK(bool _createToken);
	public: void mINDEX(bool _createToken);
	public: void mVALUE(bool _createToken);
	protected: void mSWEC(bool _createToken);
	protected: void mDIGITS(bool _createToken);
	public: void mCHAR_LITERAL(bool _createToken);
	protected: void mESC(bool _createToken);
	public: void mSTRING_LITERAL(bool _createToken);
private:
	
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
};

#endif /*INC_wb_wbllexer_hpp_*/
