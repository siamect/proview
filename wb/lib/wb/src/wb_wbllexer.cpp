/* $ANTLR 2.7.1: "wb_wbllexer.g" -> "wb_wbllexer.cpp"$ */
#include "wb_wbllexer.hpp"
#include "antlr/CharBuffer.hpp"
#include "antlr/TokenStreamException.hpp"
#include "antlr/TokenStreamIOException.hpp"
#include "antlr/TokenStreamRecognitionException.hpp"
#include "antlr/CharStreamException.hpp"
#include "antlr/CharStreamIOException.hpp"
#include "antlr/NoViableAltForCharException.hpp"

#line 1 "wb_wbllexer.g"

#line 14 "wb_wbllexer.cpp"
wb_wbllexer::wb_wbllexer(ANTLR_USE_NAMESPACE(std)istream& in)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(new ANTLR_USE_NAMESPACE(antlr)CharBuffer(in))
{
	setCaseSensitive(true);
	initLiterals();
}

wb_wbllexer::wb_wbllexer(ANTLR_USE_NAMESPACE(antlr)InputBuffer& ib)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(ib)
{
	setCaseSensitive(true);
	initLiterals();
}

wb_wbllexer::wb_wbllexer(const ANTLR_USE_NAMESPACE(antlr)LexerSharedInputState& state)
	: ANTLR_USE_NAMESPACE(antlr)CharScanner(state)
{
	setCaseSensitive(true);
	initLiterals();
}

void wb_wbllexer::initLiterals()
{
}
bool wb_wbllexer::getCaseSensitiveLiterals() const
{
	return true;
}

