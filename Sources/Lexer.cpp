#include "Lexer.h"

#include <array>

std::array<Symbol, 256> CHAR_TO_SYMBOL_MAP =
{
	/* 000 nul */	Symbol::UNDEFINED,
	/* 001 soh */	Symbol::UNDEFINED,
	/* 002 stx */	Symbol::UNDEFINED,
	/* 003 etx */	Symbol::UNDEFINED,
	/* 004 eot */	Symbol::UNDEFINED,
	/* 005 enq */	Symbol::UNDEFINED,
	/* 006 ack */	Symbol::UNDEFINED,
	/* 007 bel */	Symbol::UNDEFINED,
	/* 010 bs  */	Symbol::UNDEFINED,
	/* 011 ht  */	Symbol::BLANK,
	/* 012 nl  */	Symbol::NEWLINE,
	/* 013 vt  */	Symbol::BLANK,
	/* 014 ff  */	Symbol::BLANK,
	/* 015 cr  */	Symbol::UNDEFINED,
	/* 016 so  */	Symbol::UNDEFINED,
	/* 017 si  */	Symbol::UNDEFINED,
	/* 020 dle */	Symbol::UNDEFINED,
	/* 021 dc1 */	Symbol::UNDEFINED,
	/* 022 dc2 */	Symbol::UNDEFINED,
	/* 023 dc3 */	Symbol::UNDEFINED,
	/* 024 dc4 */	Symbol::UNDEFINED,
	/* 025 nak */	Symbol::UNDEFINED,
	/* 026 syn */	Symbol::UNDEFINED,
	/* 027 etb */	Symbol::UNDEFINED,
	/* 030 can */	Symbol::UNDEFINED,
	/* 031 em  */	Symbol::UNDEFINED,
	/* 032 sub */	Symbol::UNDEFINED,
	/* 033 esc */	Symbol::UNDEFINED,
	/* 034 fs  */	Symbol::UNDEFINED,
	/* 035 gs  */	Symbol::UNDEFINED,
	/* 036 rs  */	Symbol::UNDEFINED,
	/* 037 us  */	Symbol::UNDEFINED,
	/* 040 sp  */	Symbol::BLANK,
	/* 041 !   */	Symbol::OTHER,
	/* 042 "   */	Symbol::INDICATOR,
	/* 043 #   */	Symbol::UNDEFINED,	// undefined symbol in MinusC
	/* 044 $   */	Symbol::UNDEFINED,
	/* 045 %   */	Symbol::OTHER,
	/* 046 &   */	Symbol::INDICATOR,	// undefined symbol in MinusC, except for "&&"
	/* 047 '   */	Symbol::INDICATOR,	// e.g 'a'
	/* 050 (   */	Symbol::OTHER,
	/* 051    */	Symbol::OTHER,
	/* 052 *   */	Symbol::OTHER,
	/* 053 +   */	Symbol::OTHER,
	/* 054 ,   */	Symbol::OTHER,
	/* 055 -   */	Symbol::OTHER,
	/* 056 .   */	Symbol::INDICATOR,	// undefined symbol in MinusC, except for introduction a float-part
	/* 057 /   */	Symbol::OTHER,
	/* 060 0   */	Symbol::DIGIT,
	/* 061 1   */	Symbol::DIGIT,
	/* 062 2   */	Symbol::DIGIT,
	/* 063 3   */	Symbol::DIGIT,
	/* 064 4   */	Symbol::DIGIT,
	/* 065 5   */	Symbol::DIGIT,
	/* 066 6   */	Symbol::DIGIT,
	/* 067 7   */	Symbol::DIGIT,
	/* 070 8   */	Symbol::DIGIT,
	/* 071 9   */	Symbol::DIGIT,
	/* 072 :   */	Symbol::OTHER,
	/* 073 ;   */	Symbol::OTHER,
	/* 074 <   */	Symbol::OTHER,
	/* 075 =   */	Symbol::OTHER,
	/* 076 >   */	Symbol::OTHER,
	/* 077 ?   */	Symbol::UNDEFINED,	// undefined symbol in MinusC
	/* 100 @   */	Symbol::UNDEFINED,
	/* 101 A   */	Symbol::LETTER,
	/* 102 B   */	Symbol::LETTER,
	/* 103 C   */	Symbol::LETTER,
	/* 104 D   */	Symbol::LETTER,
	/* 105 E   */	Symbol::LETTER,
	/* 106 F   */	Symbol::LETTER,
	/* 107 G   */	Symbol::LETTER,
	/* 110 H   */	Symbol::LETTER,
	/* 111 I   */	Symbol::LETTER,
	/* 112 J   */	Symbol::LETTER,
	/* 113 K   */	Symbol::LETTER,
	/* 114 L   */	Symbol::LETTER,
	/* 115 M   */	Symbol::LETTER,
	/* 116 N   */	Symbol::LETTER,
	/* 117 O   */	Symbol::LETTER,
	/* 120 P   */	Symbol::LETTER,
	/* 121 Q   */	Symbol::LETTER,
	/* 122 R   */	Symbol::LETTER,
	/* 123 S   */	Symbol::LETTER,
	/* 124 T   */	Symbol::LETTER,
	/* 125 U   */	Symbol::LETTER,
	/* 126 V   */	Symbol::LETTER,
	/* 127 W   */	Symbol::LETTER,
	/* 130 X   */	Symbol::LETTER,
	/* 131 Y   */	Symbol::LETTER,
	/* 132 Z   */	Symbol::LETTER,
	/* 133 [   */	Symbol::OTHER,
	/* 134 \   */	Symbol::UNDEFINED,	// undefined symbol in MinusC, except for introduction an escape sequence
	/* 135 ]   */	Symbol::OTHER,
	/* 136 ^   */	Symbol::UNDEFINED,	// undefined symbol in MinusC
	/* 137 _   */	Symbol::LETTER,
	/* 140 `   */	Symbol::UNDEFINED,
	/* 141 a   */	Symbol::LETTER,
	/* 142 b   */	Symbol::LETTER,
	/* 143 c   */	Symbol::LETTER,
	/* 144 d   */	Symbol::LETTER,
	/* 145 e   */	Symbol::LETTER,
	/* 146 f   */	Symbol::LETTER,
	/* 147 g   */	Symbol::LETTER,
	/* 150 h   */	Symbol::LETTER,
	/* 151 i   */	Symbol::LETTER,
	/* 152 j   */	Symbol::LETTER,
	/* 153 k   */	Symbol::LETTER,
	/* 154 l   */	Symbol::LETTER,
	/* 155 m   */	Symbol::LETTER,
	/* 156 n   */	Symbol::LETTER,
	/* 157 o   */	Symbol::LETTER,
	/* 160 p   */	Symbol::LETTER,
	/* 161 q   */	Symbol::LETTER,
	/* 162 r   */	Symbol::LETTER,
	/* 163 s   */	Symbol::LETTER,
	/* 164 t   */	Symbol::LETTER,
	/* 165 u   */	Symbol::LETTER,
	/* 166 v   */	Symbol::LETTER,
	/* 167 w   */	Symbol::LETTER,
	/* 170 x   */	Symbol::LETTER,
	/* 171 y   */	Symbol::LETTER,
	/* 172 z   */	Symbol::LETTER,
	/* 173 {   */	Symbol::OTHER,
	/* 174 |   */	Symbol::INDICATOR,	// undefined symbol in MinusC, except for  "||"
	/* 175 }   */	Symbol::OTHER,
	/* 176 ~   */	Symbol::UNDEFINED
};

