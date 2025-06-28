/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include <memory>
#include <list>
#include "base/error_logger.h"
#include "syntax.h"

namespace peak {

class Sentence;
class SentenceExpression;
class IValueCalculate;

struct ParseData {
	bool bSuccess{false};
	std::vector<std::unique_ptr<Sentence>> sentenceList;
};

class Parse {
public:
	static std::unique_ptr<ParseData> Load(const std::string& src);
	static std::unique_ptr<SentenceExpression> LoadExpression(const std::string& src);

private:
	static bool Jump(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool JumpEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

	static bool JumpTextSpace(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool JumpTextSpaceAndEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool JumpComment(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static bool JumpCommentBlock(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

	static std::unique_ptr<Sentence> ParseSentence(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

private:
	static std::unique_ptr<SentenceExpression> _ParseExpression(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseExpressionMath(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseExpressionMathBracket(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, bool bBracket);
	static std::unique_ptr<SentenceExpression> _ParseExpressionValue(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseVariable(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

	static std::unique_ptr<SentenceExpression> __ParseExpressionValueForArrayValue(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> __ParseExpressionInsideHeader(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> __ParseExpressionInsideAppend(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

private: // Sentence
	static std::unique_ptr<Sentence> _ParseImport(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseReturn(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseFunctionDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseEnumDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseObjectDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseVariableDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseVariableAssign(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseVariableSet(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseExpressionToEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseCondition(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseLoop(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseFor(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseForeach(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseWhile(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseDoWhile(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseBlock(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseEcho(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseTryCatchFinally(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<Sentence> _ParseLoopControl(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

private: // Expression Value
	static std::unique_ptr<SentenceExpression> _ParseString(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseNumber(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseBool(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseNull(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseArray(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseArrayItem(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseVariableName(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseFunctioCall(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseDoubleExpression(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseNotExpression(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseNew(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);
	static std::unique_ptr<SentenceExpression> _ParseInside(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos);

private:
	static IValueCalculate* _GetCalculate(MathSymbol symbol);
	static IValueCalculate* _GetCalculate(DoubleSymbol symbol);

private:
	using SentenceParseList = std::vector<std::function<std::unique_ptr<Sentence>(const std::string&, std::size_t, std::size_t, std::size_t*)>>;
	using ExpressionParseList = std::vector<std::function<std::unique_ptr<SentenceExpression>(const std::string&, std::size_t, std::size_t, std::size_t*)>>;

	static SentenceParseList _sentenceParseList;
	static ExpressionParseList _sentenceValueParseList;
	static ExpressionParseList _sentenceVariableParseList;

	static ExpressionParseList __sentenceArrayItemParseList;
	static ExpressionParseList __sentenceInsideHeaderParseList;
	static ExpressionParseList __sentenceInsideAppendParseList;
};
} // namespace peak