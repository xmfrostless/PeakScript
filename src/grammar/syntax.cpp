#include "syntax.h"

using namespace peak;

// syntax
static const std::unordered_set<char> SIGN_SET_TEXT_SPACE = { ' ', '\n', '\r', '\t' };
static const std::unordered_set<char> SIGN_SET_TEXT_NEW_LINE = { '\n', '\r' };
static const std::unordered_set<char> SIGN_STRING = { '\"', '\'', '`' };
static const std::vector<std::string> SIGN_COMMENT = { "//", "#" };
static const std::vector<std::string> SIGN_EXTENDS = { ":", "extends" };
static const std::unordered_map<MathSymbol, std::pair<int, std::vector<std::string>>> SIGN_MAP_MATH_SYMBOL = {
	{MathSymbol::Mul, {100, {"*"}}},
	{MathSymbol::Div, {100, {"/"}}},
	{MathSymbol::Mod, {100, {"%"}}},
	{MathSymbol::Add, {90, {"+"}}},
	{MathSymbol::Sub, {90, {"-"}}},
	{MathSymbol::Less, {80, {"<"}}},
	{MathSymbol::LessEqual, {80, {"<="}}},
	{MathSymbol::More, {80, {">"}}},
	{MathSymbol::MoreEqual, {80, {">="}}},
	{MathSymbol::Equal, {70, {"=="}}},
	{MathSymbol::NotEqual, {70, {"!="}}},
	{MathSymbol::LogicAnd, {60, {"&&", "and"}}},
	{MathSymbol::LogicOr, {60, {"||", "or"}}},
	{MathSymbol::AssignMul, {50, {"*="}}},
	{MathSymbol::AssignDiv, {50, {"/="}}},
	{MathSymbol::AssignMod, {50, {"%="}}},
	{MathSymbol::AssignAdd, {50, {"+="}}},
	{MathSymbol::AssignSub, {50, {"-="}}},
};
static const std::unordered_map<DoubleSymbol, std::string> SIGN_DOUBLE_SYMBOL = {
	{DoubleSymbol::AddAdd, "++"},
	{DoubleSymbol::SubSub, "--"},
};
static const std::string SIGN_COMMENT_BLOCK_BEGIN = "/*";
static const std::string SIGN_COMMENT_BLOCK_END = "*/";

static const std::string SIGN_VARIABLE_DEFINE = "var";
static const std::string SIGN_CONST = "const";

static const std::string SIGN_BOOL_TRUE = "true";
static const std::string SIGN_BOOL_FALSE = "false";
static const std::string SIGN_CONDITION_IF = "if";
static const std::string SIGN_CONDITION_ELSE = "else";

static const std::vector<std::string> SIGN_FOR_IN = { "in", ":" };
static const std::string SIGN_NULL = "null";
static const std::string SIGN_ECHO = "echo";
static const std::string SIGN_FOR = "for";
static const std::string SIGN_WHILE = "while";
static const std::string SIGN_DO = "do";
static const std::string SIGN_LOOP = "loop";
static const std::string SIGN_TRY = "try";
static const std::string SIGN_CATCH = "catch";
static const std::string SIGN_FINALLY = "finally";
static const std::string SIGN_BREAK = "break";
static const std::string SIGN_CONTINUE = "continue";
static const std::string SIGN_FUNCTION = "function";
static const std::string SIGN_RETURN = "return";
static const std::string SIGN_SET = "set";
static const std::string SIGN_NEW = "new";
static const std::string SIGN_OBJECT = "object";
static const std::string SIGN_ENUM = "enum";
static const std::string SIGN_IMPORT = "import";
static const std::string SIGN_IMPORT_AS = "as";

static const char SIGN_NOT_SYMBOL = '!';
static const char SIGN_LEFT_BRACKET = '(';
static const char SIGN_RIGHT_BRACKET = ')';
static const char SIGN_SPLIT_SYMBOL = ',';
static const char SIGN_ARRAY_BEGIN = '[';
static const char SIGN_ARRAY_END = ']';
static const char SIGN_INSIDE_SYMBOL = '.';
static const char SIGN_END = ';';
static const char SIGN_ASSIGN = '=';
static const char SIGN_BLOCK_BEGIN = '{';
static const char SIGN_BLOCK_END = '}';