int Lexer::m_lineBufferSize = 2049;
unsigned int Lexer::m_tokenMaxLength = 32;

#define COMPLEX_OPERATOR_DFA(ch1stSymbol, ch2ndSymbol, singleOperator, complexOperator)	\
{\
	if (*m_tokenIter == ch1stSymbol)	\
	{	\
		++m_tokenIter;	\
		if (*m_tokenIter == ch2ndSymbol)	\
		{	\
			token->token = complexOperator;	\
			++m_tokenIter;	\
		}	\
		else\
		{	\
			if (CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_tokenHead)] == Symbol::INDICATOR)	\
			{	\
				token->token = TokenType::ERROR;	\
				/* TODO: Report the error */	\
			}	\
			else\
			{	\
				token->token = singleOperator;	\
			} \
        }	\
		goto EXIT_FUNC; \
	}\
}

int Lexer::IdentifierDFA::m_idMaxLength = 32;
const char* Lexer::IdentifierDFA::m_keywords[] =
{
	"if",
	"else",
	"for",
	"while",
	"return",
	"continue",
	"break",
	"char",
	"goto",
	"int",
	"double",
	"void",
	"read",
	"write",
	nullptr
};

void Lexer::IdentifierDFA::GetToken(Token* token)
{
	while (+(CHAR_TO_SYMBOL_MAP[static_cast<int>(*++m_lexer.m_tokenIter)] & (Symbol::DIGIT | Symbol::LETTER)))
	{
		// Do nothing	
	}

	if ((m_lexer.m_tokenIter - m_lexer.m_tokenHead) / sizeof(char) > m_tokenMaxLength)
	{
		token->token = TokenType::ERROR;

		// TODO: Report the error
	}
	else
	{
		assert(m_lexer.m_tokenIter > m_lexer.m_tokenHead);

		const char temp = *m_lexer.m_tokenIter;
		*m_lexer.m_tokenIter = '\0';

		const char * keyword = GetKeyword(m_lexer.m_tokenHead);

		*m_lexer.m_tokenIter = temp;

		if (keyword != nullptr)
		{
			switch (*keyword)
			{
			case 'b':
				token->token = TokenType::KEYWORD_BREAK;
				break;
			case 'c':
				if (*++keyword == 'h')
				{
					token->token = TokenType::KEYWORD_CHAR;
				}
				else
				{
					token->token = TokenType::KEYWORD_CONTINUE;
				}
				break;
			case 'd':
				token->token = TokenType::KEYWORD_DOUBLE;
				break;
			case 'e':
				token->token = TokenType::KEYWORD_ELSE;
				break;
			case 'f':
				token->token = TokenType::KEYWORD_FOR;
				break;
			case 'g':
				token->token = TokenType::KEYWORD_GOTO;
				break;
			case 'i':
				if (*++keyword == 'f')
				{
					token->token = TokenType::KEYWORD_IF;
				}
				else
				{
					token->token = TokenType::KEYWORD_INT;
				}
				break;
			case 'r':
				if (keyword[2] == 'a')
				{
					token->token = TokenType::KEYWORD_READ;
				}
				else
				{
					token->token = TokenType::KEYWORD_RETURN;
				}
				break;
			case 'v':
				token->token = TokenType::KEYWORD_VOID;
				break;
			case 'w':
				if (*++keyword == 'h')
				{
					token->token = TokenType::KEYWORD_WHILE;
				}
				else
				{
					token->token = TokenType::KEYWORD_WRITE;
				}
				break;
			default:
				assert(nullptr);
				token->token = TokenType::ERROR;
				// TODO: Report the error
			}
		}
		else
		{
			token->token = TokenType::ID;
			token->val.strVal = new char[m_lexer.m_tokenIter - m_lexer.m_tokenHead + 1];
			memset(token->val.strVal, 0, m_lexer.m_tokenIter - m_lexer.m_tokenHead + 1);
			strncpy_s(token->val.strVal, m_lexer.m_tokenIter - m_lexer.m_tokenHead, m_lexer.m_tokenHead, m_lexer.m_tokenIter - m_lexer.m_tokenHead);
		}
	}
}

