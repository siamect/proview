/* $ANTLR 2.7.6 (20060211): "wb_wbllexer.g" -> "wb_wbllexer.cpp"$ */
#include "wb_wbllexer.hpp"
#include <antlr/CharBuffer.hpp>
#include <antlr/TokenStreamException.hpp>
#include <antlr/TokenStreamIOException.hpp>
#include <antlr/TokenStreamRecognitionException.hpp>
#include <antlr/CharStreamException.hpp>
#include <antlr/CharStreamIOException.hpp>
#include <antlr/NoViableAltForCharException.hpp>

#line 1 "wb_wbllexer.g"
#line 13 "wb_wbllexer.cpp"
wb_wbllexer::wb_wbllexer(ANTLR_USE_NAMESPACE(std)istream& in)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in),true)
{
	initLiterals();
}

wb_wbllexer::wb_wbllexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(ib,true)
{
	initLiterals();
}

wb_wbllexer::wb_wbllexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(state,true)
{
	initLiterals();
}

void wb_wbllexer::initLiterals()
{
}

ANTLR_USE_NAMESPACE(antlr)RefToken wb_wbllexer::nextToken()
{
	ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
	for (;;) {
		ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
		int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
		resetText();
		try {   // for lexical and char stream error handling
			switch ( LA(1)) {
			case 0x3d /* '=' */ :
			{
				mEQ(true);
				theRetToken=_returnToken;
				break;
			}
			case 0x7c /* '|' */ :
			{
				mOREQ(true);
				theRetToken=_returnToken;
				break;
			}
			case 0x9 /* '\t' */ :
			case 0xa /* '\n' */ :
			case 0xd /* '\r' */ :
			case 0x20 /* ' ' */ :
			{
				mWS(true);
				theRetToken=_returnToken;
				break;
			}
			case 0x5b /* '[' */ :
			{
				mINDEX(true);
				theRetToken=_returnToken;
				break;
			}
			case 0x24 /* '$' */ :
			case 0x2d /* '-' */ :
			case 0x2e /* '.' */ :
			case 0x30 /* '0' */ :
			case 0x31 /* '1' */ :
			case 0x32 /* '2' */ :
			case 0x33 /* '3' */ :
			case 0x34 /* '4' */ :
			case 0x35 /* '5' */ :
			case 0x36 /* '6' */ :
			case 0x37 /* '7' */ :
			case 0x38 /* '8' */ :
			case 0x39 /* '9' */ :
			case 0x41 /* 'A' */ :
			case 0x42 /* 'B' */ :
			case 0x43 /* 'C' */ :
			case 0x44 /* 'D' */ :
			case 0x45 /* 'E' */ :
			case 0x46 /* 'F' */ :
			case 0x47 /* 'G' */ :
			case 0x48 /* 'H' */ :
			case 0x49 /* 'I' */ :
			case 0x4a /* 'J' */ :
			case 0x4b /* 'K' */ :
			case 0x4c /* 'L' */ :
			case 0x4d /* 'M' */ :
			case 0x4e /* 'N' */ :
			case 0x4f /* 'O' */ :
			case 0x50 /* 'P' */ :
			case 0x51 /* 'Q' */ :
			case 0x52 /* 'R' */ :
			case 0x53 /* 'S' */ :
			case 0x54 /* 'T' */ :
			case 0x55 /* 'U' */ :
			case 0x56 /* 'V' */ :
			case 0x57 /* 'W' */ :
			case 0x58 /* 'X' */ :
			case 0x59 /* 'Y' */ :
			case 0x5a /* 'Z' */ :
			case 0x5f /* '_' */ :
			case 0x61 /* 'a' */ :
			case 0x62 /* 'b' */ :
			case 0x63 /* 'c' */ :
			case 0x64 /* 'd' */ :
			case 0x65 /* 'e' */ :
			case 0x66 /* 'f' */ :
			case 0x67 /* 'g' */ :
			case 0x68 /* 'h' */ :
			case 0x69 /* 'i' */ :
			case 0x6a /* 'j' */ :
			case 0x6b /* 'k' */ :
			case 0x6c /* 'l' */ :
			case 0x6d /* 'm' */ :
			case 0x6e /* 'n' */ :
			case 0x6f /* 'o' */ :
			case 0x70 /* 'p' */ :
			case 0x71 /* 'q' */ :
			case 0x72 /* 'r' */ :
			case 0x73 /* 's' */ :
			case 0x74 /* 't' */ :
			case 0x75 /* 'u' */ :
			case 0x76 /* 'v' */ :
			case 0x77 /* 'w' */ :
			case 0x78 /* 'x' */ :
			case 0x79 /* 'y' */ :
			case 0x7a /* 'z' */ :
			case 0xc4:
			case 0xc5:
			case 0xd6:
			case 0xe4:
			case 0xe5:
			case 0xf6:
			{
				mVALUE(true);
				theRetToken=_returnToken;
				break;
			}
			case 0x27 /* '\'' */ :
			{
				mCHAR_LITERAL(true);
				theRetToken=_returnToken;
				break;
			}
			case 0x22 /* '\"' */ :
			{
				mSTRING_LITERAL(true);
				theRetToken=_returnToken;
				break;
			}
			default:
				if ((LA(1) == 0x21 /* '!' */ ) && (LA(2) == 0x9 /* '\t' */  || LA(2) == 0xa /* '\n' */  || LA(2) == 0x20 /* ' ' */ )) {
					mCOMMENT(true);
					theRetToken=_returnToken;
				}
				else if ((LA(1) == 0x21 /* '!' */ ) && (LA(2) == 0x2f /* '/' */ )) {
					mDOCBLOCK(true);
					theRetToken=_returnToken;
				}
			else {
				if (LA(1)==EOF_CHAR)
				{
					uponEOF();
					_returnToken = makeToken(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
				}
				else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
			}
			}
			if ( !_returnToken )
				goto tryAgain; // found SKIP token

			_ttype = _returnToken->getType();
			_ttype = testLiteralsTable(_ttype);
			_returnToken->setType(_ttype);
			return _returnToken;
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& e) {
				throw ANTLR_USE_NAMESPACE(antlr)TokenStreamRecognitionException(e);
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamIOException& csie) {
			throw ANTLR_USE_NAMESPACE(antlr)TokenStreamIOException(csie.io);
		}
		catch (ANTLR_USE_NAMESPACE(antlr)CharStreamException& cse) {
			throw ANTLR_USE_NAMESPACE(antlr)TokenStreamException(cse.getMessage());
		}
tryAgain:;
	}
}

void wb_wbllexer::mEQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = EQ;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	match('=' /* charlit */ );
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mOREQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = OREQ;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	match("|=");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mWS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = WS;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	{ // ( ... )+
	int _cnt5=0;
	for (;;) {
		switch ( LA(1)) {
		case 0x20 /* ' ' */ :
		{
			match(' ' /* charlit */ );
			break;
		}
		case 0x9 /* '\t' */ :
		{
			match('\t' /* charlit */ );
			break;
		}
		case 0xa /* '\n' */ :
		{
			match('\n' /* charlit */ );
			if ( inputState->guessing==0 ) {
#line 50 "wb_wbllexer.g"
				newline();
#line 253 "wb_wbllexer.cpp"
			}
			break;
		}
		case 0xd /* '\r' */ :
		{
			match('\r' /* charlit */ );
			break;
		}
		default:
		{
			if ( _cnt5>=1 ) { goto _loop5; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
		}
		}
		_cnt5++;
	}
	_loop5:;
	}  // ( ... )+
	if ( inputState->guessing==0 ) {
#line 52 "wb_wbllexer.g"
		_ttype = antlr::Token::SKIP;
#line 274 "wb_wbllexer.cpp"
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mCOMMENT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = COMMENT;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	{
	if ((LA(1) == 0x21 /* '!' */ ) && (LA(2) == 0x9 /* '\t' */  || LA(2) == 0x20 /* ' ' */ )) {
		match("!");
		{
		switch ( LA(1)) {
		case 0x20 /* ' ' */ :
		{
			match(' ' /* charlit */ );
			break;
		}
		case 0x9 /* '\t' */ :
		{
			match('\t' /* charlit */ );
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
		}
		}
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				matchNot('\n' /* charlit */ );
			}
			else {
				goto _loop10;
			}
			
		}
		_loop10:;
		} // ( ... )*
		match('\n' /* charlit */ );
	}
	else if ((LA(1) == 0x21 /* '!' */ ) && (LA(2) == 0xa /* '\n' */ )) {
		match("!");
		match('\n' /* charlit */ );
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
	}
	
	}
	if ( inputState->guessing==0 ) {
#line 58 "wb_wbllexer.g"
		_ttype = antlr::Token::SKIP; newline();
#line 336 "wb_wbllexer.cpp"
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mDOCBLOCK(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = DOCBLOCK;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	match("!/**");
	{ // ( ... )*
	for (;;) {
		if (((LA(1) == 0x21 /* '!' */ ) && ((LA(2) >= 0x3 /* '\3' */  && LA(2) <= 0xff)))&&( LA(2)!='*' )) {
			match('!' /* charlit */ );
		}
		else if ((LA(1) == 0xd /* '\r' */ ) && (LA(2) == 0xa /* '\n' */ )) {
			match('\r' /* charlit */ );
			match('\n' /* charlit */ );
			if ( inputState->guessing==0 ) {
#line 75 "wb_wbllexer.g"
				newline();
#line 363 "wb_wbllexer.cpp"
			}
		}
		else if ((LA(1) == 0xd /* '\r' */ ) && ((LA(2) >= 0x3 /* '\3' */  && LA(2) <= 0xff))) {
			match('\r' /* charlit */ );
			if ( inputState->guessing==0 ) {
#line 76 "wb_wbllexer.g"
				newline();
#line 371 "wb_wbllexer.cpp"
			}
		}
		else if ((LA(1) == 0xa /* '\n' */ )) {
			match('\n' /* charlit */ );
			if ( inputState->guessing==0 ) {
#line 77 "wb_wbllexer.g"
				newline();
#line 379 "wb_wbllexer.cpp"
			}
		}
		else if ((_tokenSet_1.member(LA(1)))) {
			{
			match(_tokenSet_1);
			}
		}
		else {
			goto _loop14;
		}
		
	}
	_loop14:;
	} // ( ... )*
	match("!*/");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mINDEX(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = INDEX;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	_saveIndex = text.length();
	match('[' /* charlit */ );
	text.erase(_saveIndex);
	{ // ( ... )+
	int _cnt17=0;
	for (;;) {
		if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
			matchRange('0','9');
		}
		else {
			if ( _cnt17>=1 ) { goto _loop17; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
		}
		
		_cnt17++;
	}
	_loop17:;
	}  // ( ... )+
	_saveIndex = text.length();
	match(']' /* charlit */ );
	text.erase(_saveIndex);
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mVALUE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = VALUE;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	ANTLR_USE_NAMESPACE(antlr)RefToken oname;
	ANTLR_USE_NAMESPACE(antlr)RefToken soname;
	ANTLR_USE_NAMESPACE(antlr)RefToken doname;
	ANTLR_USE_NAMESPACE(antlr)RefToken vname;
	ANTLR_USE_NAMESPACE(antlr)RefToken buname;
	ANTLR_USE_NAMESPACE(antlr)RefToken boname;
	ANTLR_USE_NAMESPACE(antlr)RefToken aname;
	
	bool synPredMatched20 = false;
	if (((LA(1) == 0x4f /* 'O' */ ) && (LA(2) == 0x62 /* 'b' */ ))) {
		int _m20 = mark();
		synPredMatched20 = true;
		inputState->guessing++;
		try {
			{
			match("Object");
			mWS(false);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched20 = false;
		}
		rewind(_m20);
		inputState->guessing--;
	}
	if ( synPredMatched20 ) {
		_saveIndex = text.length();
		match("Object");
		text.erase(_saveIndex);
		_saveIndex = text.length();
		mWS(false);
		text.erase(_saveIndex);
		{ // ( ... )+
		int _cnt_oname=0;
		for (;;) {
			switch ( LA(1)) {
			case 0x61 /* 'a' */ :
			case 0x62 /* 'b' */ :
			case 0x63 /* 'c' */ :
			case 0x64 /* 'd' */ :
			case 0x65 /* 'e' */ :
			case 0x66 /* 'f' */ :
			case 0x67 /* 'g' */ :
			case 0x68 /* 'h' */ :
			case 0x69 /* 'i' */ :
			case 0x6a /* 'j' */ :
			case 0x6b /* 'k' */ :
			case 0x6c /* 'l' */ :
			case 0x6d /* 'm' */ :
			case 0x6e /* 'n' */ :
			case 0x6f /* 'o' */ :
			case 0x70 /* 'p' */ :
			case 0x71 /* 'q' */ :
			case 0x72 /* 'r' */ :
			case 0x73 /* 's' */ :
			case 0x74 /* 't' */ :
			case 0x75 /* 'u' */ :
			case 0x76 /* 'v' */ :
			case 0x77 /* 'w' */ :
			case 0x78 /* 'x' */ :
			case 0x79 /* 'y' */ :
			case 0x7a /* 'z' */ :
			{
				matchRange('a','z');
				break;
			}
			case 0x41 /* 'A' */ :
			case 0x42 /* 'B' */ :
			case 0x43 /* 'C' */ :
			case 0x44 /* 'D' */ :
			case 0x45 /* 'E' */ :
			case 0x46 /* 'F' */ :
			case 0x47 /* 'G' */ :
			case 0x48 /* 'H' */ :
			case 0x49 /* 'I' */ :
			case 0x4a /* 'J' */ :
			case 0x4b /* 'K' */ :
			case 0x4c /* 'L' */ :
			case 0x4d /* 'M' */ :
			case 0x4e /* 'N' */ :
			case 0x4f /* 'O' */ :
			case 0x50 /* 'P' */ :
			case 0x51 /* 'Q' */ :
			case 0x52 /* 'R' */ :
			case 0x53 /* 'S' */ :
			case 0x54 /* 'T' */ :
			case 0x55 /* 'U' */ :
			case 0x56 /* 'V' */ :
			case 0x57 /* 'W' */ :
			case 0x58 /* 'X' */ :
			case 0x59 /* 'Y' */ :
			case 0x5a /* 'Z' */ :
			{
				matchRange('A','Z');
				break;
			}
			case 0x30 /* '0' */ :
			case 0x31 /* '1' */ :
			case 0x32 /* '2' */ :
			case 0x33 /* '3' */ :
			case 0x34 /* '4' */ :
			case 0x35 /* '5' */ :
			case 0x36 /* '6' */ :
			case 0x37 /* '7' */ :
			case 0x38 /* '8' */ :
			case 0x39 /* '9' */ :
			{
				matchRange('0','9');
				break;
			}
			case 0x24 /* '$' */ :
			{
				match('$' /* charlit */ );
				break;
			}
			case 0x5f /* '_' */ :
			{
				match('_' /* charlit */ );
				break;
			}
			case 0xc4:
			case 0xc5:
			case 0xd6:
			case 0xe4:
			case 0xe5:
			case 0xf6:
			{
				mSWEC(false);
				break;
			}
			default:
			{
				if ( _cnt_oname>=1 ) { goto oname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
			}
			}
			_cnt_oname++;
		}
		oname:;
		}  // ( ... )+
		if ( inputState->guessing==0 ) {
#line 101 "wb_wbllexer.g"
			_ttype = OBJECT;
#line 582 "wb_wbllexer.cpp"
		}
	}
	else {
		bool synPredMatched24 = false;
		if (((LA(1) == 0x45 /* 'E' */ ) && (LA(2) == 0x6e /* 'n' */ ))) {
			int _m24 = mark();
			synPredMatched24 = true;
			inputState->guessing++;
			try {
				{
				match("EndObject");
				mWS(false);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched24 = false;
			}
			rewind(_m24);
			inputState->guessing--;
		}
		if ( synPredMatched24 ) {
			match("EndObject");
			_saveIndex = text.length();
			mWS(false);
			text.erase(_saveIndex);
			if ( inputState->guessing==0 ) {
#line 103 "wb_wbllexer.g"
				_ttype = ENDOBJECT;
#line 611 "wb_wbllexer.cpp"
			}
		}
		else {
			bool synPredMatched26 = false;
			if (((LA(1) == 0x53 /* 'S' */ ) && (LA(2) == 0x4f /* 'O' */ ))) {
				int _m26 = mark();
				synPredMatched26 = true;
				inputState->guessing++;
				try {
					{
					match("SObject");
					mWS(false);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched26 = false;
				}
				rewind(_m26);
				inputState->guessing--;
			}
			if ( synPredMatched26 ) {
				_saveIndex = text.length();
				match("SObject");
				text.erase(_saveIndex);
				_saveIndex = text.length();
				mWS(false);
				text.erase(_saveIndex);
				{ // ( ... )+
				int _cnt_soname=0;
				for (;;) {
					switch ( LA(1)) {
					case 0x61 /* 'a' */ :
					case 0x62 /* 'b' */ :
					case 0x63 /* 'c' */ :
					case 0x64 /* 'd' */ :
					case 0x65 /* 'e' */ :
					case 0x66 /* 'f' */ :
					case 0x67 /* 'g' */ :
					case 0x68 /* 'h' */ :
					case 0x69 /* 'i' */ :
					case 0x6a /* 'j' */ :
					case 0x6b /* 'k' */ :
					case 0x6c /* 'l' */ :
					case 0x6d /* 'm' */ :
					case 0x6e /* 'n' */ :
					case 0x6f /* 'o' */ :
					case 0x70 /* 'p' */ :
					case 0x71 /* 'q' */ :
					case 0x72 /* 'r' */ :
					case 0x73 /* 's' */ :
					case 0x74 /* 't' */ :
					case 0x75 /* 'u' */ :
					case 0x76 /* 'v' */ :
					case 0x77 /* 'w' */ :
					case 0x78 /* 'x' */ :
					case 0x79 /* 'y' */ :
					case 0x7a /* 'z' */ :
					{
						matchRange('a','z');
						break;
					}
					case 0x41 /* 'A' */ :
					case 0x42 /* 'B' */ :
					case 0x43 /* 'C' */ :
					case 0x44 /* 'D' */ :
					case 0x45 /* 'E' */ :
					case 0x46 /* 'F' */ :
					case 0x47 /* 'G' */ :
					case 0x48 /* 'H' */ :
					case 0x49 /* 'I' */ :
					case 0x4a /* 'J' */ :
					case 0x4b /* 'K' */ :
					case 0x4c /* 'L' */ :
					case 0x4d /* 'M' */ :
					case 0x4e /* 'N' */ :
					case 0x4f /* 'O' */ :
					case 0x50 /* 'P' */ :
					case 0x51 /* 'Q' */ :
					case 0x52 /* 'R' */ :
					case 0x53 /* 'S' */ :
					case 0x54 /* 'T' */ :
					case 0x55 /* 'U' */ :
					case 0x56 /* 'V' */ :
					case 0x57 /* 'W' */ :
					case 0x58 /* 'X' */ :
					case 0x59 /* 'Y' */ :
					case 0x5a /* 'Z' */ :
					{
						matchRange('A','Z');
						break;
					}
					case 0x30 /* '0' */ :
					case 0x31 /* '1' */ :
					case 0x32 /* '2' */ :
					case 0x33 /* '3' */ :
					case 0x34 /* '4' */ :
					case 0x35 /* '5' */ :
					case 0x36 /* '6' */ :
					case 0x37 /* '7' */ :
					case 0x38 /* '8' */ :
					case 0x39 /* '9' */ :
					{
						matchRange('0','9');
						break;
					}
					case 0x24 /* '$' */ :
					{
						match('$' /* charlit */ );
						break;
					}
					case 0x5f /* '_' */ :
					{
						match('_' /* charlit */ );
						break;
					}
					case 0x3a /* ':' */ :
					{
						match(':' /* charlit */ );
						break;
					}
					case 0x2d /* '-' */ :
					{
						match('-' /* charlit */ );
						break;
					}
					case 0xc4:
					case 0xc5:
					case 0xd6:
					case 0xe4:
					case 0xe5:
					case 0xf6:
					{
						mSWEC(false);
						break;
					}
					default:
					{
						if ( _cnt_soname>=1 ) { goto soname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
					}
					}
					_cnt_soname++;
				}
				soname:;
				}  // ( ... )+
				if ( inputState->guessing==0 ) {
#line 105 "wb_wbllexer.g"
					_ttype = SOBJECT;
#line 759 "wb_wbllexer.cpp"
				}
			}
			else {
				bool synPredMatched30 = false;
				if (((LA(1) == 0x45 /* 'E' */ ) && (LA(2) == 0x6e /* 'n' */ ))) {
					int _m30 = mark();
					synPredMatched30 = true;
					inputState->guessing++;
					try {
						{
						match("EndSObject");
						mWS(false);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched30 = false;
					}
					rewind(_m30);
					inputState->guessing--;
				}
				if ( synPredMatched30 ) {
					match("EndSObject");
					_saveIndex = text.length();
					mWS(false);
					text.erase(_saveIndex);
					if ( inputState->guessing==0 ) {
#line 107 "wb_wbllexer.g"
						_ttype = ENDSOBJECT;
#line 788 "wb_wbllexer.cpp"
					}
				}
				else {
					bool synPredMatched32 = false;
					if (((LA(1) == 0x44 /* 'D' */ ) && (LA(2) == 0x4f /* 'O' */ ))) {
						int _m32 = mark();
						synPredMatched32 = true;
						inputState->guessing++;
						try {
							{
							match("DObject");
							mWS(false);
							}
						}
						catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
							synPredMatched32 = false;
						}
						rewind(_m32);
						inputState->guessing--;
					}
					if ( synPredMatched32 ) {
						_saveIndex = text.length();
						match("DObject");
						text.erase(_saveIndex);
						_saveIndex = text.length();
						mWS(false);
						text.erase(_saveIndex);
						{ // ( ... )+
						int _cnt_doname=0;
						for (;;) {
							switch ( LA(1)) {
							case 0x61 /* 'a' */ :
							case 0x62 /* 'b' */ :
							case 0x63 /* 'c' */ :
							case 0x64 /* 'd' */ :
							case 0x65 /* 'e' */ :
							case 0x66 /* 'f' */ :
							case 0x67 /* 'g' */ :
							case 0x68 /* 'h' */ :
							case 0x69 /* 'i' */ :
							case 0x6a /* 'j' */ :
							case 0x6b /* 'k' */ :
							case 0x6c /* 'l' */ :
							case 0x6d /* 'm' */ :
							case 0x6e /* 'n' */ :
							case 0x6f /* 'o' */ :
							case 0x70 /* 'p' */ :
							case 0x71 /* 'q' */ :
							case 0x72 /* 'r' */ :
							case 0x73 /* 's' */ :
							case 0x74 /* 't' */ :
							case 0x75 /* 'u' */ :
							case 0x76 /* 'v' */ :
							case 0x77 /* 'w' */ :
							case 0x78 /* 'x' */ :
							case 0x79 /* 'y' */ :
							case 0x7a /* 'z' */ :
							{
								matchRange('a','z');
								break;
							}
							case 0x41 /* 'A' */ :
							case 0x42 /* 'B' */ :
							case 0x43 /* 'C' */ :
							case 0x44 /* 'D' */ :
							case 0x45 /* 'E' */ :
							case 0x46 /* 'F' */ :
							case 0x47 /* 'G' */ :
							case 0x48 /* 'H' */ :
							case 0x49 /* 'I' */ :
							case 0x4a /* 'J' */ :
							case 0x4b /* 'K' */ :
							case 0x4c /* 'L' */ :
							case 0x4d /* 'M' */ :
							case 0x4e /* 'N' */ :
							case 0x4f /* 'O' */ :
							case 0x50 /* 'P' */ :
							case 0x51 /* 'Q' */ :
							case 0x52 /* 'R' */ :
							case 0x53 /* 'S' */ :
							case 0x54 /* 'T' */ :
							case 0x55 /* 'U' */ :
							case 0x56 /* 'V' */ :
							case 0x57 /* 'W' */ :
							case 0x58 /* 'X' */ :
							case 0x59 /* 'Y' */ :
							case 0x5a /* 'Z' */ :
							{
								matchRange('A','Z');
								break;
							}
							case 0x30 /* '0' */ :
							case 0x31 /* '1' */ :
							case 0x32 /* '2' */ :
							case 0x33 /* '3' */ :
							case 0x34 /* '4' */ :
							case 0x35 /* '5' */ :
							case 0x36 /* '6' */ :
							case 0x37 /* '7' */ :
							case 0x38 /* '8' */ :
							case 0x39 /* '9' */ :
							{
								matchRange('0','9');
								break;
							}
							case 0x24 /* '$' */ :
							{
								match('$' /* charlit */ );
								break;
							}
							case 0x5f /* '_' */ :
							{
								match('_' /* charlit */ );
								break;
							}
							case 0xc4:
							case 0xc5:
							case 0xd6:
							case 0xe4:
							case 0xe5:
							case 0xf6:
							{
								mSWEC(false);
								break;
							}
							default:
							{
								if ( _cnt_doname>=1 ) { goto doname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
							}
							}
							_cnt_doname++;
						}
						doname:;
						}  // ( ... )+
						if ( inputState->guessing==0 ) {
#line 109 "wb_wbllexer.g"
							_ttype = DOBJECT;
#line 926 "wb_wbllexer.cpp"
						}
					}
					else {
						bool synPredMatched36 = false;
						if (((LA(1) == 0x45 /* 'E' */ ) && (LA(2) == 0x6e /* 'n' */ ))) {
							int _m36 = mark();
							synPredMatched36 = true;
							inputState->guessing++;
							try {
								{
								match("EndDObject");
								mWS(false);
								}
							}
							catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
								synPredMatched36 = false;
							}
							rewind(_m36);
							inputState->guessing--;
						}
						if ( synPredMatched36 ) {
							match("EndDObject");
							_saveIndex = text.length();
							mWS(false);
							text.erase(_saveIndex);
							if ( inputState->guessing==0 ) {
#line 111 "wb_wbllexer.g"
								_ttype = ENDDOBJECT;
#line 955 "wb_wbllexer.cpp"
							}
						}
						else {
							bool synPredMatched38 = false;
							if (((LA(1) == 0x56 /* 'V' */ ) && (LA(2) == 0x6f /* 'o' */ ))) {
								int _m38 = mark();
								synPredMatched38 = true;
								inputState->guessing++;
								try {
									{
									match("Volume");
									mWS(false);
									}
								}
								catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
									synPredMatched38 = false;
								}
								rewind(_m38);
								inputState->guessing--;
							}
							if ( synPredMatched38 ) {
								_saveIndex = text.length();
								match("Volume");
								text.erase(_saveIndex);
								_saveIndex = text.length();
								mWS(false);
								text.erase(_saveIndex);
								{ // ( ... )+
								int _cnt_vname=0;
								for (;;) {
									switch ( LA(1)) {
									case 0x61 /* 'a' */ :
									case 0x62 /* 'b' */ :
									case 0x63 /* 'c' */ :
									case 0x64 /* 'd' */ :
									case 0x65 /* 'e' */ :
									case 0x66 /* 'f' */ :
									case 0x67 /* 'g' */ :
									case 0x68 /* 'h' */ :
									case 0x69 /* 'i' */ :
									case 0x6a /* 'j' */ :
									case 0x6b /* 'k' */ :
									case 0x6c /* 'l' */ :
									case 0x6d /* 'm' */ :
									case 0x6e /* 'n' */ :
									case 0x6f /* 'o' */ :
									case 0x70 /* 'p' */ :
									case 0x71 /* 'q' */ :
									case 0x72 /* 'r' */ :
									case 0x73 /* 's' */ :
									case 0x74 /* 't' */ :
									case 0x75 /* 'u' */ :
									case 0x76 /* 'v' */ :
									case 0x77 /* 'w' */ :
									case 0x78 /* 'x' */ :
									case 0x79 /* 'y' */ :
									case 0x7a /* 'z' */ :
									{
										matchRange('a','z');
										break;
									}
									case 0x41 /* 'A' */ :
									case 0x42 /* 'B' */ :
									case 0x43 /* 'C' */ :
									case 0x44 /* 'D' */ :
									case 0x45 /* 'E' */ :
									case 0x46 /* 'F' */ :
									case 0x47 /* 'G' */ :
									case 0x48 /* 'H' */ :
									case 0x49 /* 'I' */ :
									case 0x4a /* 'J' */ :
									case 0x4b /* 'K' */ :
									case 0x4c /* 'L' */ :
									case 0x4d /* 'M' */ :
									case 0x4e /* 'N' */ :
									case 0x4f /* 'O' */ :
									case 0x50 /* 'P' */ :
									case 0x51 /* 'Q' */ :
									case 0x52 /* 'R' */ :
									case 0x53 /* 'S' */ :
									case 0x54 /* 'T' */ :
									case 0x55 /* 'U' */ :
									case 0x56 /* 'V' */ :
									case 0x57 /* 'W' */ :
									case 0x58 /* 'X' */ :
									case 0x59 /* 'Y' */ :
									case 0x5a /* 'Z' */ :
									{
										matchRange('A','Z');
										break;
									}
									case 0x30 /* '0' */ :
									case 0x31 /* '1' */ :
									case 0x32 /* '2' */ :
									case 0x33 /* '3' */ :
									case 0x34 /* '4' */ :
									case 0x35 /* '5' */ :
									case 0x36 /* '6' */ :
									case 0x37 /* '7' */ :
									case 0x38 /* '8' */ :
									case 0x39 /* '9' */ :
									{
										matchRange('0','9');
										break;
									}
									case 0x24 /* '$' */ :
									{
										match('$' /* charlit */ );
										break;
									}
									case 0x5f /* '_' */ :
									{
										match('_' /* charlit */ );
										break;
									}
									case 0xc4:
									case 0xc5:
									case 0xd6:
									case 0xe4:
									case 0xe5:
									case 0xf6:
									{
										mSWEC(false);
										break;
									}
									default:
									{
										if ( _cnt_vname>=1 ) { goto vname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
									}
									}
									_cnt_vname++;
								}
								vname:;
								}  // ( ... )+
								{
								if ((LA(1) == 0x3a /* ':' */ )) {
									match(':' /* charlit */ );
								}
								else {
								}
								
								}
								if ( inputState->guessing==0 ) {
#line 113 "wb_wbllexer.g"
									_ttype = VOLUME;
#line 1101 "wb_wbllexer.cpp"
								}
							}
							else {
								bool synPredMatched43 = false;
								if (((LA(1) == 0x45 /* 'E' */ ) && (LA(2) == 0x6e /* 'n' */ ))) {
									int _m43 = mark();
									synPredMatched43 = true;
									inputState->guessing++;
									try {
										{
										match("EndVolume");
										mWS(false);
										}
									}
									catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
										synPredMatched43 = false;
									}
									rewind(_m43);
									inputState->guessing--;
								}
								if ( synPredMatched43 ) {
									match("EndVolume");
									_saveIndex = text.length();
									mWS(false);
									text.erase(_saveIndex);
									if ( inputState->guessing==0 ) {
#line 115 "wb_wbllexer.g"
										_ttype = ENDVOLUME;
#line 1130 "wb_wbllexer.cpp"
									}
								}
								else {
									bool synPredMatched45 = false;
									if (((LA(1) == 0x42 /* 'B' */ ) && (LA(2) == 0x75 /* 'u' */ ))) {
										int _m45 = mark();
										synPredMatched45 = true;
										inputState->guessing++;
										try {
											{
											match("Buffer");
											mWS(false);
											}
										}
										catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
											synPredMatched45 = false;
										}
										rewind(_m45);
										inputState->guessing--;
									}
									if ( synPredMatched45 ) {
										_saveIndex = text.length();
										match("Buffer");
										text.erase(_saveIndex);
										_saveIndex = text.length();
										mWS(false);
										text.erase(_saveIndex);
										{ // ( ... )+
										int _cnt_buname=0;
										for (;;) {
											switch ( LA(1)) {
											case 0x61 /* 'a' */ :
											case 0x62 /* 'b' */ :
											case 0x63 /* 'c' */ :
											case 0x64 /* 'd' */ :
											case 0x65 /* 'e' */ :
											case 0x66 /* 'f' */ :
											case 0x67 /* 'g' */ :
											case 0x68 /* 'h' */ :
											case 0x69 /* 'i' */ :
											case 0x6a /* 'j' */ :
											case 0x6b /* 'k' */ :
											case 0x6c /* 'l' */ :
											case 0x6d /* 'm' */ :
											case 0x6e /* 'n' */ :
											case 0x6f /* 'o' */ :
											case 0x70 /* 'p' */ :
											case 0x71 /* 'q' */ :
											case 0x72 /* 'r' */ :
											case 0x73 /* 's' */ :
											case 0x74 /* 't' */ :
											case 0x75 /* 'u' */ :
											case 0x76 /* 'v' */ :
											case 0x77 /* 'w' */ :
											case 0x78 /* 'x' */ :
											case 0x79 /* 'y' */ :
											case 0x7a /* 'z' */ :
											{
												matchRange('a','z');
												break;
											}
											case 0x41 /* 'A' */ :
											case 0x42 /* 'B' */ :
											case 0x43 /* 'C' */ :
											case 0x44 /* 'D' */ :
											case 0x45 /* 'E' */ :
											case 0x46 /* 'F' */ :
											case 0x47 /* 'G' */ :
											case 0x48 /* 'H' */ :
											case 0x49 /* 'I' */ :
											case 0x4a /* 'J' */ :
											case 0x4b /* 'K' */ :
											case 0x4c /* 'L' */ :
											case 0x4d /* 'M' */ :
											case 0x4e /* 'N' */ :
											case 0x4f /* 'O' */ :
											case 0x50 /* 'P' */ :
											case 0x51 /* 'Q' */ :
											case 0x52 /* 'R' */ :
											case 0x53 /* 'S' */ :
											case 0x54 /* 'T' */ :
											case 0x55 /* 'U' */ :
											case 0x56 /* 'V' */ :
											case 0x57 /* 'W' */ :
											case 0x58 /* 'X' */ :
											case 0x59 /* 'Y' */ :
											case 0x5a /* 'Z' */ :
											{
												matchRange('A','Z');
												break;
											}
											case 0x30 /* '0' */ :
											case 0x31 /* '1' */ :
											case 0x32 /* '2' */ :
											case 0x33 /* '3' */ :
											case 0x34 /* '4' */ :
											case 0x35 /* '5' */ :
											case 0x36 /* '6' */ :
											case 0x37 /* '7' */ :
											case 0x38 /* '8' */ :
											case 0x39 /* '9' */ :
											{
												matchRange('0','9');
												break;
											}
											case 0x24 /* '$' */ :
											{
												match('$' /* charlit */ );
												break;
											}
											case 0x5f /* '_' */ :
											{
												match('_' /* charlit */ );
												break;
											}
											case 0xc4:
											case 0xc5:
											case 0xd6:
											case 0xe4:
											case 0xe5:
											case 0xf6:
											{
												mSWEC(false);
												break;
											}
											default:
											{
												if ( _cnt_buname>=1 ) { goto buname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
											}
											}
											_cnt_buname++;
										}
										buname:;
										}  // ( ... )+
										if ( inputState->guessing==0 ) {
#line 117 "wb_wbllexer.g"
											_ttype = BUFFER;
#line 1268 "wb_wbllexer.cpp"
										}
									}
									else {
										bool synPredMatched49 = false;
										if (((LA(1) == 0x45 /* 'E' */ ) && (LA(2) == 0x6e /* 'n' */ ))) {
											int _m49 = mark();
											synPredMatched49 = true;
											inputState->guessing++;
											try {
												{
												match("EndBuffer");
												mWS(false);
												}
											}
											catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
												synPredMatched49 = false;
											}
											rewind(_m49);
											inputState->guessing--;
										}
										if ( synPredMatched49 ) {
											match("EndBuffer");
											_saveIndex = text.length();
											mWS(false);
											text.erase(_saveIndex);
											if ( inputState->guessing==0 ) {
#line 119 "wb_wbllexer.g"
												_ttype = ENDBUFFER;
#line 1297 "wb_wbllexer.cpp"
											}
										}
										else {
											bool synPredMatched51 = false;
											if (((LA(1) == 0x42 /* 'B' */ ) && (LA(2) == 0x6f /* 'o' */ ))) {
												int _m51 = mark();
												synPredMatched51 = true;
												inputState->guessing++;
												try {
													{
													match("Body");
													mWS(false);
													}
												}
												catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
													synPredMatched51 = false;
												}
												rewind(_m51);
												inputState->guessing--;
											}
											if ( synPredMatched51 ) {
												_saveIndex = text.length();
												match("Body");
												text.erase(_saveIndex);
												_saveIndex = text.length();
												mWS(false);
												text.erase(_saveIndex);
												{ // ( ... )+
												int _cnt_boname=0;
												for (;;) {
													switch ( LA(1)) {
													case 0x61 /* 'a' */ :
													case 0x62 /* 'b' */ :
													case 0x63 /* 'c' */ :
													case 0x64 /* 'd' */ :
													case 0x65 /* 'e' */ :
													case 0x66 /* 'f' */ :
													case 0x67 /* 'g' */ :
													case 0x68 /* 'h' */ :
													case 0x69 /* 'i' */ :
													case 0x6a /* 'j' */ :
													case 0x6b /* 'k' */ :
													case 0x6c /* 'l' */ :
													case 0x6d /* 'm' */ :
													case 0x6e /* 'n' */ :
													case 0x6f /* 'o' */ :
													case 0x70 /* 'p' */ :
													case 0x71 /* 'q' */ :
													case 0x72 /* 'r' */ :
													case 0x73 /* 's' */ :
													case 0x74 /* 't' */ :
													case 0x75 /* 'u' */ :
													case 0x76 /* 'v' */ :
													case 0x77 /* 'w' */ :
													case 0x78 /* 'x' */ :
													case 0x79 /* 'y' */ :
													case 0x7a /* 'z' */ :
													{
														matchRange('a','z');
														break;
													}
													case 0x41 /* 'A' */ :
													case 0x42 /* 'B' */ :
													case 0x43 /* 'C' */ :
													case 0x44 /* 'D' */ :
													case 0x45 /* 'E' */ :
													case 0x46 /* 'F' */ :
													case 0x47 /* 'G' */ :
													case 0x48 /* 'H' */ :
													case 0x49 /* 'I' */ :
													case 0x4a /* 'J' */ :
													case 0x4b /* 'K' */ :
													case 0x4c /* 'L' */ :
													case 0x4d /* 'M' */ :
													case 0x4e /* 'N' */ :
													case 0x4f /* 'O' */ :
													case 0x50 /* 'P' */ :
													case 0x51 /* 'Q' */ :
													case 0x52 /* 'R' */ :
													case 0x53 /* 'S' */ :
													case 0x54 /* 'T' */ :
													case 0x55 /* 'U' */ :
													case 0x56 /* 'V' */ :
													case 0x57 /* 'W' */ :
													case 0x58 /* 'X' */ :
													case 0x59 /* 'Y' */ :
													case 0x5a /* 'Z' */ :
													{
														matchRange('A','Z');
														break;
													}
													case 0x30 /* '0' */ :
													case 0x31 /* '1' */ :
													case 0x32 /* '2' */ :
													case 0x33 /* '3' */ :
													case 0x34 /* '4' */ :
													case 0x35 /* '5' */ :
													case 0x36 /* '6' */ :
													case 0x37 /* '7' */ :
													case 0x38 /* '8' */ :
													case 0x39 /* '9' */ :
													{
														matchRange('0','9');
														break;
													}
													case 0x24 /* '$' */ :
													{
														match('$' /* charlit */ );
														break;
													}
													case 0x5f /* '_' */ :
													{
														match('_' /* charlit */ );
														break;
													}
													case 0xc4:
													case 0xc5:
													case 0xd6:
													case 0xe4:
													case 0xe5:
													case 0xf6:
													{
														mSWEC(false);
														break;
													}
													default:
													{
														if ( _cnt_boname>=1 ) { goto boname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
													}
													}
													_cnt_boname++;
												}
												boname:;
												}  // ( ... )+
												if ( inputState->guessing==0 ) {
#line 121 "wb_wbllexer.g"
													_ttype = BODY;
#line 1435 "wb_wbllexer.cpp"
												}
											}
											else {
												bool synPredMatched55 = false;
												if (((LA(1) == 0x45 /* 'E' */ ) && (LA(2) == 0x6e /* 'n' */ ))) {
													int _m55 = mark();
													synPredMatched55 = true;
													inputState->guessing++;
													try {
														{
														match("EndBody");
														mWS(false);
														}
													}
													catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
														synPredMatched55 = false;
													}
													rewind(_m55);
													inputState->guessing--;
												}
												if ( synPredMatched55 ) {
													match("EndBody");
													_saveIndex = text.length();
													mWS(false);
													text.erase(_saveIndex);
													if ( inputState->guessing==0 ) {
#line 123 "wb_wbllexer.g"
														_ttype = ENDBODY;
#line 1464 "wb_wbllexer.cpp"
													}
												}
												else {
													bool synPredMatched57 = false;
													if (((LA(1) == 0x41 /* 'A' */ ) && (LA(2) == 0x74 /* 't' */ ))) {
														int _m57 = mark();
														synPredMatched57 = true;
														inputState->guessing++;
														try {
															{
															match("Attr");
															mWS(false);
															}
														}
														catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
															synPredMatched57 = false;
														}
														rewind(_m57);
														inputState->guessing--;
													}
													if ( synPredMatched57 ) {
														_saveIndex = text.length();
														match("Attr");
														text.erase(_saveIndex);
														_saveIndex = text.length();
														mWS(false);
														text.erase(_saveIndex);
														{ // ( ... )+
														int _cnt_aname=0;
														for (;;) {
															switch ( LA(1)) {
															case 0x61 /* 'a' */ :
															case 0x62 /* 'b' */ :
															case 0x63 /* 'c' */ :
															case 0x64 /* 'd' */ :
															case 0x65 /* 'e' */ :
															case 0x66 /* 'f' */ :
															case 0x67 /* 'g' */ :
															case 0x68 /* 'h' */ :
															case 0x69 /* 'i' */ :
															case 0x6a /* 'j' */ :
															case 0x6b /* 'k' */ :
															case 0x6c /* 'l' */ :
															case 0x6d /* 'm' */ :
															case 0x6e /* 'n' */ :
															case 0x6f /* 'o' */ :
															case 0x70 /* 'p' */ :
															case 0x71 /* 'q' */ :
															case 0x72 /* 'r' */ :
															case 0x73 /* 's' */ :
															case 0x74 /* 't' */ :
															case 0x75 /* 'u' */ :
															case 0x76 /* 'v' */ :
															case 0x77 /* 'w' */ :
															case 0x78 /* 'x' */ :
															case 0x79 /* 'y' */ :
															case 0x7a /* 'z' */ :
															{
																matchRange('a','z');
																break;
															}
															case 0x41 /* 'A' */ :
															case 0x42 /* 'B' */ :
															case 0x43 /* 'C' */ :
															case 0x44 /* 'D' */ :
															case 0x45 /* 'E' */ :
															case 0x46 /* 'F' */ :
															case 0x47 /* 'G' */ :
															case 0x48 /* 'H' */ :
															case 0x49 /* 'I' */ :
															case 0x4a /* 'J' */ :
															case 0x4b /* 'K' */ :
															case 0x4c /* 'L' */ :
															case 0x4d /* 'M' */ :
															case 0x4e /* 'N' */ :
															case 0x4f /* 'O' */ :
															case 0x50 /* 'P' */ :
															case 0x51 /* 'Q' */ :
															case 0x52 /* 'R' */ :
															case 0x53 /* 'S' */ :
															case 0x54 /* 'T' */ :
															case 0x55 /* 'U' */ :
															case 0x56 /* 'V' */ :
															case 0x57 /* 'W' */ :
															case 0x58 /* 'X' */ :
															case 0x59 /* 'Y' */ :
															case 0x5a /* 'Z' */ :
															{
																matchRange('A','Z');
																break;
															}
															case 0x30 /* '0' */ :
															case 0x31 /* '1' */ :
															case 0x32 /* '2' */ :
															case 0x33 /* '3' */ :
															case 0x34 /* '4' */ :
															case 0x35 /* '5' */ :
															case 0x36 /* '6' */ :
															case 0x37 /* '7' */ :
															case 0x38 /* '8' */ :
															case 0x39 /* '9' */ :
															{
																matchRange('0','9');
																break;
															}
															case 0x24 /* '$' */ :
															{
																match('$' /* charlit */ );
																break;
															}
															case 0x5f /* '_' */ :
															{
																match('_' /* charlit */ );
																break;
															}
															case 0xc4:
															case 0xc5:
															case 0xd6:
															case 0xe4:
															case 0xe5:
															case 0xf6:
															{
																mSWEC(false);
																break;
															}
															case 0x5b /* '[' */ :
															{
																match('[' /* charlit */ );
																break;
															}
															case 0x5d /* ']' */ :
															{
																match(']' /* charlit */ );
																break;
															}
															default:
															{
																if ( _cnt_aname>=1 ) { goto aname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
															}
															}
															_cnt_aname++;
														}
														aname:;
														}  // ( ... )+
														{ // ( ... )*
														for (;;) {
															if ((LA(1) == 0x2e /* '.' */ )) {
																match('.' /* charlit */ );
																{ // ( ... )+
																int _cnt62=0;
																for (;;) {
																	switch ( LA(1)) {
																	case 0x61 /* 'a' */ :
																	case 0x62 /* 'b' */ :
																	case 0x63 /* 'c' */ :
																	case 0x64 /* 'd' */ :
																	case 0x65 /* 'e' */ :
																	case 0x66 /* 'f' */ :
																	case 0x67 /* 'g' */ :
																	case 0x68 /* 'h' */ :
																	case 0x69 /* 'i' */ :
																	case 0x6a /* 'j' */ :
																	case 0x6b /* 'k' */ :
																	case 0x6c /* 'l' */ :
																	case 0x6d /* 'm' */ :
																	case 0x6e /* 'n' */ :
																	case 0x6f /* 'o' */ :
																	case 0x70 /* 'p' */ :
																	case 0x71 /* 'q' */ :
																	case 0x72 /* 'r' */ :
																	case 0x73 /* 's' */ :
																	case 0x74 /* 't' */ :
																	case 0x75 /* 'u' */ :
																	case 0x76 /* 'v' */ :
																	case 0x77 /* 'w' */ :
																	case 0x78 /* 'x' */ :
																	case 0x79 /* 'y' */ :
																	case 0x7a /* 'z' */ :
																	{
																		matchRange('a','z');
																		break;
																	}
																	case 0x41 /* 'A' */ :
																	case 0x42 /* 'B' */ :
																	case 0x43 /* 'C' */ :
																	case 0x44 /* 'D' */ :
																	case 0x45 /* 'E' */ :
																	case 0x46 /* 'F' */ :
																	case 0x47 /* 'G' */ :
																	case 0x48 /* 'H' */ :
																	case 0x49 /* 'I' */ :
																	case 0x4a /* 'J' */ :
																	case 0x4b /* 'K' */ :
																	case 0x4c /* 'L' */ :
																	case 0x4d /* 'M' */ :
																	case 0x4e /* 'N' */ :
																	case 0x4f /* 'O' */ :
																	case 0x50 /* 'P' */ :
																	case 0x51 /* 'Q' */ :
																	case 0x52 /* 'R' */ :
																	case 0x53 /* 'S' */ :
																	case 0x54 /* 'T' */ :
																	case 0x55 /* 'U' */ :
																	case 0x56 /* 'V' */ :
																	case 0x57 /* 'W' */ :
																	case 0x58 /* 'X' */ :
																	case 0x59 /* 'Y' */ :
																	case 0x5a /* 'Z' */ :
																	{
																		matchRange('A','Z');
																		break;
																	}
																	case 0x30 /* '0' */ :
																	case 0x31 /* '1' */ :
																	case 0x32 /* '2' */ :
																	case 0x33 /* '3' */ :
																	case 0x34 /* '4' */ :
																	case 0x35 /* '5' */ :
																	case 0x36 /* '6' */ :
																	case 0x37 /* '7' */ :
																	case 0x38 /* '8' */ :
																	case 0x39 /* '9' */ :
																	{
																		matchRange('0','9');
																		break;
																	}
																	case 0x24 /* '$' */ :
																	{
																		match('$' /* charlit */ );
																		break;
																	}
																	case 0x5f /* '_' */ :
																	{
																		match('_' /* charlit */ );
																		break;
																	}
																	case 0xc4:
																	case 0xc5:
																	case 0xd6:
																	case 0xe4:
																	case 0xe5:
																	case 0xf6:
																	{
																		mSWEC(false);
																		break;
																	}
																	case 0x5b /* '[' */ :
																	{
																		match('[' /* charlit */ );
																		break;
																	}
																	case 0x5d /* ']' */ :
																	{
																		match(']' /* charlit */ );
																		break;
																	}
																	default:
																	{
																		if ( _cnt62>=1 ) { goto _loop62; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
																	}
																	}
																	_cnt62++;
																}
																_loop62:;
																}  // ( ... )+
															}
															else {
																goto _loop63;
															}
															
														}
														_loop63:;
														} // ( ... )*
														if ( inputState->guessing==0 ) {
#line 125 "wb_wbllexer.g"
															_ttype = ATTRIBUTE;
#line 1741 "wb_wbllexer.cpp"
														}
													}
													else {
														bool synPredMatched65 = false;
														if ((((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ )) && (_tokenSet_2.member(LA(2))))) {
															int _m65 = mark();
															synPredMatched65 = true;
															inputState->guessing++;
															try {
																{
																mDIGITS(false);
																match('.' /* charlit */ );
																mDIGITS(false);
																match('.' /* charlit */ );
																}
															}
															catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																synPredMatched65 = false;
															}
															rewind(_m65);
															inputState->guessing--;
														}
														if ( synPredMatched65 ) {
															mDIGITS(false);
															match('.' /* charlit */ );
															mDIGITS(false);
															match('.' /* charlit */ );
															mDIGITS(false);
															match('.' /* charlit */ );
															mDIGITS(false);
															{
															if ((LA(1) == 0x3a /* ':' */ )) {
																match(':' /* charlit */ );
																mDIGITS(false);
															}
															else {
															}
															
															}
															if ( inputState->guessing==0 ) {
#line 127 "wb_wbllexer.g"
																_ttype = OID;
#line 1784 "wb_wbllexer.cpp"
															}
														}
														else {
															bool synPredMatched73 = false;
															if ((((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ )) && ((LA(2) >= 0x30 /* '0' */  && LA(2) <= 0x39 /* '9' */ )))) {
																int _m73 = mark();
																synPredMatched73 = true;
																inputState->guessing++;
																try {
																	{
																	{
																	matchRange('0','9');
																	}
																	{
																	matchRange('0','9');
																	}
																	match('-' /* charlit */ );
																	{
																	switch ( LA(1)) {
																	case 0x61 /* 'a' */ :
																	case 0x62 /* 'b' */ :
																	case 0x63 /* 'c' */ :
																	case 0x64 /* 'd' */ :
																	case 0x65 /* 'e' */ :
																	case 0x66 /* 'f' */ :
																	case 0x67 /* 'g' */ :
																	case 0x68 /* 'h' */ :
																	case 0x69 /* 'i' */ :
																	case 0x6a /* 'j' */ :
																	case 0x6b /* 'k' */ :
																	case 0x6c /* 'l' */ :
																	case 0x6d /* 'm' */ :
																	case 0x6e /* 'n' */ :
																	case 0x6f /* 'o' */ :
																	case 0x70 /* 'p' */ :
																	case 0x71 /* 'q' */ :
																	case 0x72 /* 'r' */ :
																	case 0x73 /* 's' */ :
																	case 0x74 /* 't' */ :
																	case 0x75 /* 'u' */ :
																	case 0x76 /* 'v' */ :
																	case 0x77 /* 'w' */ :
																	case 0x78 /* 'x' */ :
																	case 0x79 /* 'y' */ :
																	case 0x7a /* 'z' */ :
																	{
																		matchRange('a','z');
																		break;
																	}
																	case 0x41 /* 'A' */ :
																	case 0x42 /* 'B' */ :
																	case 0x43 /* 'C' */ :
																	case 0x44 /* 'D' */ :
																	case 0x45 /* 'E' */ :
																	case 0x46 /* 'F' */ :
																	case 0x47 /* 'G' */ :
																	case 0x48 /* 'H' */ :
																	case 0x49 /* 'I' */ :
																	case 0x4a /* 'J' */ :
																	case 0x4b /* 'K' */ :
																	case 0x4c /* 'L' */ :
																	case 0x4d /* 'M' */ :
																	case 0x4e /* 'N' */ :
																	case 0x4f /* 'O' */ :
																	case 0x50 /* 'P' */ :
																	case 0x51 /* 'Q' */ :
																	case 0x52 /* 'R' */ :
																	case 0x53 /* 'S' */ :
																	case 0x54 /* 'T' */ :
																	case 0x55 /* 'U' */ :
																	case 0x56 /* 'V' */ :
																	case 0x57 /* 'W' */ :
																	case 0x58 /* 'X' */ :
																	case 0x59 /* 'Y' */ :
																	case 0x5a /* 'Z' */ :
																	{
																		matchRange('A','Z');
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																	}
																	}
																	}
																	{
																	switch ( LA(1)) {
																	case 0x61 /* 'a' */ :
																	case 0x62 /* 'b' */ :
																	case 0x63 /* 'c' */ :
																	case 0x64 /* 'd' */ :
																	case 0x65 /* 'e' */ :
																	case 0x66 /* 'f' */ :
																	case 0x67 /* 'g' */ :
																	case 0x68 /* 'h' */ :
																	case 0x69 /* 'i' */ :
																	case 0x6a /* 'j' */ :
																	case 0x6b /* 'k' */ :
																	case 0x6c /* 'l' */ :
																	case 0x6d /* 'm' */ :
																	case 0x6e /* 'n' */ :
																	case 0x6f /* 'o' */ :
																	case 0x70 /* 'p' */ :
																	case 0x71 /* 'q' */ :
																	case 0x72 /* 'r' */ :
																	case 0x73 /* 's' */ :
																	case 0x74 /* 't' */ :
																	case 0x75 /* 'u' */ :
																	case 0x76 /* 'v' */ :
																	case 0x77 /* 'w' */ :
																	case 0x78 /* 'x' */ :
																	case 0x79 /* 'y' */ :
																	case 0x7a /* 'z' */ :
																	{
																		matchRange('a','z');
																		break;
																	}
																	case 0x41 /* 'A' */ :
																	case 0x42 /* 'B' */ :
																	case 0x43 /* 'C' */ :
																	case 0x44 /* 'D' */ :
																	case 0x45 /* 'E' */ :
																	case 0x46 /* 'F' */ :
																	case 0x47 /* 'G' */ :
																	case 0x48 /* 'H' */ :
																	case 0x49 /* 'I' */ :
																	case 0x4a /* 'J' */ :
																	case 0x4b /* 'K' */ :
																	case 0x4c /* 'L' */ :
																	case 0x4d /* 'M' */ :
																	case 0x4e /* 'N' */ :
																	case 0x4f /* 'O' */ :
																	case 0x50 /* 'P' */ :
																	case 0x51 /* 'Q' */ :
																	case 0x52 /* 'R' */ :
																	case 0x53 /* 'S' */ :
																	case 0x54 /* 'T' */ :
																	case 0x55 /* 'U' */ :
																	case 0x56 /* 'V' */ :
																	case 0x57 /* 'W' */ :
																	case 0x58 /* 'X' */ :
																	case 0x59 /* 'Y' */ :
																	case 0x5a /* 'Z' */ :
																	{
																		matchRange('A','Z');
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																	}
																	}
																	}
																	{
																	switch ( LA(1)) {
																	case 0x61 /* 'a' */ :
																	case 0x62 /* 'b' */ :
																	case 0x63 /* 'c' */ :
																	case 0x64 /* 'd' */ :
																	case 0x65 /* 'e' */ :
																	case 0x66 /* 'f' */ :
																	case 0x67 /* 'g' */ :
																	case 0x68 /* 'h' */ :
																	case 0x69 /* 'i' */ :
																	case 0x6a /* 'j' */ :
																	case 0x6b /* 'k' */ :
																	case 0x6c /* 'l' */ :
																	case 0x6d /* 'm' */ :
																	case 0x6e /* 'n' */ :
																	case 0x6f /* 'o' */ :
																	case 0x70 /* 'p' */ :
																	case 0x71 /* 'q' */ :
																	case 0x72 /* 'r' */ :
																	case 0x73 /* 's' */ :
																	case 0x74 /* 't' */ :
																	case 0x75 /* 'u' */ :
																	case 0x76 /* 'v' */ :
																	case 0x77 /* 'w' */ :
																	case 0x78 /* 'x' */ :
																	case 0x79 /* 'y' */ :
																	case 0x7a /* 'z' */ :
																	{
																		matchRange('a','z');
																		break;
																	}
																	case 0x41 /* 'A' */ :
																	case 0x42 /* 'B' */ :
																	case 0x43 /* 'C' */ :
																	case 0x44 /* 'D' */ :
																	case 0x45 /* 'E' */ :
																	case 0x46 /* 'F' */ :
																	case 0x47 /* 'G' */ :
																	case 0x48 /* 'H' */ :
																	case 0x49 /* 'I' */ :
																	case 0x4a /* 'J' */ :
																	case 0x4b /* 'K' */ :
																	case 0x4c /* 'L' */ :
																	case 0x4d /* 'M' */ :
																	case 0x4e /* 'N' */ :
																	case 0x4f /* 'O' */ :
																	case 0x50 /* 'P' */ :
																	case 0x51 /* 'Q' */ :
																	case 0x52 /* 'R' */ :
																	case 0x53 /* 'S' */ :
																	case 0x54 /* 'T' */ :
																	case 0x55 /* 'U' */ :
																	case 0x56 /* 'V' */ :
																	case 0x57 /* 'W' */ :
																	case 0x58 /* 'X' */ :
																	case 0x59 /* 'Y' */ :
																	case 0x5a /* 'Z' */ :
																	{
																		matchRange('A','Z');
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																	}
																	}
																	}
																	match('-' /* charlit */ );
																	}
																}
																catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																	synPredMatched73 = false;
																}
																rewind(_m73);
																inputState->guessing--;
															}
															if ( synPredMatched73 ) {
																{
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																match('-' /* charlit */ );
																{
																switch ( LA(1)) {
																case 0x61 /* 'a' */ :
																case 0x62 /* 'b' */ :
																case 0x63 /* 'c' */ :
																case 0x64 /* 'd' */ :
																case 0x65 /* 'e' */ :
																case 0x66 /* 'f' */ :
																case 0x67 /* 'g' */ :
																case 0x68 /* 'h' */ :
																case 0x69 /* 'i' */ :
																case 0x6a /* 'j' */ :
																case 0x6b /* 'k' */ :
																case 0x6c /* 'l' */ :
																case 0x6d /* 'm' */ :
																case 0x6e /* 'n' */ :
																case 0x6f /* 'o' */ :
																case 0x70 /* 'p' */ :
																case 0x71 /* 'q' */ :
																case 0x72 /* 'r' */ :
																case 0x73 /* 's' */ :
																case 0x74 /* 't' */ :
																case 0x75 /* 'u' */ :
																case 0x76 /* 'v' */ :
																case 0x77 /* 'w' */ :
																case 0x78 /* 'x' */ :
																case 0x79 /* 'y' */ :
																case 0x7a /* 'z' */ :
																{
																	matchRange('a','z');
																	break;
																}
																case 0x41 /* 'A' */ :
																case 0x42 /* 'B' */ :
																case 0x43 /* 'C' */ :
																case 0x44 /* 'D' */ :
																case 0x45 /* 'E' */ :
																case 0x46 /* 'F' */ :
																case 0x47 /* 'G' */ :
																case 0x48 /* 'H' */ :
																case 0x49 /* 'I' */ :
																case 0x4a /* 'J' */ :
																case 0x4b /* 'K' */ :
																case 0x4c /* 'L' */ :
																case 0x4d /* 'M' */ :
																case 0x4e /* 'N' */ :
																case 0x4f /* 'O' */ :
																case 0x50 /* 'P' */ :
																case 0x51 /* 'Q' */ :
																case 0x52 /* 'R' */ :
																case 0x53 /* 'S' */ :
																case 0x54 /* 'T' */ :
																case 0x55 /* 'U' */ :
																case 0x56 /* 'V' */ :
																case 0x57 /* 'W' */ :
																case 0x58 /* 'X' */ :
																case 0x59 /* 'Y' */ :
																case 0x5a /* 'Z' */ :
																{
																	matchRange('A','Z');
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																}
																}
																}
																{
																switch ( LA(1)) {
																case 0x61 /* 'a' */ :
																case 0x62 /* 'b' */ :
																case 0x63 /* 'c' */ :
																case 0x64 /* 'd' */ :
																case 0x65 /* 'e' */ :
																case 0x66 /* 'f' */ :
																case 0x67 /* 'g' */ :
																case 0x68 /* 'h' */ :
																case 0x69 /* 'i' */ :
																case 0x6a /* 'j' */ :
																case 0x6b /* 'k' */ :
																case 0x6c /* 'l' */ :
																case 0x6d /* 'm' */ :
																case 0x6e /* 'n' */ :
																case 0x6f /* 'o' */ :
																case 0x70 /* 'p' */ :
																case 0x71 /* 'q' */ :
																case 0x72 /* 'r' */ :
																case 0x73 /* 's' */ :
																case 0x74 /* 't' */ :
																case 0x75 /* 'u' */ :
																case 0x76 /* 'v' */ :
																case 0x77 /* 'w' */ :
																case 0x78 /* 'x' */ :
																case 0x79 /* 'y' */ :
																case 0x7a /* 'z' */ :
																{
																	matchRange('a','z');
																	break;
																}
																case 0x41 /* 'A' */ :
																case 0x42 /* 'B' */ :
																case 0x43 /* 'C' */ :
																case 0x44 /* 'D' */ :
																case 0x45 /* 'E' */ :
																case 0x46 /* 'F' */ :
																case 0x47 /* 'G' */ :
																case 0x48 /* 'H' */ :
																case 0x49 /* 'I' */ :
																case 0x4a /* 'J' */ :
																case 0x4b /* 'K' */ :
																case 0x4c /* 'L' */ :
																case 0x4d /* 'M' */ :
																case 0x4e /* 'N' */ :
																case 0x4f /* 'O' */ :
																case 0x50 /* 'P' */ :
																case 0x51 /* 'Q' */ :
																case 0x52 /* 'R' */ :
																case 0x53 /* 'S' */ :
																case 0x54 /* 'T' */ :
																case 0x55 /* 'U' */ :
																case 0x56 /* 'V' */ :
																case 0x57 /* 'W' */ :
																case 0x58 /* 'X' */ :
																case 0x59 /* 'Y' */ :
																case 0x5a /* 'Z' */ :
																{
																	matchRange('A','Z');
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																}
																}
																}
																{
																switch ( LA(1)) {
																case 0x61 /* 'a' */ :
																case 0x62 /* 'b' */ :
																case 0x63 /* 'c' */ :
																case 0x64 /* 'd' */ :
																case 0x65 /* 'e' */ :
																case 0x66 /* 'f' */ :
																case 0x67 /* 'g' */ :
																case 0x68 /* 'h' */ :
																case 0x69 /* 'i' */ :
																case 0x6a /* 'j' */ :
																case 0x6b /* 'k' */ :
																case 0x6c /* 'l' */ :
																case 0x6d /* 'm' */ :
																case 0x6e /* 'n' */ :
																case 0x6f /* 'o' */ :
																case 0x70 /* 'p' */ :
																case 0x71 /* 'q' */ :
																case 0x72 /* 'r' */ :
																case 0x73 /* 's' */ :
																case 0x74 /* 't' */ :
																case 0x75 /* 'u' */ :
																case 0x76 /* 'v' */ :
																case 0x77 /* 'w' */ :
																case 0x78 /* 'x' */ :
																case 0x79 /* 'y' */ :
																case 0x7a /* 'z' */ :
																{
																	matchRange('a','z');
																	break;
																}
																case 0x41 /* 'A' */ :
																case 0x42 /* 'B' */ :
																case 0x43 /* 'C' */ :
																case 0x44 /* 'D' */ :
																case 0x45 /* 'E' */ :
																case 0x46 /* 'F' */ :
																case 0x47 /* 'G' */ :
																case 0x48 /* 'H' */ :
																case 0x49 /* 'I' */ :
																case 0x4a /* 'J' */ :
																case 0x4b /* 'K' */ :
																case 0x4c /* 'L' */ :
																case 0x4d /* 'M' */ :
																case 0x4e /* 'N' */ :
																case 0x4f /* 'O' */ :
																case 0x50 /* 'P' */ :
																case 0x51 /* 'Q' */ :
																case 0x52 /* 'R' */ :
																case 0x53 /* 'S' */ :
																case 0x54 /* 'T' */ :
																case 0x55 /* 'U' */ :
																case 0x56 /* 'V' */ :
																case 0x57 /* 'W' */ :
																case 0x58 /* 'X' */ :
																case 0x59 /* 'Y' */ :
																case 0x5a /* 'Z' */ :
																{
																	matchRange('A','Z');
																	break;
																}
																default:
																{
																	throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																}
																}
																}
																match('-' /* charlit */ );
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																match(' ' /* charlit */ );
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																match(':' /* charlit */ );
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																match(':' /* charlit */ );
																{
																matchRange('0','9');
																}
																{
																matchRange('0','9');
																}
																{
																if ((LA(1) == 0x2e /* '.' */ )) {
																	match('.' /* charlit */ );
																	{
																	matchRange('0','9');
																	}
																	{
																	matchRange('0','9');
																	}
																}
																else {
																}
																
																}
																}
																if ( inputState->guessing==0 ) {
#line 129 "wb_wbllexer.g"
																	_ttype = ASC_TIME;
#line 2280 "wb_wbllexer.cpp"
																}
															}
															else {
																bool synPredMatched94 = false;
																if ((((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ )) && (_tokenSet_2.member(LA(2))))) {
																	int _m94 = mark();
																	synPredMatched94 = true;
																	inputState->guessing++;
																	try {
																		{
																		mDIGITS(false);
																		match('.' /* charlit */ );
																		}
																	}
																	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																		synPredMatched94 = false;
																	}
																	rewind(_m94);
																	inputState->guessing--;
																}
																if ( synPredMatched94 ) {
																	mDIGITS(false);
																	match('.' /* charlit */ );
																	{ // ( ... )*
																	for (;;) {
																		if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
																			matchRange('0','9');
																		}
																		else {
																			goto _loop96;
																		}
																		
																	}
																	_loop96:;
																	} // ( ... )*
																	{
																	if ((LA(1) == 0x45 /* 'E' */  || LA(1) == 0x65 /* 'e' */ )) {
																		{
																		switch ( LA(1)) {
																		case 0x65 /* 'e' */ :
																		{
																			match('e' /* charlit */ );
																			break;
																		}
																		case 0x45 /* 'E' */ :
																		{
																			match('E' /* charlit */ );
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																		}
																		}
																		}
																		{
																		switch ( LA(1)) {
																		case 0x2b /* '+' */ :
																		{
																			match('+' /* charlit */ );
																			break;
																		}
																		case 0x2d /* '-' */ :
																		{
																			match('-' /* charlit */ );
																			break;
																		}
																		case 0x30 /* '0' */ :
																		case 0x31 /* '1' */ :
																		case 0x32 /* '2' */ :
																		case 0x33 /* '3' */ :
																		case 0x34 /* '4' */ :
																		case 0x35 /* '5' */ :
																		case 0x36 /* '6' */ :
																		case 0x37 /* '7' */ :
																		case 0x38 /* '8' */ :
																		case 0x39 /* '9' */ :
																		{
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																		}
																		}
																		}
																		{ // ( ... )+
																		int _cnt101=0;
																		for (;;) {
																			if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
																				matchRange('0','9');
																			}
																			else {
																				if ( _cnt101>=1 ) { goto _loop101; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
																			}
																			
																			_cnt101++;
																		}
																		_loop101:;
																		}  // ( ... )+
																	}
																	else {
																	}
																	
																	}
																	if ( inputState->guessing==0 ) {
#line 131 "wb_wbllexer.g"
																		_ttype = NUM_FLOAT;
#line 2389 "wb_wbllexer.cpp"
																	}
																}
																else {
																	bool synPredMatched103 = false;
																	if (((LA(1) == 0x2d /* '-' */ ) && ((LA(2) >= 0x30 /* '0' */  && LA(2) <= 0x39 /* '9' */ )))) {
																		int _m103 = mark();
																		synPredMatched103 = true;
																		inputState->guessing++;
																		try {
																			{
																			match('-' /* charlit */ );
																			mDIGITS(false);
																			match('.' /* charlit */ );
																			}
																		}
																		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																			synPredMatched103 = false;
																		}
																		rewind(_m103);
																		inputState->guessing--;
																	}
																	if ( synPredMatched103 ) {
																		match('-' /* charlit */ );
																		mDIGITS(false);
																		match('.' /* charlit */ );
																		{ // ( ... )*
																		for (;;) {
																			if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
																				matchRange('0','9');
																			}
																			else {
																				goto _loop105;
																			}
																			
																		}
																		_loop105:;
																		} // ( ... )*
																		{
																		if ((LA(1) == 0x45 /* 'E' */  || LA(1) == 0x65 /* 'e' */ )) {
																			{
																			switch ( LA(1)) {
																			case 0x65 /* 'e' */ :
																			{
																				match('e' /* charlit */ );
																				break;
																			}
																			case 0x45 /* 'E' */ :
																			{
																				match('E' /* charlit */ );
																				break;
																			}
																			default:
																			{
																				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																			}
																			}
																			}
																			{
																			switch ( LA(1)) {
																			case 0x2b /* '+' */ :
																			{
																				match('+' /* charlit */ );
																				break;
																			}
																			case 0x2d /* '-' */ :
																			{
																				match('-' /* charlit */ );
																				break;
																			}
																			case 0x30 /* '0' */ :
																			case 0x31 /* '1' */ :
																			case 0x32 /* '2' */ :
																			case 0x33 /* '3' */ :
																			case 0x34 /* '4' */ :
																			case 0x35 /* '5' */ :
																			case 0x36 /* '6' */ :
																			case 0x37 /* '7' */ :
																			case 0x38 /* '8' */ :
																			case 0x39 /* '9' */ :
																			{
																				break;
																			}
																			default:
																			{
																				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																			}
																			}
																			}
																			{ // ( ... )+
																			int _cnt110=0;
																			for (;;) {
																				if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
																					matchRange('0','9');
																				}
																				else {
																					if ( _cnt110>=1 ) { goto _loop110; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
																				}
																				
																				_cnt110++;
																			}
																			_loop110:;
																			}  // ( ... )+
																		}
																		else {
																		}
																		
																		}
																		if ( inputState->guessing==0 ) {
#line 133 "wb_wbllexer.g"
																			_ttype = NUM_FLOAT;
#line 2500 "wb_wbllexer.cpp"
																		}
																	}
																	else if ((LA(1) == 0x2d /* '-' */ ) && ((LA(2) >= 0x30 /* '0' */  && LA(2) <= 0x39 /* '9' */ ))) {
																		match('-' /* charlit */ );
																		mDIGITS(false);
																		if ( inputState->guessing==0 ) {
#line 139 "wb_wbllexer.g"
																			_ttype = INT;
#line 2509 "wb_wbllexer.cpp"
																		}
																	}
																	else if ((LA(1) == 0x2e /* '.' */ )) {
																		match('.' /* charlit */ );
																		{ // ( ... )+
																		int _cnt112=0;
																		for (;;) {
																			if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
																				matchRange('0','9');
																			}
																			else {
																				if ( _cnt112>=1 ) { goto _loop112; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
																			}
																			
																			_cnt112++;
																		}
																		_loop112:;
																		}  // ( ... )+
																		{
																		if ((LA(1) == 0x45 /* 'E' */  || LA(1) == 0x65 /* 'e' */ )) {
																			{
																			switch ( LA(1)) {
																			case 0x65 /* 'e' */ :
																			{
																				match('e' /* charlit */ );
																				break;
																			}
																			case 0x45 /* 'E' */ :
																			{
																				match('E' /* charlit */ );
																				break;
																			}
																			default:
																			{
																				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																			}
																			}
																			}
																			{
																			switch ( LA(1)) {
																			case 0x2b /* '+' */ :
																			{
																				match('+' /* charlit */ );
																				break;
																			}
																			case 0x2d /* '-' */ :
																			{
																				match('-' /* charlit */ );
																				break;
																			}
																			case 0x30 /* '0' */ :
																			case 0x31 /* '1' */ :
																			case 0x32 /* '2' */ :
																			case 0x33 /* '3' */ :
																			case 0x34 /* '4' */ :
																			case 0x35 /* '5' */ :
																			case 0x36 /* '6' */ :
																			case 0x37 /* '7' */ :
																			case 0x38 /* '8' */ :
																			case 0x39 /* '9' */ :
																			{
																				break;
																			}
																			default:
																			{
																				throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																			}
																			}
																			}
																			{ // ( ... )+
																			int _cnt117=0;
																			for (;;) {
																				if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
																					matchRange('0','9');
																				}
																				else {
																					if ( _cnt117>=1 ) { goto _loop117; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
																				}
																				
																				_cnt117++;
																			}
																			_loop117:;
																			}  // ( ... )+
																		}
																		else {
																		}
																		
																		}
																		if ( inputState->guessing==0 ) {
#line 135 "wb_wbllexer.g"
																			_ttype = NUM_FLOAT;
#line 2601 "wb_wbllexer.cpp"
																		}
																	}
																	else if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ )) && (true)) {
																		mDIGITS(false);
																		if ( inputState->guessing==0 ) {
#line 137 "wb_wbllexer.g"
																			_ttype = INT;
#line 2609 "wb_wbllexer.cpp"
																		}
																	}
																	else if ((_tokenSet_3.member(LA(1))) && (true)) {
																		{
																		switch ( LA(1)) {
																		case 0x61 /* 'a' */ :
																		case 0x62 /* 'b' */ :
																		case 0x63 /* 'c' */ :
																		case 0x64 /* 'd' */ :
																		case 0x65 /* 'e' */ :
																		case 0x66 /* 'f' */ :
																		case 0x67 /* 'g' */ :
																		case 0x68 /* 'h' */ :
																		case 0x69 /* 'i' */ :
																		case 0x6a /* 'j' */ :
																		case 0x6b /* 'k' */ :
																		case 0x6c /* 'l' */ :
																		case 0x6d /* 'm' */ :
																		case 0x6e /* 'n' */ :
																		case 0x6f /* 'o' */ :
																		case 0x70 /* 'p' */ :
																		case 0x71 /* 'q' */ :
																		case 0x72 /* 'r' */ :
																		case 0x73 /* 's' */ :
																		case 0x74 /* 't' */ :
																		case 0x75 /* 'u' */ :
																		case 0x76 /* 'v' */ :
																		case 0x77 /* 'w' */ :
																		case 0x78 /* 'x' */ :
																		case 0x79 /* 'y' */ :
																		case 0x7a /* 'z' */ :
																		{
																			matchRange('a','z');
																			break;
																		}
																		case 0x41 /* 'A' */ :
																		case 0x42 /* 'B' */ :
																		case 0x43 /* 'C' */ :
																		case 0x44 /* 'D' */ :
																		case 0x45 /* 'E' */ :
																		case 0x46 /* 'F' */ :
																		case 0x47 /* 'G' */ :
																		case 0x48 /* 'H' */ :
																		case 0x49 /* 'I' */ :
																		case 0x4a /* 'J' */ :
																		case 0x4b /* 'K' */ :
																		case 0x4c /* 'L' */ :
																		case 0x4d /* 'M' */ :
																		case 0x4e /* 'N' */ :
																		case 0x4f /* 'O' */ :
																		case 0x50 /* 'P' */ :
																		case 0x51 /* 'Q' */ :
																		case 0x52 /* 'R' */ :
																		case 0x53 /* 'S' */ :
																		case 0x54 /* 'T' */ :
																		case 0x55 /* 'U' */ :
																		case 0x56 /* 'V' */ :
																		case 0x57 /* 'W' */ :
																		case 0x58 /* 'X' */ :
																		case 0x59 /* 'Y' */ :
																		case 0x5a /* 'Z' */ :
																		{
																			matchRange('A','Z');
																			break;
																		}
																		case 0x24 /* '$' */ :
																		{
																			match('$' /* charlit */ );
																			break;
																		}
																		case 0x5f /* '_' */ :
																		{
																			match('_' /* charlit */ );
																			break;
																		}
																		case 0xc4:
																		case 0xc5:
																		case 0xd6:
																		case 0xe4:
																		case 0xe5:
																		case 0xf6:
																		{
																			mSWEC(false);
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
																		}
																		}
																		}
																		{ // ( ... )*
																		for (;;) {
																			switch ( LA(1)) {
																			case 0x61 /* 'a' */ :
																			case 0x62 /* 'b' */ :
																			case 0x63 /* 'c' */ :
																			case 0x64 /* 'd' */ :
																			case 0x65 /* 'e' */ :
																			case 0x66 /* 'f' */ :
																			case 0x67 /* 'g' */ :
																			case 0x68 /* 'h' */ :
																			case 0x69 /* 'i' */ :
																			case 0x6a /* 'j' */ :
																			case 0x6b /* 'k' */ :
																			case 0x6c /* 'l' */ :
																			case 0x6d /* 'm' */ :
																			case 0x6e /* 'n' */ :
																			case 0x6f /* 'o' */ :
																			case 0x70 /* 'p' */ :
																			case 0x71 /* 'q' */ :
																			case 0x72 /* 'r' */ :
																			case 0x73 /* 's' */ :
																			case 0x74 /* 't' */ :
																			case 0x75 /* 'u' */ :
																			case 0x76 /* 'v' */ :
																			case 0x77 /* 'w' */ :
																			case 0x78 /* 'x' */ :
																			case 0x79 /* 'y' */ :
																			case 0x7a /* 'z' */ :
																			{
																				matchRange('a','z');
																				break;
																			}
																			case 0x41 /* 'A' */ :
																			case 0x42 /* 'B' */ :
																			case 0x43 /* 'C' */ :
																			case 0x44 /* 'D' */ :
																			case 0x45 /* 'E' */ :
																			case 0x46 /* 'F' */ :
																			case 0x47 /* 'G' */ :
																			case 0x48 /* 'H' */ :
																			case 0x49 /* 'I' */ :
																			case 0x4a /* 'J' */ :
																			case 0x4b /* 'K' */ :
																			case 0x4c /* 'L' */ :
																			case 0x4d /* 'M' */ :
																			case 0x4e /* 'N' */ :
																			case 0x4f /* 'O' */ :
																			case 0x50 /* 'P' */ :
																			case 0x51 /* 'Q' */ :
																			case 0x52 /* 'R' */ :
																			case 0x53 /* 'S' */ :
																			case 0x54 /* 'T' */ :
																			case 0x55 /* 'U' */ :
																			case 0x56 /* 'V' */ :
																			case 0x57 /* 'W' */ :
																			case 0x58 /* 'X' */ :
																			case 0x59 /* 'Y' */ :
																			case 0x5a /* 'Z' */ :
																			{
																				matchRange('A','Z');
																				break;
																			}
																			case 0x30 /* '0' */ :
																			case 0x31 /* '1' */ :
																			case 0x32 /* '2' */ :
																			case 0x33 /* '3' */ :
																			case 0x34 /* '4' */ :
																			case 0x35 /* '5' */ :
																			case 0x36 /* '6' */ :
																			case 0x37 /* '7' */ :
																			case 0x38 /* '8' */ :
																			case 0x39 /* '9' */ :
																			{
																				matchRange('0','9');
																				break;
																			}
																			case 0x24 /* '$' */ :
																			{
																				match('$' /* charlit */ );
																				break;
																			}
																			case 0x5f /* '_' */ :
																			{
																				match('_' /* charlit */ );
																				break;
																			}
																			case 0xc4:
																			case 0xc5:
																			case 0xd6:
																			case 0xe4:
																			case 0xe5:
																			case 0xf6:
																			{
																				mSWEC(false);
																				break;
																			}
																			default:
																			{
																				goto _loop120;
																			}
																			}
																		}
																		_loop120:;
																		} // ( ... )*
																	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
	}
	}}}}}}}}}}}}}}}}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mSWEC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = SWEC;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	switch ( LA(1)) {
	case 0xe5:
	{
		match(static_cast<unsigned char>('\345') /* charlit */ );
		break;
	}
	case 0xe4:
	{
		match(static_cast<unsigned char>('\344') /* charlit */ );
		break;
	}
	case 0xf6:
	{
		match(static_cast<unsigned char>('\366') /* charlit */ );
		break;
	}
	case 0xc5:
	{
		match(static_cast<unsigned char>('\305') /* charlit */ );
		break;
	}
	case 0xc4:
	{
		match(static_cast<unsigned char>('\304') /* charlit */ );
		break;
	}
	case 0xd6:
	{
		match(static_cast<unsigned char>('\326') /* charlit */ );
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mDIGITS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = DIGITS;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	{ // ( ... )+
	int _cnt128=0;
	for (;;) {
		if (((LA(1) >= 0x30 /* '0' */  && LA(1) <= 0x39 /* '9' */ ))) {
			matchRange('0','9');
		}
		else {
			if ( _cnt128>=1 ) { goto _loop128; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());}
		}
		
		_cnt128++;
	}
	_loop128:;
	}  // ( ... )+
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mCHAR_LITERAL(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = CHAR_LITERAL;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	match('\'' /* charlit */ );
	{
	if ((LA(1) == 0x5c /* '\\' */ )) {
		mESC(false);
	}
	else if ((_tokenSet_4.member(LA(1)))) {
		matchNot('\'' /* charlit */ );
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
	}
	
	}
	match('\'' /* charlit */ );
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mESC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = ESC;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	match('\\' /* charlit */ );
	{
	switch ( LA(1)) {
	case 0x6e /* 'n' */ :
	{
		match('n' /* charlit */ );
		break;
	}
	case 0x72 /* 'r' */ :
	{
		match('r' /* charlit */ );
		break;
	}
	case 0x74 /* 't' */ :
	{
		match('t' /* charlit */ );
		break;
	}
	case 0x62 /* 'b' */ :
	{
		match('b' /* charlit */ );
		break;
	}
	case 0x66 /* 'f' */ :
	{
		match('f' /* charlit */ );
		break;
	}
	case 0x22 /* '\"' */ :
	{
		match('\"' /* charlit */ );
		break;
	}
	case 0x30 /* '0' */ :
	{
		match('0' /* charlit */ );
		break;
	}
	case 0x27 /* '\'' */ :
	{
		match('\'' /* charlit */ );
		break;
	}
	case 0x5c /* '\\' */ :
	{
		match('\\' /* charlit */ );
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());
	}
	}
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mSTRING_LITERAL(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; ANTLR_USE_NAMESPACE(std)string::size_type _begin = text.length();
	_ttype = STRING_LITERAL;
	ANTLR_USE_NAMESPACE(std)string::size_type _saveIndex;
	
	match('\"' /* charlit */ );
	{ // ( ... )*
	for (;;) {
		switch ( LA(1)) {
		case 0x5c /* '\\' */ :
		{
			mESC(false);
			break;
		}
		case 0xa /* '\n' */ :
		{
			match('\n' /* charlit */ );
			if ( inputState->guessing==0 ) {
#line 153 "wb_wbllexer.g"
				newline();
#line 3009 "wb_wbllexer.cpp"
			}
			break;
		}
		default:
			if ((_tokenSet_5.member(LA(1)))) {
				matchNot('\"' /* charlit */ );
			}
		else {
			goto _loop125;
		}
		}
	}
	_loop125:;
	} // ( ... )*
	match('\"' /* charlit */ );
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}


const unsigned long wb_wbllexer::_tokenSet_0_data_[] = { 4294966264UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xd 0xe 0xf 0x10 0x11 0x12 0x13 
// 0x14 0x15 0x16 0x17 0x18 0x19 0x1a 0x1b 0x1c 0x1d 0x1e 0x1f   ! \" # 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long wb_wbllexer::_tokenSet_1_data_[] = { 4294958072UL, 4294967293UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xe 0xf 0x10 0x11 0x12 0x13 0x14 
// 0x15 0x16 0x17 0x18 0x19 0x1a 0x1b 0x1c 0x1d 0x1e 0x1f   \" # 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_1(_tokenSet_1_data_,16);
const unsigned long wb_wbllexer::_tokenSet_2_data_[] = { 0UL, 67059712UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_2(_tokenSet_2_data_,10);
const unsigned long wb_wbllexer::_tokenSet_3_data_[] = { 0UL, 16UL, 2281701374UL, 134217726UL, 0UL, 0UL, 4194352UL, 4194352UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_3(_tokenSet_3_data_,16);
const unsigned long wb_wbllexer::_tokenSet_4_data_[] = { 4294967288UL, 4294967167UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xa 0xb 0xc 0xd 0xe 0xf 0x10 0x11 0x12 0x13 
// 0x14 0x15 0x16 0x17 0x18 0x19 0x1a 0x1b 0x1c 0x1d 0x1e 0x1f   ! \" # 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_4(_tokenSet_4_data_,16);
const unsigned long wb_wbllexer::_tokenSet_5_data_[] = { 4294966264UL, 4294967291UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
// 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xb 0xc 0xd 0xe 0xf 0x10 0x11 0x12 0x13 
// 0x14 0x15 0x16 0x17 0x18 0x19 0x1a 0x1b 0x1c 0x1d 0x1e 0x1f   ! # 
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_5(_tokenSet_5_data_,16);