bool Syntax::IsTextSpace(char ch) {
	return (SIGN_SET_TEXT_SPACE.find(ch) != SIGN_SET_TEXT_SPACE.end());
}
bool Syntax::IsTextNewLine(char ch) {
	return (SIGN_SET_TEXT_NEW_LINE.find(ch) != SIGN_SET_TEXT_NEW_LINE.end());
}
bool Syntax::IsTextSpecialChar(char ch) {
	return (ch >= 0 && ch <= 47) || (ch >= 58 && ch <= 64) || (ch >= 91 && ch <= 94) || (ch == 96) || (ch >= 123);
}
bool Syntax::IsTextNumber(char ch) {
	return ch >= '0' && ch <= '9';
}
bool Syntax::IsGrammarStringSign(char ch) {
	return (SIGN_STRING.find(ch) != SIGN_STRING.end());
}

bool Syntax::IsGrammarEndSign(char ch) {
	return SIGN_END == ch;
}

bool Syntax::IsSpecialSign(const std::string& value) {
	std::size_t pos = 0;
	std::size_t size = value.size();
	do {
		if (MatchVariableDefine(value, size, 0, &pos)) {
			break;
		}
		if (MatchNull(value, size, 0, &pos)) {
			break;
		}
		bool bBoolValue = false;
		if (MatchBool(value, size, 0, &pos, &bBoolValue)) {
			break;
		}
		if (MatchEcho(value, size, 0, &pos)) {
			break;
		}
		if (MatchFor(value, size, 0, &pos)) {
			break;
		}
		if (MatchWhile(value, size, 0, &pos)) {
			break;
		}
		if (MatchLoop(value, size, 0, &pos)) {
			break;
		}
		if (MatchBlockBegin(value, size, 0, &pos)) {
			break;
		}
		if (MatchBlockEnd(value, size, 0, &pos)) {
			break;
		}
		if (MatchTry(value, size, 0, &pos)) {
			break;
		}
		if (MatchCatch(value, size, 0, &pos)) {
			break;
		}
		if (MatchFinally(value, size, 0, &pos)) {
			break;
		}
		if (MatchBreak(value, size, 0, &pos)) {
			break;
		}
		if (MatchContinue(value, size, 0, &pos)) {
			break;
		}
		if (MatchDo(value, size, 0, &pos)) {
			break;
		}
		if (MatchConditionIf(value, size, 0, &pos)) {
			break;
		}
		if (MatchConditionElse(value, size, 0, &pos)) {
			break;
		}
		if (MatchReturn(value, size, 0, &pos)) {
			break;
		}
		if (MatchNew(value, size, 0, &pos)) {
			break;
		}
		if (MatchObject(value, size, 0, &pos)) {
			break;
		}
		if (MatchFunction(value, size, 0, &pos)) {
			break;
		}
		if (MatchConst(value, size, 0, &pos)) {
			break;
		}
		if (MatchEnum(value, size, 0, &pos)) {
			break;
		}
		if (MatchImport(value, size, 0, &pos)) {
			break;
		}
		return false;
	} while (false);

	return pos >= size || IsTextSpecialChar(value[pos]);
}
int Syntax::GetMathSymbolLevel(MathSymbol value) {
	auto ite = SIGN_MAP_MATH_SYMBOL.find(value);
	if (ite != SIGN_MAP_MATH_SYMBOL.end()) {
		return ite->second.first;
	}
	return 0;
}

bool Syntax::IsLeftBrcket(char ch) {
	return ch == SIGN_LEFT_BRACKET;
}
bool Syntax::IsRightBrcket(char ch) {
	return ch == SIGN_RIGHT_BRACKET;
}