int Lexer::NumericsDFA::m_wordLength = 16;
unsigned int Lexer::NumericsDFA::m_bufferMaxSize = 32;

void Lexer::NumericsDFA::GetToken(Token* token) const
{
	while (+(CHAR_TO_SYMBOL_MAP[static_cast<int>(*++m_lexer.m_tokenIter)] & Symbol::DIGIT))
	{
		// Do nothing	
	}

	if (*m_lexer.m_tokenIter == '.' || *m_lexer.m_tokenIter == 'E')
	{
		// Floating-point or exponent part
	}
	else
	{
		if (CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_lexer.m_tokenIter)] == Symbol::LETTER)
		{
			token->token = TokenType::ERROR;

			// TODO: Report the error
		}
		else
		{
			token->token = TokenType::NUM_INT;
		}

		goto EXIT_FUNC;
	}

	if (*m_lexer.m_tokenIter == '.')
	{
		while (CHAR_TO_SYMBOL_MAP[static_cast<int>(*++m_lexer.m_tokenIter)] == Symbol::DIGIT)
		{
			// Do nothing
		}

		if (*m_lexer.m_tokenIter == 'E')
		{
		EXP_PART:
			if (*m_lexer.m_tokenIter == '+' || *m_lexer.m_tokenIter == '-')
			{
				++m_lexer.m_tokenIter;
			}

			if (+(CHAR_TO_SYMBOL_MAP[static_cast<int>(*++m_lexer.m_tokenIter)] & Symbol::DIGIT))
			{
				while (+(CHAR_TO_SYMBOL_MAP[static_cast<int>(*++m_lexer.m_tokenIter)] & Symbol::DIGIT))
				{
					// Do nothing
				}

				if (+(CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_lexer.m_tokenIter)] & Symbol::LETTER))
				{
					token->token = TokenType::ERROR;

					// TODO: Report the error
				}
				else
				{
					token->token = TokenType::NUM_DOUBLE;
				}
			}
			else
			{
				token->token = TokenType::ERROR;

				// TODO: Report the error
			}
		}
		else
		{
			if (+(CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_lexer.m_tokenIter)] & Symbol::LETTER))
			{
				token->token = TokenType::ERROR;

				// TODO: Report the error
			}
			else
			{
				token->token = TokenType::NUM_DOUBLE;
			}
		}
	}
	else if (*m_lexer.m_tokenIter == 'E')
	{
		goto EXP_PART;
	}

