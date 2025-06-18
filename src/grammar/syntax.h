/*
	PeakScript: https://github.com/Jenocn/PeakScript
	By Jenocn: https://jenocn.github.io
*/

#pragma once

#include <string>
#include <unordered_set>

namespace peak {

enum class MathSymbol: char {
	None,
	LessEqual, // <=
	MoreEqual, // >=
	Equal,	   // ==
	NotEqual,  // !=
	LogicAnd,  // &&
	LogicOr,   // ||
	AssignAdd, // +=
	AssignSub, // -=
	AssignMul, // *=
	AssignDiv, // /=
	AssignMod, // %=
	Less,	   // <
	More,	   // >
	Mul,	   // *
	Div,	   // /
	Mod,	   // %
	Add,	   // +
	Sub,	   // -
};

enum class DoubleSymbol: char {
	AddAdd, // ++
	SubSub, // --
};

class Syntax {
public:
	static bool IsTextSpace(char ch);
	static bool IsTextNewLine(char ch);
	static bool IsTextSpecialChar(char ch);
	static bool IsTextNumber(char ch);
	static bool IsGrammarStringSign(char ch);
	static bool IsGrammarEndSign(char ch);
	static bool IsSpecialSign(const std::string& value);
	static bool IsLeftBrcket(char ch);
	static bool IsRightBrcket(char ch);
	static bool IsVariableSelfAssignSymbol(MathSymbol value);
	static bool IsWordValidSymbol(char ch);
	static bool SearchNextInside(const std::string& src, std::size_t size, std::size_t pos);
	static bool SearchNextArray(const std::string& src, std::size_t size, std::size_t pos);

public:
	static int GetMathSymbolLevel(MathSymbol value);

public:
	static bool MatchImport(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchImportAs(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchEnum(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchExtends(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchObject(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchInsideSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchNew(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchObjectBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchObjectEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchArrayBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchArrayEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchDoubleSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, DoubleSymbol* symbol);
	static bool MatchNotSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchConst(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchReturn(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchSplitSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchFunction(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchBreak(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchContinue(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchTry(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchCatch(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchFinally(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchEcho(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchBlockBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchBlockEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchConditionIf(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchConditionElse(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchFor(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchForIn(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchWhile(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchDo(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchLoop(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchLeftBrcket(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchRightBrcket(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchMathSymbol(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, MathSymbol* symbol);
	static bool MatchNull(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchBool(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, bool* value);
	static bool MatchNumber(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, double* number);
	static bool MatchName(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* name);
	static bool MatchAssign(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchComment(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchCommentBlockBegin(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchCommentBlockEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchVariableDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchVariableSet(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchSign(const std::unordered_set<std::string>& sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchSign(const std::vector<std::string>& sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchSign(const std::string& sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchSign(char sign, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool MatchPair(const std::string& signLeft, const std::string& signRight, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* result);
	static bool MatchPair(char signLeft, char signRight, const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, std::string* result);

public:
};
} // namespace peak