bool Syntax::IsVariableSelfAssignSymbol(MathSymbol value) {
	static const std::unordered_set<MathSymbol> selfAssignSymbol = {
		MathSymbol::AssignAdd,
		MathSymbol::AssignDiv,
		MathSymbol::AssignSub,
		MathSymbol::AssignMul,
		MathSymbol::AssignMod,
	};
	return selfAssignSymbol.find(value) != selfAssignSymbol.end();
}
bool Syntax::IsWordValidSymbol(char ch) {
	if (!IsTextSpecialChar(ch)) {
		return true;
	}
	return IsTextSpace(ch) ||
		(ch == SIGN_LEFT_BRACKET) ||
		(ch == SIGN_RIGHT_BRACKET) ||
		(ch == SIGN_SPLIT_SYMBOL) ||
		(ch == SIGN_ARRAY_BEGIN) ||
		(ch == SIGN_ARRAY_END) ||
		(ch == SIGN_INSIDE_SYMBOL);
}

bool Syntax::SearchNextInside(const std::string& src, std::size_t size, std::size_t pos) {
	for (auto i = pos; i < size; ++i) {
		if (MatchInsideSymbol(src, size, i, &i)) {
			return true;
		}
		char ch = src[i];
		if (!IsWordValidSymbol(ch)) {
			MathSymbol symbol;
			if (!MatchMathSymbol(src, size, i, &i, &symbol)) {
				return false;
			}
		}
	}
	return false;
}

bool Syntax::SearchNextArray(const std::string& src, std::size_t size, std::size_t pos) {
	for (auto i = pos; i < size; ++i) {
		if (MatchArrayBegin(src, size, i, &i)) {
			return true;
		}
		char ch = src[i];
		if (!IsWordValidSymbol(ch)) {
			MathSymbol symbol;
			if (!MatchMathSymbol(src, size, i, &i, &symbol)) {
				return false;
			}
		}
	}
	return false;
}

bool Syntax::MatchImport(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_IMPORT, src, size, pos, nextPos);
}
bool Syntax::MatchImportAs(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_IMPORT_AS, src, size, pos, nextPos);
}

bool Syntax::MatchEnum(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_ENUM, src, size, pos, nextPos);
}
bool Syntax::MatchExtends(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_EXTENDS, src, size, pos, nextPos);
}
bool Syntax::MatchObject(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_OBJECT, src, size, pos, nextPos);
}
bool Syntax::MatchInsideSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_INSIDE_SYMBOL, src, size, pos, nextPos);
}

bool Syntax::MatchNew(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_NEW, src, size, pos, nextPos);
}
bool Syntax::MatchObjectBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchBlockBegin(src, size, pos, nextPos);
}
bool Syntax::MatchObjectEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchBlockEnd(src, size, pos, nextPos);
}
bool Syntax::MatchArrayBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_ARRAY_BEGIN, src, size, pos, nextPos);
}
bool Syntax::MatchArrayEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_ARRAY_END, src, size, pos, nextPos);
}
bool Syntax::MatchDoubleSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, DoubleSymbol* symbol) {
	std::size_t signSize = 0;
	std::size_t tmpNextPos;
	for (auto& pair : SIGN_DOUBLE_SYMBOL) {
		if (pair.second.size() <= signSize) {
			continue;
		}
		if (MatchSign(pair.second, src, size, pos, &tmpNextPos)) {
			signSize = pair.second.size();
			*nextPos = tmpNextPos;
			*symbol = pair.first;
		}
	}
	return signSize > 0;
}
bool Syntax::MatchNotSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_NOT_SYMBOL, src, size, pos, nextPos);
}
bool Syntax::MatchConst(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_CONST, src, size, pos, nextPos);
}
bool Syntax::MatchReturn(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_RETURN, src, size, pos, nextPos);
}
bool Syntax::MatchSplitSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_SPLIT_SYMBOL, src, size, pos, nextPos);
}
bool Syntax::MatchFunction(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_FUNCTION, src, size, pos, nextPos);
}
bool Syntax::MatchBreak(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_BREAK, src, size, pos, nextPos);
}
bool Syntax::MatchContinue(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_CONTINUE, src, size, pos, nextPos);
}
bool Syntax::MatchTry(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_TRY, src, size, pos, nextPos);
}
bool Syntax::MatchCatch(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_CATCH, src, size, pos, nextPos);
}
bool Syntax::MatchFinally(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_FINALLY, src, size, pos, nextPos);
}
bool Syntax::MatchEcho(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_ECHO, src, size, pos, nextPos);
}
bool Syntax::MatchBlockBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_BLOCK_BEGIN, src, size, pos, nextPos);
}
bool Syntax::MatchBlockEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_BLOCK_END, src, size, pos, nextPos);
}
bool Syntax::MatchConditionIf(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_CONDITION_IF, src, size, pos, nextPos);
}
bool Syntax::MatchConditionElse(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_CONDITION_ELSE, src, size, pos, nextPos);
}
bool Syntax::MatchFor(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_FOR, src, size, pos, nextPos);
}
bool Syntax::MatchForIn(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_FOR_IN, src, size, pos, nextPos);
}
bool Syntax::MatchWhile(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_WHILE, src, size, pos, nextPos);
}
bool Syntax::MatchDo(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_DO, src, size, pos, nextPos);
}
bool Syntax::MatchLoop(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_LOOP, src, size, pos, nextPos);
}
bool Syntax::MatchLeftBrcket(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_LEFT_BRACKET, src, size, pos, nextPos);
}
bool Syntax::MatchRightBrcket(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_RIGHT_BRACKET, src, size, pos, nextPos);
}
bool Syntax::MatchMathSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, MathSymbol* symbol) {
	std::size_t signSize = 0;
	std::size_t tmpNextPos;
	for (auto& pair : SIGN_MAP_MATH_SYMBOL) {
		const auto& setPair = pair.second;
		const auto& signSet = setPair.second;
		for (auto& sign : signSet) {
			if (sign.size() <= signSize) {
				continue;
			}
			if (MatchSign(sign, src, size, pos, &tmpNextPos)) {
				signSize = sign.size();
				*symbol = pair.first;
				*nextPos = tmpNextPos;
			}
		}
	}
	return signSize > 0;
}