EXIT_FUNC:
	if (token->token == TokenType::NUM_INT || token->token == TokenType::NUM_DOUBLE)
	{
		const unsigned int charCount = m_lexer.m_tokenIter - m_lexer.m_tokenHead;

		if (charCount > m_bufferMaxSize)
		{
			token->token = TokenType::ERROR;

			// TODO: Report the error
		}
		else
		{
			token->val.strVal = new char[charCount + 1];
			memset(token->val.strVal, 0, charCount + 1);
			strncpy_s(token->val.strVal, charCount, m_lexer.m_tokenHead, charCount);
		}
	}
}

int Lexer::CharDFA::m_charSeqMaxLength = 4;
char Lexer::CharDFA::m_reservedEscSeq[10] = { '\0' };

void Lexer::CharDFA::GetToken(Token* token) const
{
	int charCount;

	++m_lexer.m_tokenIter;

	while (*m_lexer.m_tokenIter != '\'' && *m_lexer.m_tokenIter != '\0')
	{
		++m_lexer.m_tokenIter;
	}

	if (*m_lexer.m_tokenIter != '\'')
	{
		token->token = TokenType::ERROR;

		// TODO: Report the error
	}
	else
	{
		charCount = (m_lexer.m_tokenIter - m_lexer.m_tokenHead - 1) / sizeof(char);
		assert(charCount >= 0);

		if (charCount == 0)
		{
			token->token = TokenType::ERROR;

			// TODO: Report the error

			goto EXIT_FUNC;
		}

		if (charCount > m_charSeqMaxLength)
		{
			token->token = TokenType::ERROR;

			// TODO: Report the error

			goto EXIT_FUNC;
		}

		if (m_lexer.m_tokenHead[1] == '\\')
		{
			if (charCount > 2)
			{
				token->token = TokenType::ERROR;

				// TODO: Report the error

				goto EXIT_FUNC;
			}

			if (charCount == 2)
			{
				switch (m_lexer.m_tokenHead[2])
				{
				case 'n':
					token->token = TokenType::CHAR;
					token->val.chVal = 0xa;
					break;
				case 'r':
					token->token = TokenType::CHAR;
					token->val.chVal = 0xd;
					break;
				case 't':
					token->token = TokenType::CHAR;
					token->val.chVal = 0x9;
					break;
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
					token->token = TokenType::CHAR;
					token->val.chVal = 0x0;
					break;
				default:
					token->token = TokenType::ERROR;

					// TODO: Report the error
				}

				goto EXIT_FUNC;
			}
		}

		token->token = TokenType::CHAR;
		token->val.chVal = m_lexer.m_tokenHead[1];
	}

EXIT_FUNC:
	if (*m_lexer.m_tokenIter == '\'')
	{
		++m_lexer.m_tokenIter;
	}
}