ANTLR_USE_NAMESPACE(antlr)RefToken wb_wbllexer::nextToken()
{
	ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
	for (;;) {
		ANTLR_USE_NAMESPACE(antlr)RefToken theRetToken;
		int _ttype = ANTLR_USE_NAMESPACE(antlr)Token::INVALID_TYPE;
		resetText();
		try {   // for char stream error handling
			try {   // for lexical error handling
				switch ( LA(1)) {
				case static_cast<unsigned char>('='):
				{
					mEQ(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('|'):
				{
					mOREQ(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('\t'):
				case static_cast<unsigned char>('\n'):
				case static_cast<unsigned char>('\r'):
				case static_cast<unsigned char>(' '):
				{
					mWS(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('['):
				{
					mINDEX(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('$'):
				case static_cast<unsigned char>('-'):
				case static_cast<unsigned char>('.'):
				case static_cast<unsigned char>('0'):
				case static_cast<unsigned char>('1'):
				case static_cast<unsigned char>('2'):
				case static_cast<unsigned char>('3'):
				case static_cast<unsigned char>('4'):
				case static_cast<unsigned char>('5'):
				case static_cast<unsigned char>('6'):
				case static_cast<unsigned char>('7'):
				case static_cast<unsigned char>('8'):
				case static_cast<unsigned char>('9'):
				case static_cast<unsigned char>('A'):
				case static_cast<unsigned char>('B'):
				case static_cast<unsigned char>('C'):
				case static_cast<unsigned char>('D'):
				case static_cast<unsigned char>('E'):
				case static_cast<unsigned char>('F'):
				case static_cast<unsigned char>('G'):
				case static_cast<unsigned char>('H'):
				case static_cast<unsigned char>('I'):
				case static_cast<unsigned char>('J'):
				case static_cast<unsigned char>('K'):
				case static_cast<unsigned char>('L'):
				case static_cast<unsigned char>('M'):
				case static_cast<unsigned char>('N'):
				case static_cast<unsigned char>('O'):
				case static_cast<unsigned char>('P'):
				case static_cast<unsigned char>('Q'):
				case static_cast<unsigned char>('R'):
				case static_cast<unsigned char>('S'):
				case static_cast<unsigned char>('T'):
				case static_cast<unsigned char>('U'):
				case static_cast<unsigned char>('V'):
				case static_cast<unsigned char>('W'):
				case static_cast<unsigned char>('X'):
				case static_cast<unsigned char>('Y'):
				case static_cast<unsigned char>('Z'):
				case static_cast<unsigned char>('_'):
				case static_cast<unsigned char>('a'):
				case static_cast<unsigned char>('b'):
				case static_cast<unsigned char>('c'):
				case static_cast<unsigned char>('d'):
				case static_cast<unsigned char>('e'):
				case static_cast<unsigned char>('f'):
				case static_cast<unsigned char>('g'):
				case static_cast<unsigned char>('h'):
				case static_cast<unsigned char>('i'):
				case static_cast<unsigned char>('j'):
				case static_cast<unsigned char>('k'):
				case static_cast<unsigned char>('l'):
				case static_cast<unsigned char>('m'):
				case static_cast<unsigned char>('n'):
				case static_cast<unsigned char>('o'):
				case static_cast<unsigned char>('p'):
				case static_cast<unsigned char>('q'):
				case static_cast<unsigned char>('r'):
				case static_cast<unsigned char>('s'):
				case static_cast<unsigned char>('t'):
				case static_cast<unsigned char>('u'):
				case static_cast<unsigned char>('v'):
				case static_cast<unsigned char>('w'):
				case static_cast<unsigned char>('x'):
				case static_cast<unsigned char>('y'):
				case static_cast<unsigned char>('z'):
				case static_cast<unsigned char>('\304'):
				case static_cast<unsigned char>('\305'):
				case static_cast<unsigned char>('\326'):
				case static_cast<unsigned char>('\344'):
				case static_cast<unsigned char>('\345'):
				case static_cast<unsigned char>('\366'):
				{
					mVALUE(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('\''):
				{
					mCHAR_LITERAL(true);
					theRetToken=_returnToken;
					break;
				}
				case static_cast<unsigned char>('"'):
				{
					mSTRING_LITERAL(true);
					theRetToken=_returnToken;
					break;
				}
				default:
					if ((LA(1)==static_cast<unsigned char>('!')) && (LA(2)==static_cast<unsigned char>('\t')||LA(2)==static_cast<unsigned char>('\n')||LA(2)==static_cast<unsigned char>(' '))) {
						mCOMMENT(true);
						theRetToken=_returnToken;
					}
					else if ((LA(1)==static_cast<unsigned char>('!')) && (LA(2)==static_cast<unsigned char>('/'))) {
						mDOCBLOCK(true);
						theRetToken=_returnToken;
					}
				else {
					if (LA(1)==EOF_CHAR) {uponEOF(); _returnToken = makeToken(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);}
				else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
				}
				}
				if ( !_returnToken ) goto tryAgain; // found SKIP token
				_ttype = _returnToken->getType();
				_ttype = testLiteralsTable(_ttype);
				_returnToken->setType(_ttype);
				return _returnToken;
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& e) {
				throw ANTLR_USE_NAMESPACE(antlr)TokenStreamRecognitionException(e);
			}
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
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = EQ;
	int _saveIndex;
	
	match(static_cast<unsigned char>('='));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mOREQ(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = OREQ;
	int _saveIndex;
	
	match("|=");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mWS(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = WS;
	int _saveIndex;
	
	{
	int _cnt5=0;
	for (;;) {
		switch ( LA(1)) {
		case static_cast<unsigned char>(' '):
		{
			match(static_cast<unsigned char>(' '));
			break;
		}
		case static_cast<unsigned char>('\t'):
		{
			match(static_cast<unsigned char>('\t'));
			break;
		}
		case static_cast<unsigned char>('\n'):
		{
			match(static_cast<unsigned char>('\n'));
			if ( inputState->guessing==0 ) {
#line 49 "wb_wbllexer.g"
				newline();
#line 257 "wb_wbllexer.cpp"
			}
			break;
		}
		case static_cast<unsigned char>('\r'):
		{
			match(static_cast<unsigned char>('\r'));
			break;
		}
		default:
		{
			if ( _cnt5>=1 ) { goto _loop5; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		}
		_cnt5++;
	}
	_loop5:;
	}
	if ( inputState->guessing==0 ) {
#line 51 "wb_wbllexer.g"
		_ttype = antlr::Token::SKIP;
#line 278 "wb_wbllexer.cpp"
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mCOMMENT(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = COMMENT;
	int _saveIndex;
	
	{
	if ((LA(1)==static_cast<unsigned char>('!')) && (LA(2)==static_cast<unsigned char>('\t')||LA(2)==static_cast<unsigned char>(' '))) {
		match("!");
		{
		switch ( LA(1)) {
		case static_cast<unsigned char>(' '):
		{
			match(static_cast<unsigned char>(' '));
			break;
		}
		case static_cast<unsigned char>('\t'):
		{
			match(static_cast<unsigned char>('\t'));
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
		}
		}
		}
		{
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				matchNot(static_cast<unsigned char>('\n'));
			}
			else {
				goto _loop10;
			}
			
		}
		_loop10:;
		}
		match(static_cast<unsigned char>('\n'));
	}
	else if ((LA(1)==static_cast<unsigned char>('!')) && (LA(2)==static_cast<unsigned char>('\n'))) {
		match("!");
		match(static_cast<unsigned char>('\n'));
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	
	}
	if ( inputState->guessing==0 ) {
#line 57 "wb_wbllexer.g"
		_ttype = antlr::Token::SKIP; newline();
#line 340 "wb_wbllexer.cpp"
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mDOCBLOCK(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = DOCBLOCK;
	int _saveIndex;
	
	match("!/**");
	{
	for (;;) {
		if (((LA(1)==static_cast<unsigned char>('!')) && ((LA(2) >= static_cast<unsigned char>('\3') && LA(2) <= static_cast<unsigned char>('\377'))))&&( LA(2)!='*' )) {
			match(static_cast<unsigned char>('!'));
		}
		else if ((LA(1)==static_cast<unsigned char>('\r')) && (LA(2)==static_cast<unsigned char>('\n'))) {
			match(static_cast<unsigned char>('\r'));
			match(static_cast<unsigned char>('\n'));
			if ( inputState->guessing==0 ) {
#line 74 "wb_wbllexer.g"
				newline();
#line 367 "wb_wbllexer.cpp"
			}
		}
		else if ((LA(1)==static_cast<unsigned char>('\r')) && ((LA(2) >= static_cast<unsigned char>('\3') && LA(2) <= static_cast<unsigned char>('\377')))) {
			match(static_cast<unsigned char>('\r'));
			if ( inputState->guessing==0 ) {
#line 75 "wb_wbllexer.g"
				newline();
#line 375 "wb_wbllexer.cpp"
			}
		}
		else if ((LA(1)==static_cast<unsigned char>('\n'))) {
			match(static_cast<unsigned char>('\n'));
			if ( inputState->guessing==0 ) {
#line 76 "wb_wbllexer.g"
				newline();
#line 383 "wb_wbllexer.cpp"
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
	}
	match("!*/");
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mINDEX(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = INDEX;
	int _saveIndex;
	
	_saveIndex=text.length();
	match(static_cast<unsigned char>('['));
	text.erase(_saveIndex);
	{
	int _cnt17=0;
	for (;;) {
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
		}
		else {
			if ( _cnt17>=1 ) { goto _loop17; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		
		_cnt17++;
	}
	_loop17:;
	}
	_saveIndex=text.length();
	match(static_cast<unsigned char>(']'));
	text.erase(_saveIndex);
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mVALUE(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = VALUE;
	int _saveIndex;
	ANTLR_USE_NAMESPACE(antlr)RefToken oname;
	ANTLR_USE_NAMESPACE(antlr)RefToken soname;
	ANTLR_USE_NAMESPACE(antlr)RefToken doname;
	ANTLR_USE_NAMESPACE(antlr)RefToken vname;
	ANTLR_USE_NAMESPACE(antlr)RefToken buname;
	ANTLR_USE_NAMESPACE(antlr)RefToken boname;
	ANTLR_USE_NAMESPACE(antlr)RefToken aname;
	
	bool synPredMatched20 = false;
	if (((LA(1)==static_cast<unsigned char>('O')) && (LA(2)==static_cast<unsigned char>('b')))) {
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
		_saveIndex=text.length();
		match("Object");
		text.erase(_saveIndex);
		_saveIndex = text.length();
		mWS(false);
		text.erase(_saveIndex);
		{
		int _cnt_oname=0;
		for (;;) {
			switch ( LA(1)) {
			case static_cast<unsigned char>('a'):
			case static_cast<unsigned char>('b'):
			case static_cast<unsigned char>('c'):
			case static_cast<unsigned char>('d'):
			case static_cast<unsigned char>('e'):
			case static_cast<unsigned char>('f'):
			case static_cast<unsigned char>('g'):
			case static_cast<unsigned char>('h'):
			case static_cast<unsigned char>('i'):
			case static_cast<unsigned char>('j'):
			case static_cast<unsigned char>('k'):
			case static_cast<unsigned char>('l'):
			case static_cast<unsigned char>('m'):
			case static_cast<unsigned char>('n'):
			case static_cast<unsigned char>('o'):
			case static_cast<unsigned char>('p'):
			case static_cast<unsigned char>('q'):
			case static_cast<unsigned char>('r'):
			case static_cast<unsigned char>('s'):
			case static_cast<unsigned char>('t'):
			case static_cast<unsigned char>('u'):
			case static_cast<unsigned char>('v'):
			case static_cast<unsigned char>('w'):
			case static_cast<unsigned char>('x'):
			case static_cast<unsigned char>('y'):
			case static_cast<unsigned char>('z'):
			{
				matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
				break;
			}
			case static_cast<unsigned char>('A'):
			case static_cast<unsigned char>('B'):
			case static_cast<unsigned char>('C'):
			case static_cast<unsigned char>('D'):
			case static_cast<unsigned char>('E'):
			case static_cast<unsigned char>('F'):
			case static_cast<unsigned char>('G'):
			case static_cast<unsigned char>('H'):
			case static_cast<unsigned char>('I'):
			case static_cast<unsigned char>('J'):
			case static_cast<unsigned char>('K'):
			case static_cast<unsigned char>('L'):
			case static_cast<unsigned char>('M'):
			case static_cast<unsigned char>('N'):
			case static_cast<unsigned char>('O'):
			case static_cast<unsigned char>('P'):
			case static_cast<unsigned char>('Q'):
			case static_cast<unsigned char>('R'):
			case static_cast<unsigned char>('S'):
			case static_cast<unsigned char>('T'):
			case static_cast<unsigned char>('U'):
			case static_cast<unsigned char>('V'):
			case static_cast<unsigned char>('W'):
			case static_cast<unsigned char>('X'):
			case static_cast<unsigned char>('Y'):
			case static_cast<unsigned char>('Z'):
			{
				matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
				break;
			}
			case static_cast<unsigned char>('0'):
			case static_cast<unsigned char>('1'):
			case static_cast<unsigned char>('2'):
			case static_cast<unsigned char>('3'):
			case static_cast<unsigned char>('4'):
			case static_cast<unsigned char>('5'):
			case static_cast<unsigned char>('6'):
			case static_cast<unsigned char>('7'):
			case static_cast<unsigned char>('8'):
			case static_cast<unsigned char>('9'):
			{
				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
				break;
			}
			case static_cast<unsigned char>('$'):
			{
				match(static_cast<unsigned char>('$'));
				break;
			}
			case static_cast<unsigned char>('_'):
			{
				match(static_cast<unsigned char>('_'));
				break;
			}
			case static_cast<unsigned char>('\304'):
			case static_cast<unsigned char>('\305'):
			case static_cast<unsigned char>('\326'):
			case static_cast<unsigned char>('\344'):
			case static_cast<unsigned char>('\345'):
			case static_cast<unsigned char>('\366'):
			{
				mSWEC(false);
				break;
			}
			default:
			{
				if ( _cnt_oname>=1 ) { goto oname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
			}
			}
			_cnt_oname++;
		}
		oname:;
		}
		if ( inputState->guessing==0 ) {
#line 100 "wb_wbllexer.g"
			_ttype = OBJECT;
#line 586 "wb_wbllexer.cpp"
		}
	}
	else {
		bool synPredMatched24 = false;
		if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
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
#line 102 "wb_wbllexer.g"
				_ttype = ENDOBJECT;
#line 615 "wb_wbllexer.cpp"
			}
		}
		else {
			bool synPredMatched26 = false;
			if (((LA(1)==static_cast<unsigned char>('S')) && (LA(2)==static_cast<unsigned char>('O')))) {
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
				_saveIndex=text.length();
				match("SObject");
				text.erase(_saveIndex);
				_saveIndex = text.length();
				mWS(false);
				text.erase(_saveIndex);
				{
				int _cnt_soname=0;
				for (;;) {
					switch ( LA(1)) {
					case static_cast<unsigned char>('a'):
					case static_cast<unsigned char>('b'):
					case static_cast<unsigned char>('c'):
					case static_cast<unsigned char>('d'):
					case static_cast<unsigned char>('e'):
					case static_cast<unsigned char>('f'):
					case static_cast<unsigned char>('g'):
					case static_cast<unsigned char>('h'):
					case static_cast<unsigned char>('i'):
					case static_cast<unsigned char>('j'):
					case static_cast<unsigned char>('k'):
					case static_cast<unsigned char>('l'):
					case static_cast<unsigned char>('m'):
					case static_cast<unsigned char>('n'):
					case static_cast<unsigned char>('o'):
					case static_cast<unsigned char>('p'):
					case static_cast<unsigned char>('q'):
					case static_cast<unsigned char>('r'):
					case static_cast<unsigned char>('s'):
					case static_cast<unsigned char>('t'):
					case static_cast<unsigned char>('u'):
					case static_cast<unsigned char>('v'):
					case static_cast<unsigned char>('w'):
					case static_cast<unsigned char>('x'):
					case static_cast<unsigned char>('y'):
					case static_cast<unsigned char>('z'):
					{
						matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
						break;
					}
					case static_cast<unsigned char>('A'):
					case static_cast<unsigned char>('B'):
					case static_cast<unsigned char>('C'):
					case static_cast<unsigned char>('D'):
					case static_cast<unsigned char>('E'):
					case static_cast<unsigned char>('F'):
					case static_cast<unsigned char>('G'):
					case static_cast<unsigned char>('H'):
					case static_cast<unsigned char>('I'):
					case static_cast<unsigned char>('J'):
					case static_cast<unsigned char>('K'):
					case static_cast<unsigned char>('L'):
					case static_cast<unsigned char>('M'):
					case static_cast<unsigned char>('N'):
					case static_cast<unsigned char>('O'):
					case static_cast<unsigned char>('P'):
					case static_cast<unsigned char>('Q'):
					case static_cast<unsigned char>('R'):
					case static_cast<unsigned char>('S'):
					case static_cast<unsigned char>('T'):
					case static_cast<unsigned char>('U'):
					case static_cast<unsigned char>('V'):
					case static_cast<unsigned char>('W'):
					case static_cast<unsigned char>('X'):
					case static_cast<unsigned char>('Y'):
					case static_cast<unsigned char>('Z'):
					{
						matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
						break;
					}
					case static_cast<unsigned char>('0'):
					case static_cast<unsigned char>('1'):
					case static_cast<unsigned char>('2'):
					case static_cast<unsigned char>('3'):
					case static_cast<unsigned char>('4'):
					case static_cast<unsigned char>('5'):
					case static_cast<unsigned char>('6'):
					case static_cast<unsigned char>('7'):
					case static_cast<unsigned char>('8'):
					case static_cast<unsigned char>('9'):
					{
						matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
						break;
					}
					case static_cast<unsigned char>('$'):
					{
						match(static_cast<unsigned char>('$'));
						break;
					}
					case static_cast<unsigned char>('_'):
					{
						match(static_cast<unsigned char>('_'));
						break;
					}
					case static_cast<unsigned char>(':'):
					{
						match(static_cast<unsigned char>(':'));
						break;
					}
					case static_cast<unsigned char>('-'):
					{
						match(static_cast<unsigned char>('-'));
						break;
					}
					case static_cast<unsigned char>('\304'):
					case static_cast<unsigned char>('\305'):
					case static_cast<unsigned char>('\326'):
					case static_cast<unsigned char>('\344'):
					case static_cast<unsigned char>('\345'):
					case static_cast<unsigned char>('\366'):
					{
						mSWEC(false);
						break;
					}
					default:
					{
						if ( _cnt_soname>=1 ) { goto soname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
					}
					}
					_cnt_soname++;
				}
				soname:;
				}
				if ( inputState->guessing==0 ) {
#line 104 "wb_wbllexer.g"
					_ttype = SOBJECT;
#line 763 "wb_wbllexer.cpp"
				}
			}
			else {
				bool synPredMatched30 = false;
				if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
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
#line 106 "wb_wbllexer.g"
						_ttype = ENDSOBJECT;
#line 792 "wb_wbllexer.cpp"
					}
				}
				else {
					bool synPredMatched32 = false;
					if (((LA(1)==static_cast<unsigned char>('D')) && (LA(2)==static_cast<unsigned char>('O')))) {
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
						_saveIndex=text.length();
						match("DObject");
						text.erase(_saveIndex);
						_saveIndex = text.length();
						mWS(false);
						text.erase(_saveIndex);
						{
						int _cnt_doname=0;
						for (;;) {
							switch ( LA(1)) {
							case static_cast<unsigned char>('a'):
							case static_cast<unsigned char>('b'):
							case static_cast<unsigned char>('c'):
							case static_cast<unsigned char>('d'):
							case static_cast<unsigned char>('e'):
							case static_cast<unsigned char>('f'):
							case static_cast<unsigned char>('g'):
							case static_cast<unsigned char>('h'):
							case static_cast<unsigned char>('i'):
							case static_cast<unsigned char>('j'):
							case static_cast<unsigned char>('k'):
							case static_cast<unsigned char>('l'):
							case static_cast<unsigned char>('m'):
							case static_cast<unsigned char>('n'):
							case static_cast<unsigned char>('o'):
							case static_cast<unsigned char>('p'):
							case static_cast<unsigned char>('q'):
							case static_cast<unsigned char>('r'):
							case static_cast<unsigned char>('s'):
							case static_cast<unsigned char>('t'):
							case static_cast<unsigned char>('u'):
							case static_cast<unsigned char>('v'):
							case static_cast<unsigned char>('w'):
							case static_cast<unsigned char>('x'):
							case static_cast<unsigned char>('y'):
							case static_cast<unsigned char>('z'):
							{
								matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
								break;
							}
							case static_cast<unsigned char>('A'):
							case static_cast<unsigned char>('B'):
							case static_cast<unsigned char>('C'):
							case static_cast<unsigned char>('D'):
							case static_cast<unsigned char>('E'):
							case static_cast<unsigned char>('F'):
							case static_cast<unsigned char>('G'):
							case static_cast<unsigned char>('H'):
							case static_cast<unsigned char>('I'):
							case static_cast<unsigned char>('J'):
							case static_cast<unsigned char>('K'):
							case static_cast<unsigned char>('L'):
							case static_cast<unsigned char>('M'):
							case static_cast<unsigned char>('N'):
							case static_cast<unsigned char>('O'):
							case static_cast<unsigned char>('P'):
							case static_cast<unsigned char>('Q'):
							case static_cast<unsigned char>('R'):
							case static_cast<unsigned char>('S'):
							case static_cast<unsigned char>('T'):
							case static_cast<unsigned char>('U'):
							case static_cast<unsigned char>('V'):
							case static_cast<unsigned char>('W'):
							case static_cast<unsigned char>('X'):
							case static_cast<unsigned char>('Y'):
							case static_cast<unsigned char>('Z'):
							{
								matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
								break;
							}
							case static_cast<unsigned char>('0'):
							case static_cast<unsigned char>('1'):
							case static_cast<unsigned char>('2'):
							case static_cast<unsigned char>('3'):
							case static_cast<unsigned char>('4'):
							case static_cast<unsigned char>('5'):
							case static_cast<unsigned char>('6'):
							case static_cast<unsigned char>('7'):
							case static_cast<unsigned char>('8'):
							case static_cast<unsigned char>('9'):
							{
								matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
								break;
							}
							case static_cast<unsigned char>('$'):
							{
								match(static_cast<unsigned char>('$'));
								break;
							}
							case static_cast<unsigned char>('_'):
							{
								match(static_cast<unsigned char>('_'));
								break;
							}
							case static_cast<unsigned char>('\304'):
							case static_cast<unsigned char>('\305'):
							case static_cast<unsigned char>('\326'):
							case static_cast<unsigned char>('\344'):
							case static_cast<unsigned char>('\345'):
							case static_cast<unsigned char>('\366'):
							{
								mSWEC(false);
								break;
							}
							default:
							{
								if ( _cnt_doname>=1 ) { goto doname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
							}
							}
							_cnt_doname++;
						}
						doname:;
						}
						if ( inputState->guessing==0 ) {
#line 108 "wb_wbllexer.g"
							_ttype = DOBJECT;
#line 930 "wb_wbllexer.cpp"
						}
					}
					else {
						bool synPredMatched36 = false;
						if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
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
#line 110 "wb_wbllexer.g"
								_ttype = ENDDOBJECT;
#line 959 "wb_wbllexer.cpp"
							}
						}
						else {
							bool synPredMatched38 = false;
							if (((LA(1)==static_cast<unsigned char>('V')) && (LA(2)==static_cast<unsigned char>('o')))) {
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
								_saveIndex=text.length();
								match("Volume");
								text.erase(_saveIndex);
								_saveIndex = text.length();
								mWS(false);
								text.erase(_saveIndex);
								{
								int _cnt_vname=0;
								for (;;) {
									switch ( LA(1)) {
									case static_cast<unsigned char>('a'):
									case static_cast<unsigned char>('b'):
									case static_cast<unsigned char>('c'):
									case static_cast<unsigned char>('d'):
									case static_cast<unsigned char>('e'):
									case static_cast<unsigned char>('f'):
									case static_cast<unsigned char>('g'):
									case static_cast<unsigned char>('h'):
									case static_cast<unsigned char>('i'):
									case static_cast<unsigned char>('j'):
									case static_cast<unsigned char>('k'):
									case static_cast<unsigned char>('l'):
									case static_cast<unsigned char>('m'):
									case static_cast<unsigned char>('n'):
									case static_cast<unsigned char>('o'):
									case static_cast<unsigned char>('p'):
									case static_cast<unsigned char>('q'):
									case static_cast<unsigned char>('r'):
									case static_cast<unsigned char>('s'):
									case static_cast<unsigned char>('t'):
									case static_cast<unsigned char>('u'):
									case static_cast<unsigned char>('v'):
									case static_cast<unsigned char>('w'):
									case static_cast<unsigned char>('x'):
									case static_cast<unsigned char>('y'):
									case static_cast<unsigned char>('z'):
									{
										matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
										break;
									}
									case static_cast<unsigned char>('A'):
									case static_cast<unsigned char>('B'):
									case static_cast<unsigned char>('C'):
									case static_cast<unsigned char>('D'):
									case static_cast<unsigned char>('E'):
									case static_cast<unsigned char>('F'):
									case static_cast<unsigned char>('G'):
									case static_cast<unsigned char>('H'):
									case static_cast<unsigned char>('I'):
									case static_cast<unsigned char>('J'):
									case static_cast<unsigned char>('K'):
									case static_cast<unsigned char>('L'):
									case static_cast<unsigned char>('M'):
									case static_cast<unsigned char>('N'):
									case static_cast<unsigned char>('O'):
									case static_cast<unsigned char>('P'):
									case static_cast<unsigned char>('Q'):
									case static_cast<unsigned char>('R'):
									case static_cast<unsigned char>('S'):
									case static_cast<unsigned char>('T'):
									case static_cast<unsigned char>('U'):
									case static_cast<unsigned char>('V'):
									case static_cast<unsigned char>('W'):
									case static_cast<unsigned char>('X'):
									case static_cast<unsigned char>('Y'):
									case static_cast<unsigned char>('Z'):
									{
										matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
										break;
									}
									case static_cast<unsigned char>('0'):
									case static_cast<unsigned char>('1'):
									case static_cast<unsigned char>('2'):
									case static_cast<unsigned char>('3'):
									case static_cast<unsigned char>('4'):
									case static_cast<unsigned char>('5'):
									case static_cast<unsigned char>('6'):
									case static_cast<unsigned char>('7'):
									case static_cast<unsigned char>('8'):
									case static_cast<unsigned char>('9'):
									{
										matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
										break;
									}
									case static_cast<unsigned char>('$'):
									{
										match(static_cast<unsigned char>('$'));
										break;
									}
									case static_cast<unsigned char>('_'):
									{
										match(static_cast<unsigned char>('_'));
										break;
									}
									case static_cast<unsigned char>('\304'):
									case static_cast<unsigned char>('\305'):
									case static_cast<unsigned char>('\326'):
									case static_cast<unsigned char>('\344'):
									case static_cast<unsigned char>('\345'):
									case static_cast<unsigned char>('\366'):
									{
										mSWEC(false);
										break;
									}
									default:
									{
										if ( _cnt_vname>=1 ) { goto vname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
									}
									}
									_cnt_vname++;
								}
								vname:;
								}
								{
								if ((LA(1)==static_cast<unsigned char>(':'))) {
									match(static_cast<unsigned char>(':'));
								}
								else {
								}
								
								}
								if ( inputState->guessing==0 ) {
#line 112 "wb_wbllexer.g"
									_ttype = VOLUME;
#line 1105 "wb_wbllexer.cpp"
								}
							}
							else {
								bool synPredMatched43 = false;
								if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
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
#line 114 "wb_wbllexer.g"
										_ttype = ENDVOLUME;
#line 1134 "wb_wbllexer.cpp"
									}
								}
								else {
									bool synPredMatched45 = false;
									if (((LA(1)==static_cast<unsigned char>('B')) && (LA(2)==static_cast<unsigned char>('u')))) {
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
										_saveIndex=text.length();
										match("Buffer");
										text.erase(_saveIndex);
										_saveIndex = text.length();
										mWS(false);
										text.erase(_saveIndex);
										{
										int _cnt_buname=0;
										for (;;) {
											switch ( LA(1)) {
											case static_cast<unsigned char>('a'):
											case static_cast<unsigned char>('b'):
											case static_cast<unsigned char>('c'):
											case static_cast<unsigned char>('d'):
											case static_cast<unsigned char>('e'):
											case static_cast<unsigned char>('f'):
											case static_cast<unsigned char>('g'):
											case static_cast<unsigned char>('h'):
											case static_cast<unsigned char>('i'):
											case static_cast<unsigned char>('j'):
											case static_cast<unsigned char>('k'):
											case static_cast<unsigned char>('l'):
											case static_cast<unsigned char>('m'):
											case static_cast<unsigned char>('n'):
											case static_cast<unsigned char>('o'):
											case static_cast<unsigned char>('p'):
											case static_cast<unsigned char>('q'):
											case static_cast<unsigned char>('r'):
											case static_cast<unsigned char>('s'):
											case static_cast<unsigned char>('t'):
											case static_cast<unsigned char>('u'):
											case static_cast<unsigned char>('v'):
											case static_cast<unsigned char>('w'):
											case static_cast<unsigned char>('x'):
											case static_cast<unsigned char>('y'):
											case static_cast<unsigned char>('z'):
											{
												matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
												break;
											}
											case static_cast<unsigned char>('A'):
											case static_cast<unsigned char>('B'):
											case static_cast<unsigned char>('C'):
											case static_cast<unsigned char>('D'):
											case static_cast<unsigned char>('E'):
											case static_cast<unsigned char>('F'):
											case static_cast<unsigned char>('G'):
											case static_cast<unsigned char>('H'):
											case static_cast<unsigned char>('I'):
											case static_cast<unsigned char>('J'):
											case static_cast<unsigned char>('K'):
											case static_cast<unsigned char>('L'):
											case static_cast<unsigned char>('M'):
											case static_cast<unsigned char>('N'):
											case static_cast<unsigned char>('O'):
											case static_cast<unsigned char>('P'):
											case static_cast<unsigned char>('Q'):
											case static_cast<unsigned char>('R'):
											case static_cast<unsigned char>('S'):
											case static_cast<unsigned char>('T'):
											case static_cast<unsigned char>('U'):
											case static_cast<unsigned char>('V'):
											case static_cast<unsigned char>('W'):
											case static_cast<unsigned char>('X'):
											case static_cast<unsigned char>('Y'):
											case static_cast<unsigned char>('Z'):
											{
												matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
												break;
											}
											case static_cast<unsigned char>('0'):
											case static_cast<unsigned char>('1'):
											case static_cast<unsigned char>('2'):
											case static_cast<unsigned char>('3'):
											case static_cast<unsigned char>('4'):
											case static_cast<unsigned char>('5'):
											case static_cast<unsigned char>('6'):
											case static_cast<unsigned char>('7'):
											case static_cast<unsigned char>('8'):
											case static_cast<unsigned char>('9'):
											{
												matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
												break;
											}
											case static_cast<unsigned char>('$'):
											{
												match(static_cast<unsigned char>('$'));
												break;
											}
											case static_cast<unsigned char>('_'):
											{
												match(static_cast<unsigned char>('_'));
												break;
											}
											case static_cast<unsigned char>('\304'):
											case static_cast<unsigned char>('\305'):
											case static_cast<unsigned char>('\326'):
											case static_cast<unsigned char>('\344'):
											case static_cast<unsigned char>('\345'):
											case static_cast<unsigned char>('\366'):
											{
												mSWEC(false);
												break;
											}
											default:
											{
												if ( _cnt_buname>=1 ) { goto buname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
											}
											}
											_cnt_buname++;
										}
										buname:;
										}
										if ( inputState->guessing==0 ) {
#line 116 "wb_wbllexer.g"
											_ttype = BUFFER;
#line 1272 "wb_wbllexer.cpp"
										}
									}
									else {
										bool synPredMatched49 = false;
										if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
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
#line 118 "wb_wbllexer.g"
												_ttype = ENDBUFFER;
#line 1301 "wb_wbllexer.cpp"
											}
										}
										else {
											bool synPredMatched51 = false;
											if (((LA(1)==static_cast<unsigned char>('B')) && (LA(2)==static_cast<unsigned char>('o')))) {
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
												_saveIndex=text.length();
												match("Body");
												text.erase(_saveIndex);
												_saveIndex = text.length();
												mWS(false);
												text.erase(_saveIndex);
												{
												int _cnt_boname=0;
												for (;;) {
													switch ( LA(1)) {
													case static_cast<unsigned char>('a'):
													case static_cast<unsigned char>('b'):
													case static_cast<unsigned char>('c'):
													case static_cast<unsigned char>('d'):
													case static_cast<unsigned char>('e'):
													case static_cast<unsigned char>('f'):
													case static_cast<unsigned char>('g'):
													case static_cast<unsigned char>('h'):
													case static_cast<unsigned char>('i'):
													case static_cast<unsigned char>('j'):
													case static_cast<unsigned char>('k'):
													case static_cast<unsigned char>('l'):
													case static_cast<unsigned char>('m'):
													case static_cast<unsigned char>('n'):
													case static_cast<unsigned char>('o'):
													case static_cast<unsigned char>('p'):
													case static_cast<unsigned char>('q'):
													case static_cast<unsigned char>('r'):
													case static_cast<unsigned char>('s'):
													case static_cast<unsigned char>('t'):
													case static_cast<unsigned char>('u'):
													case static_cast<unsigned char>('v'):
													case static_cast<unsigned char>('w'):
													case static_cast<unsigned char>('x'):
													case static_cast<unsigned char>('y'):
													case static_cast<unsigned char>('z'):
													{
														matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
														break;
													}
													case static_cast<unsigned char>('A'):
													case static_cast<unsigned char>('B'):
													case static_cast<unsigned char>('C'):
													case static_cast<unsigned char>('D'):
													case static_cast<unsigned char>('E'):
													case static_cast<unsigned char>('F'):
													case static_cast<unsigned char>('G'):
													case static_cast<unsigned char>('H'):
													case static_cast<unsigned char>('I'):
													case static_cast<unsigned char>('J'):
													case static_cast<unsigned char>('K'):
													case static_cast<unsigned char>('L'):
													case static_cast<unsigned char>('M'):
													case static_cast<unsigned char>('N'):
													case static_cast<unsigned char>('O'):
													case static_cast<unsigned char>('P'):
													case static_cast<unsigned char>('Q'):
													case static_cast<unsigned char>('R'):
													case static_cast<unsigned char>('S'):
													case static_cast<unsigned char>('T'):
													case static_cast<unsigned char>('U'):
													case static_cast<unsigned char>('V'):
													case static_cast<unsigned char>('W'):
													case static_cast<unsigned char>('X'):
													case static_cast<unsigned char>('Y'):
													case static_cast<unsigned char>('Z'):
													{
														matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
														break;
													}
													case static_cast<unsigned char>('0'):
													case static_cast<unsigned char>('1'):
													case static_cast<unsigned char>('2'):
													case static_cast<unsigned char>('3'):
													case static_cast<unsigned char>('4'):
													case static_cast<unsigned char>('5'):
													case static_cast<unsigned char>('6'):
													case static_cast<unsigned char>('7'):
													case static_cast<unsigned char>('8'):
													case static_cast<unsigned char>('9'):
													{
														matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
														break;
													}
													case static_cast<unsigned char>('$'):
													{
														match(static_cast<unsigned char>('$'));
														break;
													}
													case static_cast<unsigned char>('_'):
													{
														match(static_cast<unsigned char>('_'));
														break;
													}
													case static_cast<unsigned char>('\304'):
													case static_cast<unsigned char>('\305'):
													case static_cast<unsigned char>('\326'):
													case static_cast<unsigned char>('\344'):
													case static_cast<unsigned char>('\345'):
													case static_cast<unsigned char>('\366'):
													{
														mSWEC(false);
														break;
													}
													default:
													{
														if ( _cnt_boname>=1 ) { goto boname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
													}
													}
													_cnt_boname++;
												}
												boname:;
												}
												if ( inputState->guessing==0 ) {
#line 120 "wb_wbllexer.g"
													_ttype = BODY;
#line 1439 "wb_wbllexer.cpp"
												}
											}
											else {
												bool synPredMatched55 = false;
												if (((LA(1)==static_cast<unsigned char>('E')) && (LA(2)==static_cast<unsigned char>('n')))) {
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
#line 122 "wb_wbllexer.g"
														_ttype = ENDBODY;
#line 1468 "wb_wbllexer.cpp"
													}
												}
												else {
													bool synPredMatched57 = false;
													if (((LA(1)==static_cast<unsigned char>('A')) && (LA(2)==static_cast<unsigned char>('t')))) {
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
														_saveIndex=text.length();
														match("Attr");
														text.erase(_saveIndex);
														_saveIndex = text.length();
														mWS(false);
														text.erase(_saveIndex);
														{
														int _cnt_aname=0;
														for (;;) {
															switch ( LA(1)) {
															case static_cast<unsigned char>('a'):
															case static_cast<unsigned char>('b'):
															case static_cast<unsigned char>('c'):
															case static_cast<unsigned char>('d'):
															case static_cast<unsigned char>('e'):
															case static_cast<unsigned char>('f'):
															case static_cast<unsigned char>('g'):
															case static_cast<unsigned char>('h'):
															case static_cast<unsigned char>('i'):
															case static_cast<unsigned char>('j'):
															case static_cast<unsigned char>('k'):
															case static_cast<unsigned char>('l'):
															case static_cast<unsigned char>('m'):
															case static_cast<unsigned char>('n'):
															case static_cast<unsigned char>('o'):
															case static_cast<unsigned char>('p'):
															case static_cast<unsigned char>('q'):
															case static_cast<unsigned char>('r'):
															case static_cast<unsigned char>('s'):
															case static_cast<unsigned char>('t'):
															case static_cast<unsigned char>('u'):
															case static_cast<unsigned char>('v'):
															case static_cast<unsigned char>('w'):
															case static_cast<unsigned char>('x'):
															case static_cast<unsigned char>('y'):
															case static_cast<unsigned char>('z'):
															{
																matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																break;
															}
															case static_cast<unsigned char>('A'):
															case static_cast<unsigned char>('B'):
															case static_cast<unsigned char>('C'):
															case static_cast<unsigned char>('D'):
															case static_cast<unsigned char>('E'):
															case static_cast<unsigned char>('F'):
															case static_cast<unsigned char>('G'):
															case static_cast<unsigned char>('H'):
															case static_cast<unsigned char>('I'):
															case static_cast<unsigned char>('J'):
															case static_cast<unsigned char>('K'):
															case static_cast<unsigned char>('L'):
															case static_cast<unsigned char>('M'):
															case static_cast<unsigned char>('N'):
															case static_cast<unsigned char>('O'):
															case static_cast<unsigned char>('P'):
															case static_cast<unsigned char>('Q'):
															case static_cast<unsigned char>('R'):
															case static_cast<unsigned char>('S'):
															case static_cast<unsigned char>('T'):
															case static_cast<unsigned char>('U'):
															case static_cast<unsigned char>('V'):
															case static_cast<unsigned char>('W'):
															case static_cast<unsigned char>('X'):
															case static_cast<unsigned char>('Y'):
															case static_cast<unsigned char>('Z'):
															{
																matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																break;
															}
															case static_cast<unsigned char>('0'):
															case static_cast<unsigned char>('1'):
															case static_cast<unsigned char>('2'):
															case static_cast<unsigned char>('3'):
															case static_cast<unsigned char>('4'):
															case static_cast<unsigned char>('5'):
															case static_cast<unsigned char>('6'):
															case static_cast<unsigned char>('7'):
															case static_cast<unsigned char>('8'):
															case static_cast<unsigned char>('9'):
															{
																matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																break;
															}
															case static_cast<unsigned char>('$'):
															{
																match(static_cast<unsigned char>('$'));
																break;
															}
															case static_cast<unsigned char>('_'):
															{
																match(static_cast<unsigned char>('_'));
																break;
															}
															case static_cast<unsigned char>('\304'):
															case static_cast<unsigned char>('\305'):
															case static_cast<unsigned char>('\326'):
															case static_cast<unsigned char>('\344'):
															case static_cast<unsigned char>('\345'):
															case static_cast<unsigned char>('\366'):
															{
																mSWEC(false);
																break;
															}
															case static_cast<unsigned char>('['):
															{
																match(static_cast<unsigned char>('['));
																break;
															}
															case static_cast<unsigned char>(']'):
															{
																match(static_cast<unsigned char>(']'));
																break;
															}
															default:
															{
																if ( _cnt_aname>=1 ) { goto aname; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
															}
															}
															_cnt_aname++;
														}
														aname:;
														}
														{
														for (;;) {
															if ((LA(1)==static_cast<unsigned char>('.'))) {
																match(static_cast<unsigned char>('.'));
																{
																int _cnt62=0;
																for (;;) {
																	switch ( LA(1)) {
																	case static_cast<unsigned char>('a'):
																	case static_cast<unsigned char>('b'):
																	case static_cast<unsigned char>('c'):
																	case static_cast<unsigned char>('d'):
																	case static_cast<unsigned char>('e'):
																	case static_cast<unsigned char>('f'):
																	case static_cast<unsigned char>('g'):
																	case static_cast<unsigned char>('h'):
																	case static_cast<unsigned char>('i'):
																	case static_cast<unsigned char>('j'):
																	case static_cast<unsigned char>('k'):
																	case static_cast<unsigned char>('l'):
																	case static_cast<unsigned char>('m'):
																	case static_cast<unsigned char>('n'):
																	case static_cast<unsigned char>('o'):
																	case static_cast<unsigned char>('p'):
																	case static_cast<unsigned char>('q'):
																	case static_cast<unsigned char>('r'):
																	case static_cast<unsigned char>('s'):
																	case static_cast<unsigned char>('t'):
																	case static_cast<unsigned char>('u'):
																	case static_cast<unsigned char>('v'):
																	case static_cast<unsigned char>('w'):
																	case static_cast<unsigned char>('x'):
																	case static_cast<unsigned char>('y'):
																	case static_cast<unsigned char>('z'):
																	{
																		matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																		break;
																	}
																	case static_cast<unsigned char>('A'):
																	case static_cast<unsigned char>('B'):
																	case static_cast<unsigned char>('C'):
																	case static_cast<unsigned char>('D'):
																	case static_cast<unsigned char>('E'):
																	case static_cast<unsigned char>('F'):
																	case static_cast<unsigned char>('G'):
																	case static_cast<unsigned char>('H'):
																	case static_cast<unsigned char>('I'):
																	case static_cast<unsigned char>('J'):
																	case static_cast<unsigned char>('K'):
																	case static_cast<unsigned char>('L'):
																	case static_cast<unsigned char>('M'):
																	case static_cast<unsigned char>('N'):
																	case static_cast<unsigned char>('O'):
																	case static_cast<unsigned char>('P'):
																	case static_cast<unsigned char>('Q'):
																	case static_cast<unsigned char>('R'):
																	case static_cast<unsigned char>('S'):
																	case static_cast<unsigned char>('T'):
																	case static_cast<unsigned char>('U'):
																	case static_cast<unsigned char>('V'):
																	case static_cast<unsigned char>('W'):
																	case static_cast<unsigned char>('X'):
																	case static_cast<unsigned char>('Y'):
																	case static_cast<unsigned char>('Z'):
																	{
																		matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																		break;
																	}
																	case static_cast<unsigned char>('0'):
																	case static_cast<unsigned char>('1'):
																	case static_cast<unsigned char>('2'):
																	case static_cast<unsigned char>('3'):
																	case static_cast<unsigned char>('4'):
																	case static_cast<unsigned char>('5'):
																	case static_cast<unsigned char>('6'):
																	case static_cast<unsigned char>('7'):
																	case static_cast<unsigned char>('8'):
																	case static_cast<unsigned char>('9'):
																	{
																		matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																		break;
																	}
																	case static_cast<unsigned char>('$'):
																	{
																		match(static_cast<unsigned char>('$'));
																		break;
																	}
																	case static_cast<unsigned char>('_'):
																	{
																		match(static_cast<unsigned char>('_'));
																		break;
																	}
																	case static_cast<unsigned char>('\304'):
																	case static_cast<unsigned char>('\305'):
																	case static_cast<unsigned char>('\326'):
																	case static_cast<unsigned char>('\344'):
																	case static_cast<unsigned char>('\345'):
																	case static_cast<unsigned char>('\366'):
																	{
																		mSWEC(false);
																		break;
																	}
																	case static_cast<unsigned char>('['):
																	{
																		match(static_cast<unsigned char>('['));
																		break;
																	}
																	case static_cast<unsigned char>(']'):
																	{
																		match(static_cast<unsigned char>(']'));
																		break;
																	}
																	default:
																	{
																		if ( _cnt62>=1 ) { goto _loop62; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																	}
																	}
																	_cnt62++;
																}
																_loop62:;
																}
															}
															else {
																goto _loop63;
															}
															
														}
														_loop63:;
														}
														if ( inputState->guessing==0 ) {
#line 124 "wb_wbllexer.g"
															_ttype = ATTRIBUTE;
#line 1745 "wb_wbllexer.cpp"
														}
													}
													else {
														bool synPredMatched65 = false;
														if ((((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9'))) && (_tokenSet_2.member(LA(2))))) {
															int _m65 = mark();
															synPredMatched65 = true;
															inputState->guessing++;
															try {
																{
																mDIGITS(false);
																match(static_cast<unsigned char>('.'));
																mDIGITS(false);
																match(static_cast<unsigned char>('.'));
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
															match(static_cast<unsigned char>('.'));
															mDIGITS(false);
															match(static_cast<unsigned char>('.'));
															mDIGITS(false);
															match(static_cast<unsigned char>('.'));
															mDIGITS(false);
															{
															if ((LA(1)==static_cast<unsigned char>(':'))) {
																match(static_cast<unsigned char>(':'));
																mDIGITS(false);
															}
															else {
															}
															
															}
															if ( inputState->guessing==0 ) {
#line 126 "wb_wbllexer.g"
																_ttype = OID;
#line 1788 "wb_wbllexer.cpp"
															}
														}
														else {
															bool synPredMatched68 = false;
															if ((((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9'))) && (_tokenSet_2.member(LA(2))))) {
																int _m68 = mark();
																synPredMatched68 = true;
																inputState->guessing++;
																try {
																	{
																	mDIGITS(false);
																	match(static_cast<unsigned char>('.'));
																	}
																}
																catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																	synPredMatched68 = false;
																}
																rewind(_m68);
																inputState->guessing--;
															}
															if ( synPredMatched68 ) {
																mDIGITS(false);
																match(static_cast<unsigned char>('.'));
																{
																for (;;) {
																	if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																		matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																	}
																	else {
																		goto _loop70;
																	}
																	
																}
																_loop70:;
																}
																{
																if ((LA(1)==static_cast<unsigned char>('E')||LA(1)==static_cast<unsigned char>('e'))) {
																	{
																	switch ( LA(1)) {
																	case static_cast<unsigned char>('e'):
																	{
																		match(static_cast<unsigned char>('e'));
																		break;
																	}
																	case static_cast<unsigned char>('E'):
																	{
																		match(static_cast<unsigned char>('E'));
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																	}
																	}
																	}
																	{
																	switch ( LA(1)) {
																	case static_cast<unsigned char>('+'):
																	{
																		match(static_cast<unsigned char>('+'));
																		break;
																	}
																	case static_cast<unsigned char>('-'):
																	{
																		match(static_cast<unsigned char>('-'));
																		break;
																	}
																	case static_cast<unsigned char>('0'):
																	case static_cast<unsigned char>('1'):
																	case static_cast<unsigned char>('2'):
																	case static_cast<unsigned char>('3'):
																	case static_cast<unsigned char>('4'):
																	case static_cast<unsigned char>('5'):
																	case static_cast<unsigned char>('6'):
																	case static_cast<unsigned char>('7'):
																	case static_cast<unsigned char>('8'):
																	case static_cast<unsigned char>('9'):
																	{
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																	}
																	}
																	}
																	{
																	int _cnt75=0;
																	for (;;) {
																		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																		}
																		else {
																			if ( _cnt75>=1 ) { goto _loop75; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																		}
																		
																		_cnt75++;
																	}
																	_loop75:;
																	}
																}
																else {
																}
																
																}
																if ( inputState->guessing==0 ) {
#line 128 "wb_wbllexer.g"
																	_ttype = NUM_FLOAT;
#line 1897 "wb_wbllexer.cpp"
																}
															}
															else {
																bool synPredMatched77 = false;
																if (((LA(1)==static_cast<unsigned char>('-')) && ((LA(2) >= static_cast<unsigned char>('0') && LA(2) <= static_cast<unsigned char>('9'))))) {
																	int _m77 = mark();
																	synPredMatched77 = true;
																	inputState->guessing++;
																	try {
																		{
																		match(static_cast<unsigned char>('-'));
																		mDIGITS(false);
																		match(static_cast<unsigned char>('.'));
																		}
																	}
																	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
																		synPredMatched77 = false;
																	}
																	rewind(_m77);
																	inputState->guessing--;
																}
																if ( synPredMatched77 ) {
																	match(static_cast<unsigned char>('-'));
																	mDIGITS(false);
																	match(static_cast<unsigned char>('.'));
																	{
																	for (;;) {
																		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																		}
																		else {
																			goto _loop79;
																		}
																		
																	}
																	_loop79:;
																	}
																	{
																	if ((LA(1)==static_cast<unsigned char>('E')||LA(1)==static_cast<unsigned char>('e'))) {
																		{
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('e'):
																		{
																			match(static_cast<unsigned char>('e'));
																			break;
																		}
																		case static_cast<unsigned char>('E'):
																		{
																			match(static_cast<unsigned char>('E'));
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																		}
																		}
																		}
																		{
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('+'):
																		{
																			match(static_cast<unsigned char>('+'));
																			break;
																		}
																		case static_cast<unsigned char>('-'):
																		{
																			match(static_cast<unsigned char>('-'));
																			break;
																		}
																		case static_cast<unsigned char>('0'):
																		case static_cast<unsigned char>('1'):
																		case static_cast<unsigned char>('2'):
																		case static_cast<unsigned char>('3'):
																		case static_cast<unsigned char>('4'):
																		case static_cast<unsigned char>('5'):
																		case static_cast<unsigned char>('6'):
																		case static_cast<unsigned char>('7'):
																		case static_cast<unsigned char>('8'):
																		case static_cast<unsigned char>('9'):
																		{
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																		}
																		}
																		}
																		{
																		int _cnt84=0;
																		for (;;) {
																			if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																			}
																			else {
																				if ( _cnt84>=1 ) { goto _loop84; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																			}
																			
																			_cnt84++;
																		}
																		_loop84:;
																		}
																	}
																	else {
																	}
																	
																	}
																	if ( inputState->guessing==0 ) {
#line 130 "wb_wbllexer.g"
																		_ttype = NUM_FLOAT;
#line 2008 "wb_wbllexer.cpp"
																	}
																}
																else if ((LA(1)==static_cast<unsigned char>('-')) && ((LA(2) >= static_cast<unsigned char>('0') && LA(2) <= static_cast<unsigned char>('9')))) {
																	match(static_cast<unsigned char>('-'));
																	mDIGITS(false);
																	if ( inputState->guessing==0 ) {
#line 136 "wb_wbllexer.g"
																		_ttype = INT;
#line 2017 "wb_wbllexer.cpp"
																	}
																}
																else if ((LA(1)==static_cast<unsigned char>('.'))) {
																	match(static_cast<unsigned char>('.'));
																	{
																	int _cnt86=0;
																	for (;;) {
																		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																		}
																		else {
																			if ( _cnt86>=1 ) { goto _loop86; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																		}
																		
																		_cnt86++;
																	}
																	_loop86:;
																	}
																	{
																	if ((LA(1)==static_cast<unsigned char>('E')||LA(1)==static_cast<unsigned char>('e'))) {
																		{
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('e'):
																		{
																			match(static_cast<unsigned char>('e'));
																			break;
																		}
																		case static_cast<unsigned char>('E'):
																		{
																			match(static_cast<unsigned char>('E'));
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																		}
																		}
																		}
																		{
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('+'):
																		{
																			match(static_cast<unsigned char>('+'));
																			break;
																		}
																		case static_cast<unsigned char>('-'):
																		{
																			match(static_cast<unsigned char>('-'));
																			break;
																		}
																		case static_cast<unsigned char>('0'):
																		case static_cast<unsigned char>('1'):
																		case static_cast<unsigned char>('2'):
																		case static_cast<unsigned char>('3'):
																		case static_cast<unsigned char>('4'):
																		case static_cast<unsigned char>('5'):
																		case static_cast<unsigned char>('6'):
																		case static_cast<unsigned char>('7'):
																		case static_cast<unsigned char>('8'):
																		case static_cast<unsigned char>('9'):
																		{
																			break;
																		}
																		default:
																		{
																			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																		}
																		}
																		}
																		{
																		int _cnt91=0;
																		for (;;) {
																			if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
																				matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																			}
																			else {
																				if ( _cnt91>=1 ) { goto _loop91; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
																			}
																			
																			_cnt91++;
																		}
																		_loop91:;
																		}
																	}
																	else {
																	}
																	
																	}
																	if ( inputState->guessing==0 ) {
#line 132 "wb_wbllexer.g"
																		_ttype = NUM_FLOAT;
#line 2109 "wb_wbllexer.cpp"
																	}
																}
																else if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9'))) && (true)) {
																	mDIGITS(false);
																	if ( inputState->guessing==0 ) {
#line 134 "wb_wbllexer.g"
																		_ttype = INT;
#line 2117 "wb_wbllexer.cpp"
																	}
																}
																else if ((_tokenSet_3.member(LA(1))) && (true)) {
																	{
																	switch ( LA(1)) {
																	case static_cast<unsigned char>('a'):
																	case static_cast<unsigned char>('b'):
																	case static_cast<unsigned char>('c'):
																	case static_cast<unsigned char>('d'):
																	case static_cast<unsigned char>('e'):
																	case static_cast<unsigned char>('f'):
																	case static_cast<unsigned char>('g'):
																	case static_cast<unsigned char>('h'):
																	case static_cast<unsigned char>('i'):
																	case static_cast<unsigned char>('j'):
																	case static_cast<unsigned char>('k'):
																	case static_cast<unsigned char>('l'):
																	case static_cast<unsigned char>('m'):
																	case static_cast<unsigned char>('n'):
																	case static_cast<unsigned char>('o'):
																	case static_cast<unsigned char>('p'):
																	case static_cast<unsigned char>('q'):
																	case static_cast<unsigned char>('r'):
																	case static_cast<unsigned char>('s'):
																	case static_cast<unsigned char>('t'):
																	case static_cast<unsigned char>('u'):
																	case static_cast<unsigned char>('v'):
																	case static_cast<unsigned char>('w'):
																	case static_cast<unsigned char>('x'):
																	case static_cast<unsigned char>('y'):
																	case static_cast<unsigned char>('z'):
																	{
																		matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																		break;
																	}
																	case static_cast<unsigned char>('A'):
																	case static_cast<unsigned char>('B'):
																	case static_cast<unsigned char>('C'):
																	case static_cast<unsigned char>('D'):
																	case static_cast<unsigned char>('E'):
																	case static_cast<unsigned char>('F'):
																	case static_cast<unsigned char>('G'):
																	case static_cast<unsigned char>('H'):
																	case static_cast<unsigned char>('I'):
																	case static_cast<unsigned char>('J'):
																	case static_cast<unsigned char>('K'):
																	case static_cast<unsigned char>('L'):
																	case static_cast<unsigned char>('M'):
																	case static_cast<unsigned char>('N'):
																	case static_cast<unsigned char>('O'):
																	case static_cast<unsigned char>('P'):
																	case static_cast<unsigned char>('Q'):
																	case static_cast<unsigned char>('R'):
																	case static_cast<unsigned char>('S'):
																	case static_cast<unsigned char>('T'):
																	case static_cast<unsigned char>('U'):
																	case static_cast<unsigned char>('V'):
																	case static_cast<unsigned char>('W'):
																	case static_cast<unsigned char>('X'):
																	case static_cast<unsigned char>('Y'):
																	case static_cast<unsigned char>('Z'):
																	{
																		matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																		break;
																	}
																	case static_cast<unsigned char>('$'):
																	{
																		match(static_cast<unsigned char>('$'));
																		break;
																	}
																	case static_cast<unsigned char>('_'):
																	{
																		match(static_cast<unsigned char>('_'));
																		break;
																	}
																	case static_cast<unsigned char>('\304'):
																	case static_cast<unsigned char>('\305'):
																	case static_cast<unsigned char>('\326'):
																	case static_cast<unsigned char>('\344'):
																	case static_cast<unsigned char>('\345'):
																	case static_cast<unsigned char>('\366'):
																	{
																		mSWEC(false);
																		break;
																	}
																	default:
																	{
																		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
																	}
																	}
																	}
																	{
																	for (;;) {
																		switch ( LA(1)) {
																		case static_cast<unsigned char>('a'):
																		case static_cast<unsigned char>('b'):
																		case static_cast<unsigned char>('c'):
																		case static_cast<unsigned char>('d'):
																		case static_cast<unsigned char>('e'):
																		case static_cast<unsigned char>('f'):
																		case static_cast<unsigned char>('g'):
																		case static_cast<unsigned char>('h'):
																		case static_cast<unsigned char>('i'):
																		case static_cast<unsigned char>('j'):
																		case static_cast<unsigned char>('k'):
																		case static_cast<unsigned char>('l'):
																		case static_cast<unsigned char>('m'):
																		case static_cast<unsigned char>('n'):
																		case static_cast<unsigned char>('o'):
																		case static_cast<unsigned char>('p'):
																		case static_cast<unsigned char>('q'):
																		case static_cast<unsigned char>('r'):
																		case static_cast<unsigned char>('s'):
																		case static_cast<unsigned char>('t'):
																		case static_cast<unsigned char>('u'):
																		case static_cast<unsigned char>('v'):
																		case static_cast<unsigned char>('w'):
																		case static_cast<unsigned char>('x'):
																		case static_cast<unsigned char>('y'):
																		case static_cast<unsigned char>('z'):
																		{
																			matchRange(static_cast<unsigned char>('a'),static_cast<unsigned char>('z'));
																			break;
																		}
																		case static_cast<unsigned char>('A'):
																		case static_cast<unsigned char>('B'):
																		case static_cast<unsigned char>('C'):
																		case static_cast<unsigned char>('D'):
																		case static_cast<unsigned char>('E'):
																		case static_cast<unsigned char>('F'):
																		case static_cast<unsigned char>('G'):
																		case static_cast<unsigned char>('H'):
																		case static_cast<unsigned char>('I'):
																		case static_cast<unsigned char>('J'):
																		case static_cast<unsigned char>('K'):
																		case static_cast<unsigned char>('L'):
																		case static_cast<unsigned char>('M'):
																		case static_cast<unsigned char>('N'):
																		case static_cast<unsigned char>('O'):
																		case static_cast<unsigned char>('P'):
																		case static_cast<unsigned char>('Q'):
																		case static_cast<unsigned char>('R'):
																		case static_cast<unsigned char>('S'):
																		case static_cast<unsigned char>('T'):
																		case static_cast<unsigned char>('U'):
																		case static_cast<unsigned char>('V'):
																		case static_cast<unsigned char>('W'):
																		case static_cast<unsigned char>('X'):
																		case static_cast<unsigned char>('Y'):
																		case static_cast<unsigned char>('Z'):
																		{
																			matchRange(static_cast<unsigned char>('A'),static_cast<unsigned char>('Z'));
																			break;
																		}
																		case static_cast<unsigned char>('0'):
																		case static_cast<unsigned char>('1'):
																		case static_cast<unsigned char>('2'):
																		case static_cast<unsigned char>('3'):
																		case static_cast<unsigned char>('4'):
																		case static_cast<unsigned char>('5'):
																		case static_cast<unsigned char>('6'):
																		case static_cast<unsigned char>('7'):
																		case static_cast<unsigned char>('8'):
																		case static_cast<unsigned char>('9'):
																		{
																			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
																			break;
																		}
																		case static_cast<unsigned char>('$'):
																		{
																			match(static_cast<unsigned char>('$'));
																			break;
																		}
																		case static_cast<unsigned char>('_'):
																		{
																			match(static_cast<unsigned char>('_'));
																			break;
																		}
																		case static_cast<unsigned char>('\304'):
																		case static_cast<unsigned char>('\305'):
																		case static_cast<unsigned char>('\326'):
																		case static_cast<unsigned char>('\344'):
																		case static_cast<unsigned char>('\345'):
																		case static_cast<unsigned char>('\366'):
																		{
																			mSWEC(false);
																			break;
																		}
																		default:
																		{
																			goto _loop94;
																		}
																		}
																	}
																	_loop94:;
																	}
																}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	}}}}}}}}}}}}}}}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mSWEC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = SWEC;
	int _saveIndex;
	
	switch ( LA(1)) {
	case static_cast<unsigned char>('\345'):
	{
		match(static_cast<unsigned char>('\345'));
		break;
	}
	case static_cast<unsigned char>('\344'):
	{
		match(static_cast<unsigned char>('\344'));
		break;
	}
	case static_cast<unsigned char>('\366'):
	{
		match(static_cast<unsigned char>('\366'));
		break;
	}
	case static_cast<unsigned char>('\305'):
	{
		match(static_cast<unsigned char>('\305'));
		break;
	}
	case static_cast<unsigned char>('\304'):
	{
		match(static_cast<unsigned char>('\304'));
		break;
	}
	case static_cast<unsigned char>('\326'):
	{
		match(static_cast<unsigned char>('\326'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
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
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = DIGITS;
	int _saveIndex;
	
	{
	int _cnt102=0;
	for (;;) {
		if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('9')))) {
			matchRange(static_cast<unsigned char>('0'),static_cast<unsigned char>('9'));
		}
		else {
			if ( _cnt102>=1 ) { goto _loop102; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());}
		}
		
		_cnt102++;
	}
	_loop102:;
	}
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mCHAR_LITERAL(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = CHAR_LITERAL;
	int _saveIndex;
	
	match(static_cast<unsigned char>('\''));
	{
	if ((LA(1)==static_cast<unsigned char>('\\'))) {
		mESC(false);
	}
	else if ((_tokenSet_4.member(LA(1)))) {
		matchNot(static_cast<unsigned char>('\''));
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
	}
	
	}
	match(static_cast<unsigned char>('\''));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}

void wb_wbllexer::mESC(bool _createToken) {
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = ESC;
	int _saveIndex;
	
	match(static_cast<unsigned char>('\\'));
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('n'):
	{
		match(static_cast<unsigned char>('n'));
		break;
	}
	case static_cast<unsigned char>('r'):
	{
		match(static_cast<unsigned char>('r'));
		break;
	}
	case static_cast<unsigned char>('t'):
	{
		match(static_cast<unsigned char>('t'));
		break;
	}
	case static_cast<unsigned char>('b'):
	{
		match(static_cast<unsigned char>('b'));
		break;
	}
	case static_cast<unsigned char>('f'):
	{
		match(static_cast<unsigned char>('f'));
		break;
	}
	case static_cast<unsigned char>('"'):
	{
		match(static_cast<unsigned char>('"'));
		break;
	}
	case static_cast<unsigned char>('0'):
	{
		match(static_cast<unsigned char>('0'));
		break;
	}
	case static_cast<unsigned char>('\''):
	{
		match(static_cast<unsigned char>('\''));
		break;
	}
	case static_cast<unsigned char>('\\'):
	{
		match(static_cast<unsigned char>('\\'));
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltForCharException(LA(1), getFilename(), getLine());
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
	int _ttype; ANTLR_USE_NAMESPACE(antlr)RefToken _token; int _begin=text.length();
	_ttype = STRING_LITERAL;
	int _saveIndex;
	
	match(static_cast<unsigned char>('"'));
	{
	for (;;) {
		switch ( LA(1)) {
		case static_cast<unsigned char>('\\'):
		{
			mESC(false);
			break;
		}
		case static_cast<unsigned char>('\n'):
		{
			match(static_cast<unsigned char>('\n'));
			if ( inputState->guessing==0 ) {
#line 145 "wb_wbllexer.g"
				newline();
#line 2517 "wb_wbllexer.cpp"
			}
			break;
		}
		default:
			if ((_tokenSet_5.member(LA(1)))) {
				matchNot(static_cast<unsigned char>('"'));
			}
		else {
			goto _loop99;
		}
		}
	}
	_loop99:;
	}
	match(static_cast<unsigned char>('"'));
	if ( _createToken && _token==ANTLR_USE_NAMESPACE(antlr)nullToken && _ttype!=ANTLR_USE_NAMESPACE(antlr)Token::SKIP ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
	_saveIndex=0;
}


const unsigned long wb_wbllexer::_tokenSet_0_data_[] = { 4294966264UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_0(_tokenSet_0_data_,16);
const unsigned long wb_wbllexer::_tokenSet_1_data_[] = { 4294958072UL, 4294967293UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_1(_tokenSet_1_data_,16);
const unsigned long wb_wbllexer::_tokenSet_2_data_[] = { 0UL, 67059712UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_2(_tokenSet_2_data_,10);
const unsigned long wb_wbllexer::_tokenSet_3_data_[] = { 0UL, 16UL, 2281701374UL, 134217726UL, 0UL, 0UL, 4194352UL, 4194352UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_3(_tokenSet_3_data_,16);
const unsigned long wb_wbllexer::_tokenSet_4_data_[] = { 4294967288UL, 4294967167UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_4(_tokenSet_4_data_,16);
const unsigned long wb_wbllexer::_tokenSet_5_data_[] = { 4294966264UL, 4294967291UL, 4026531839UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 4294967295UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const ANTLR_USE_NAMESPACE(antlr)BitSet wb_wbllexer::_tokenSet_5(_tokenSet_5_data_,16);