bool Syntax::MatchNull(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_NULL, src, size, pos, nextPos);
}

bool Syntax::MatchBool(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, bool* value) {
	if (MatchSign(SIGN_BOOL_TRUE, src, size, pos, nextPos)) {
		*value = true;
		return true;
	}
	if (MatchSign(SIGN_BOOL_FALSE, src, size, pos, nextPos)) {
		*value = false;
		return true;
	}
	return false;
}

bool Syntax::MatchNumber(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, double* number) {
	if (pos >= size) {
		return false;
	}
	char ch = src[pos];
	bool bSub = (ch == '-');
	bool bPoint = (ch == '.');

	if (!IsTextNumber(ch) && !bSub && !bPoint) {
		return false;
	}
	auto beginPos = pos;
	++pos;
	while (pos < size) {
		ch = src[pos];
		if (!IsTextNumber(ch)) {
			if (ch == '.') {
				if (bPoint) {
					return false;
				}
				bPoint = true;
			} else {
				break;
			}
		}
		++pos;
	}

	if (pos == beginPos) {
		return false;
	}

	std::size_t checkSize = 1;
	std::size_t p = 0;
	if (bPoint) {
		if (src[pos - 1] == '.') {
			return false;
		}
		++checkSize;
	}
	if (bSub) {
		++checkSize;
		p = 1;
	}
	auto tempSize = pos - beginPos;
	if (tempSize < checkSize) {
		return false;
	}
	auto tempStr = src.substr(beginPos, tempSize);
	if (bPoint) {
		tempStr.insert(p, 1, '0');
	}
	*number = atof(tempStr.c_str());
	*nextPos = pos;
	return true;
}

bool Syntax::MatchName(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* name) {
	if (pos >= size) {
		return false;
	}
	char ch = src[pos];
	if (IsTextNumber(ch) || IsTextSpecialChar(ch) || IsTextSpace(ch)) {
		return false;
	}
	auto beginPos = pos;
	while (pos < size) {
		ch = src[pos];
		if (IsTextSpecialChar(ch) || IsTextSpace(ch)) {
			break;
		}
		++pos;
	}

	if (pos == beginPos) {
		return false;
	}
	auto tempSize = pos - beginPos;
	auto tempName = src.substr(beginPos, tempSize);
	if (IsSpecialSign(tempName)) {
		return false;
	}
	*name = std::move(tempName);
	*nextPos = pos;
	return true;
}