int Lexer::StringDFA::m_strMaxLength = 32;

void Lexer::StringDFA::GetToken(Token* token) const
{
	int charCount;

	++m_lexer.m_tokenIter;

	while (*m_lexer.m_tokenIter != '"' && *m_lexer.m_tokenIter != '\0')
	{
		++m_lexer.m_tokenIter;
	}

	if (*m_lexer.m_tokenIter != '"')
	{
		token->token = TokenType::ERROR;

		// TODO: Report the error
	}
	else
	{
		charCount = (m_lexer.m_tokenIter - m_lexer.m_tokenHead - 1) / sizeof(char);
		assert(charCount >= 0);

		if (charCount > m_strMaxLength)
		{
			token->token = TokenType::ERROR;

			// TODO: Report the error
		}
		else
		{
			token->token = TokenType::STRING_LITERAL;
			token->val.strVal = new char[charCount + 1];
			memset(token->val.strVal, 0, charCount + 1);
			strncpy_s(token->val.strVal, charCount, m_lexer.m_tokenHead + 1, charCount);
		}
	}
}

void Lexer::CommentDFA::GetToken(Token* token) const
{
	bool isExitComment = false;
	bool isReadNewLine = false;

	if (*m_lexer.m_tokenIter == '/')
	{
		while (*++m_lexer.m_tokenIter != '\0')
		{
			// Do nothing;
		}

		token->token = TokenType::UNKNOWN;
	}
	else if (*m_lexer.m_tokenIter == '*')
	{
		while (true)
		{
			if (!isReadNewLine)
			{
				++m_lexer.m_tokenIter;
			}
			else
			{
				isReadNewLine = false;
			}

			if (*m_lexer.m_tokenIter == '*')
			{
				isExitComment = true;
			}
			else if (*m_lexer.m_tokenIter == '\0')
			{
				memset(m_lexer.m_lineBuffer, 0, m_lineBufferSize);
				if (m_lexer.m_sourceFile.get(m_lexer.m_lineBuffer, m_lineBufferSize))
				{
					m_lexer.m_tokenIter = m_lexer.m_lineBuffer;
					++m_lexer.m_lineNumber;
				}
				else
				{
					token->token = TokenType::ERROR;

					// TODO: Report the error

					break;
				}

				isReadNewLine = true;
				isExitComment = false;
			}
			else if (*m_lexer.m_tokenIter == '/')
			{
				if (isExitComment)
				{
					token->token = TokenType::UNKNOWN;

					++m_lexer.m_tokenIter;

					break;
				}

				isExitComment = false;
			}
			else
			{
				isExitComment = false;
			}
		}
	}
}

Lexer::Lexer(const char* fileName) :
	m_identifierDFA(*this), m_numericsDFA(*this),
	m_charDFA(*this), m_stringDFA(*this), m_commentDFA(*this),
	m_lineBufferTail(nullptr)
{
	m_sourceFile.open(fileName);
	assert(m_sourceFile.is_open());

	m_lineBuffer = new char[m_lineBufferSize];
	memset(m_lineBuffer, 0, m_lineBufferSize);

	m_sourceFile.get(m_lineBuffer, m_lineBufferSize);
	m_tokenHead = m_tokenIter = m_lineBuffer;

	m_lineNumber = 1;
}

Lexer::~Lexer()
{
	delete m_lineBuffer;
}

Token* Lexer::GetToken()
{
	assert(m_tokenIter != nullptr);

	Token* token = new Token();

CHECK:
	if (*m_tokenHead == '\0')
	{
		if (m_sourceFile.get(m_lineBuffer, m_lineBufferSize))
		{
			m_tokenIter = m_tokenHead = m_lineBuffer;
			++m_lineNumber;
		}
		else
		{
			token->token = TokenType::END_OF_FILE;
			goto EXIT_FUNC;
		}
	}

	while (+((CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_tokenIter)] & Symbol::BLANK | Symbol::NEWLINE)))
	{
		++m_tokenIter;
	}
	m_tokenHead = m_tokenIter;

	if (*m_tokenIter == '\0')
	{
		goto CHECK;
	}

	const Symbol symbol = CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_tokenIter)];

	if (+(symbol & Symbol::LETTER))
	{
		m_identifierDFA.GetToken(token);
		goto EXIT_FUNC;
	}

	if (+(symbol & Symbol::DIGIT))
	{
		m_numericsDFA.GetToken(token);
		goto EXIT_FUNC;
	}

	if (*m_tokenIter == '\'')
	{
		m_charDFA.GetToken(token);
		goto EXIT_FUNC;
	}

	if (*m_tokenIter == '"')
	{
		m_stringDFA.GetToken(token);
		goto EXIT_FUNC;
	}

	if (*m_tokenIter == '/')
	{
		++m_tokenIter;

		if (*m_tokenIter == '/' || *m_tokenIter == '*')
		{
			m_commentDFA.GetToken(token);
		}
		else
		{
			token->token = TokenType::OP_DIV;
		}

		goto EXIT_FUNC;
	}

	COMPLEX_OPERATOR_DFA('=', '=', TokenType::OP_ASSIGN, TokenType::OP_EQUAL);
	COMPLEX_OPERATOR_DFA('!', '=', TokenType::OP_NOT, TokenType::OP_NOTEQUAL);
	COMPLEX_OPERATOR_DFA('>', '=', TokenType::OP_GREATERTHAN, TokenType::OP_GREATERTHANEQUAL);
	COMPLEX_OPERATOR_DFA('<', '=', TokenType::OP_LESSTHAN, TokenType::OP_LESSTHANEQUAL);
	COMPLEX_OPERATOR_DFA('&', '&', TokenType::ERROR, TokenType::OP_AND);
	COMPLEX_OPERATOR_DFA('|', '|', TokenType::ERROR, TokenType::OP_OR);

	if (+(symbol & (Symbol::OTHER | Symbol::INDICATOR)))
	{
		assert(
			*m_tokenIter != '!' && *m_tokenIter != '&'  && *m_tokenIter != '|' &&
			*m_tokenIter != '>' && *m_tokenIter != '<'  && *m_tokenIter != '=' &&
			*m_tokenIter != '/' && *m_tokenIter != '\'' && *m_tokenIter != '"');

		switch (*m_tokenIter)
		{
		case '%':	token->token = TokenType::OP_MOD;	break;
		case '(':	token->token = TokenType::PUNCT_LPAREN;	break;
		case ')':	token->token = TokenType::PUNCT_RPAREN;	break;
		case '*':	token->token = TokenType::OP_MUL;	break;
		case '+':	token->token = TokenType::OP_ADD;	break;
		case '-':	token->token = TokenType::OP_SUB;	break;
		case ',':	token->token = TokenType::PUNCT_COMMA;	break;
		case ':':	token->token = TokenType::PUNCT_COLON;  break;
		case ';':	token->token = TokenType::PUNCT_SEMICOLON; break;
		case '[':	token->token = TokenType::PUNCT_LBRACK;	break;
		case ']':	token->token = TokenType::PUNCT_RBRACK;	break;
		case '{':	token->token = TokenType::PUNCT_LBRACE;	break;
		case '}':	token->token = TokenType::PUNCT_RBRACE;	break;
		default:	break;
		}

		++m_tokenIter;

		goto EXIT_FUNC;
	}

	assert(CHAR_TO_SYMBOL_MAP[static_cast<int>(*m_tokenIter)] == Symbol::UNDEFINED);

	token->token = TokenType::ERROR;
	++m_tokenIter;

	// TODO: Report the error

EXIT_FUNC:
	if (*m_tokenIter == 0xa)
	{
		++m_tokenIter;
	}

	m_tokenHead = m_tokenIter;

	return token;
}