bool Syntax::MatchAssign(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_ASSIGN, src, size, pos, nextPos);
}

bool Syntax::MatchComment(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_COMMENT, src, size, pos, nextPos);
}
bool Syntax::MatchCommentBlockBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_COMMENT_BLOCK_BEGIN, src, size, pos, nextPos);
}
bool Syntax::MatchCommentBlockEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_COMMENT_BLOCK_END, src, size, pos, nextPos);
}
bool Syntax::MatchVariableDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_VARIABLE_DEFINE, src, size, pos, nextPos);
}
bool Syntax::MatchVariableSet(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return MatchSign(SIGN_SET, src, size, pos, nextPos);
}
bool Syntax::MatchSign(const std::unordered_set<std::string>& sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	std::size_t signSize = 0;
	std::size_t tmpNextPos;
	for (const auto& item : sign) {
		if (item.size() <= signSize) {
			continue;
		}
		if (MatchSign(item, src, size, pos, &tmpNextPos)) {
			signSize = item.size();
			*nextPos = tmpNextPos;
		}
	}
	return signSize > 0;
}
bool Syntax::MatchSign(const std::vector<std::string>& sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	std::size_t signSize = 0;
	std::size_t tmpNextPos;
	for (const auto& item : sign) {
		if (item.size() <= signSize) {
			continue;
		}
		if (MatchSign(item, src, size, pos, &tmpNextPos)) {
			signSize = item.size();
			*nextPos = tmpNextPos;
		}
	}
	return signSize > 0;
}
bool Syntax::MatchSign(const std::string& sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	auto signSize = sign.size();
	if (pos + signSize > size) {
		return false;
	}
	for (std::size_t i = 0; i < signSize; ++i) {
		if (sign[i] != src[pos + i]) {
			return false;
		}
	}
	*nextPos = pos + signSize;
	return true;
}
bool Syntax::MatchSign(char sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (pos >= size) {
		return false;
	}
	if (src[pos] == sign) {
		*nextPos = pos + 1;
		return true;
	}
	return false;
}

bool Syntax::MatchPair(const std::string& signLeft, const std::string& signRight, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* result) {
	if (!MatchSign(signLeft, src, size, pos, &pos)) {
		return false;
	}
	bool bSame = signLeft == signRight;
	auto beginPos = pos;
	int count = 0;
	while (pos < size) {
		if (!bSame && MatchSign(signLeft, src, size, pos, &pos)) {
			++count;
			continue;
		}
		if (MatchSign(signRight, src, size, pos, &pos)) {
			if (count > 0) {
				--count;
				continue;
			}
			auto tempSize = pos - signRight.size() - beginPos;
			*result = src.substr(beginPos, tempSize);
			*nextPos = pos;
			return true;
		}
		if (src[pos] == '\\') {
			++pos;
		}
		++pos;
	}
	return false;
}

bool Syntax::MatchPair(char signLeft, char signRight, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* result) {
	return MatchPair(std::string() + signLeft, std::string() + signRight, src, size, pos, nextPos, result);
}

bool Syntax::MatchString(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* result) {
	for (auto sign : SIGN_STRING) {
		std::string str;
		if (MatchPair(sign, sign, src, size, pos, nextPos, &str)) {
			std::vector<char> buffers;
			buffers.reserve(str.size());
			bool change = false;
			for (char ch : str) {
				if (!change && ch == '\\') {
					change = true;
					continue;
				}

				if (change) {
					if (ch == '\\') {
						buffers.emplace_back('\\');
					} else if (ch == 'n') {
						buffers.emplace_back('\n');
					} else if (ch == 't') {
						buffers.emplace_back('\t');
					} else if (ch == 'b') {
						if (!buffers.empty()) {
							buffers.pop_back();
						}
					} else if (ch == 'r') {
						buffers.emplace_back('\r');
					} else if (ch == '"') {
						buffers.emplace_back('\"');
					} else if (ch == '\'') {
						buffers.emplace_back('\'');
					} else {
						buffers.emplace_back(ch);
					}
					change = false;
				} else {
					buffers.emplace_back(ch);
				}
			}
			result->assign(buffers.begin(), buffers.end());
			return true;
		}
	}
	return false;
}
