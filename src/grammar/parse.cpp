#include "parse.h"
#include "runtime/sentence/analysis/expression_variable_analysis.h"
#include "runtime/sentence/sentence_block.h"
#include "runtime/sentence/sentence_condition.h"
#include "runtime/sentence/sentence_do_while.h"
#include "runtime/sentence/sentence_echo.h"
#include "runtime/sentence/sentence_enum_define.h"
#include "runtime/sentence/sentence_expression_double.h"
#include "runtime/sentence/sentence_expression_function_call.h"
#include "runtime/sentence/sentence_expression_inside.h"
#include "runtime/sentence/sentence_expression_math.h"
#include "runtime/sentence/sentence_expression_new.h"
#include "runtime/sentence/sentence_expression_not.h"
#include "runtime/sentence/sentence_expression_self_assign.h"
#include "runtime/sentence/sentence_expression_value_array.h"
#include "runtime/sentence/sentence_expression_variable.h"
#include "runtime/sentence/sentence_for.h"
#include "runtime/sentence/sentence_foreach.h"
#include "runtime/sentence/sentence_function_define.h"
#include "runtime/sentence/sentence_import.h"
#include "runtime/sentence/sentence_loop.h"
#include "runtime/sentence/sentence_loop_control.h"
#include "runtime/sentence/sentence_object_define.h"
#include "runtime/sentence/sentence_return.h"
#include "runtime/sentence/sentence_try_catch_finally.h"
#include "runtime/sentence/sentence_variable_assign.h"
#include "runtime/sentence/sentence_variable_define.h"
#include "runtime/sentence/sentence_variable_set.h"
#include "runtime/sentence/sentence_while.h"
#include "runtime/value/value_calculate.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"

using namespace peak;

Parse::SentenceParseList Parse::_sentenceParseList = {
	_ParseVariableAssign,
	_ParseVariableDefine,
	_ParseVariableSet,
	_ParseFunctionDefine,
	_ParseObjectDefine,
	_ParseEnumDefine,
	_ParseCondition,
	_ParseLoop,
	_ParseForeach,
	_ParseFor,
	_ParseWhile,
	_ParseDoWhile,
	_ParseBlock,
	_ParseEcho,
	_ParseLoopControl,
	_ParseReturn,
	_ParseImport,
	_ParseTryCatchFinally,
	_ParseExpressionToEnd,
	_ParseFunctioCall,
};
Parse::ExpressionParseList Parse::_sentenceValueParseList = {
	_ParseString,
	_ParseNumber,
	_ParseInside,
	_ParseArrayItem,
	_ParseArray,
	_ParseDoubleExpression,
	_ParseNotExpression,
	_ParseFunctioCall,
	_ParseVariableName,
	_ParseNew,
	_ParseBool,
	_ParseNull,
};

// target = value;
Parse::ExpressionParseList Parse::_sentenceVariableParseList = {
	_ParseInside,
	_ParseArrayItem,
	_ParseVariableName,
};

// target[]
Parse::ExpressionParseList Parse::__sentenceArrayItemParseList = {
	_ParseArray,
	_ParseFunctioCall,
	_ParseVariableName,
};

// target.
Parse::ExpressionParseList Parse::__sentenceInsideHeaderParseList = {
	_ParseNew,
	_ParseFunctioCall,
	_ParseArrayItem,
	_ParseVariableName,
};

// .target
Parse::ExpressionParseList Parse::__sentenceInsideAppendParseList = {
	_ParseArrayItem,
	_ParseFunctioCall,
	_ParseVariableName,
};
std::shared_ptr<ParseData> Parse::Load(const std::string& src) {
	auto retData = std::make_shared<ParseData>();
	retData->bSuccess = true;
	std::size_t pos = 0;
	auto size = src.size();
	while (pos < size) {
		JumpEnd(src, size, pos, &pos);
		if (pos >= size || src[pos] == '\0') {
			break;
		}
		auto parseSentence = ParseSentence(src, size, pos, &pos);
		if (!parseSentence) {
			retData->bSuccess = false;
			ErrorLogger::LogParseError(src, size, pos);
			break;
		}
		retData->sentenceList.emplace_back(parseSentence);
	}
	return retData;
}

bool Parse::Jump(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	bool bJump = false;
	bool bRet = false;
	do {
		bJump = false;
		bJump |= JumpTextSpace(src, size, pos, &pos);
		bJump |= JumpComment(src, size, pos, &pos);
		bJump |= JumpCommentBlock(src, size, pos, &pos);
		bRet |= bJump;
	} while (bJump);
	*nextPos = pos;
	return bRet;
}

bool Parse::JumpEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	bool bJump = false;
	bool bRet = false;
	do {
		bJump = false;
		bool bEnd = JumpTextSpaceAndEnd(src, size, pos, &pos);
		bJump |= bEnd;
		bJump |= JumpComment(src, size, pos, &pos);
		bJump |= JumpCommentBlock(src, size, pos, &pos);
		bRet |= bEnd;
	} while (bJump);
	*nextPos = pos;
	if (Syntax::IsGrammarEndSign('\n')) {
		bRet |= (pos >= size);
	}
	return bRet;
}

bool Parse::JumpTextSpace(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	auto beginPos = pos;
	while (pos < size) {
		if (!Syntax::IsTextSpace(src[pos])) {
			break;
		}
		++pos;
	}
	*nextPos = pos;
	return pos != beginPos;
}
bool Parse::JumpTextSpaceAndEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	bool bRet = false;
	while (pos < size) {
		char ch = src[pos];
		bool bEnd = Syntax::IsGrammarEndSign(ch);
		if (!bEnd && !Syntax::IsTextSpace(ch)) {
			break;
		}
		bRet |= bEnd;
		++pos;
	}
	*nextPos = pos;
	return bRet;
}
bool Parse::JumpComment(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (Syntax::MatchComment(src, size, pos, &pos)) {
		while (pos <= size) {
			if ((pos == size) || Syntax::IsTextNewLine(src[pos])) {
				*nextPos = pos;
				return true;
			}
			++pos;
		}
	}
	return false;
}

bool Parse::JumpCommentBlock(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	int findBegin = 0;
	if (Syntax::MatchCommentBlockBegin(src, size, pos, &pos)) {
		while (pos < size) {
			if (Syntax::MatchCommentBlockBegin(src, size, pos, &pos)) {
				++findBegin;
				continue;
			}
			if (Syntax::MatchCommentBlockEnd(src, size, pos, &pos)) {
				if (findBegin == 0) {
					*nextPos = pos;
					return true;
				}
				--findBegin;
				continue;
			}
			++pos;
		}
	}
	return false;
}

std::shared_ptr<Sentence> Parse::ParseSentence(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	for (auto func : _sentenceParseList) {
		auto parseData = func(src, size, pos, nextPos);
		if (parseData) {
			return parseData;
		}
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::_ParseExpression(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return _ParseExpressionMath(src, size, pos, nextPos);
}

std::shared_ptr<SentenceExpression> Parse::_ParseExpressionValue(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	for (auto func : _sentenceValueParseList) {
		auto value = func(src, size, pos, nextPos);
		if (value) {
			return value;
		}
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::_ParseVariable(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	for (auto func : _sentenceVariableParseList) {
		auto value = func(src, size, pos, nextPos);
		if (value) {
			return value;
		}
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::__ParseExpressionValueForArrayValue(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	for (auto func : __sentenceArrayItemParseList) {
		auto value = func(src, size, pos, nextPos);
		if (value) {
			return value;
		}
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::__ParseExpressionInsideHeader(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	for (auto func : __sentenceInsideHeaderParseList) {
		auto value = func(src, size, pos, nextPos);
		if (value) {
			return value;
		}
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::__ParseExpressionInsideAppend(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	for (auto func : __sentenceInsideAppendParseList) {
		auto value = func(src, size, pos, nextPos);
		if (value) {
			return value;
		}
	}
	return nullptr;
}

std::shared_ptr<Sentence> Parse::_ParseImport(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchImport(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);

	if (pos >= size) {
		return nullptr;
	}
	std::string moduleName;
	if (!Syntax::MatchString(src, size, pos, &pos, &moduleName)) {
		return nullptr;
	}

	Jump(src, size, pos, &pos);

	std::string alias { "" };
	if (Syntax::MatchImportAs(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchName(src, size, pos, &pos, &alias)) {
			return nullptr;
		}
	}

	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceImport>(moduleName, alias);
}

std::shared_ptr<Sentence> Parse::_ParseReturn(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchReturn(src, size, pos, &pos)) {
		return nullptr;
	}

	std::shared_ptr<SentenceExpression> expression { nullptr };
	if (Jump(src, size, pos, &pos)) {
		expression = _ParseExpression(src, size, pos, &pos);
	}

	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}

	*nextPos = pos;
	return std::make_shared<SentenceReturn>(expression);
}
std::shared_ptr<Sentence> Parse::_ParseFunctionDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchFunction(src, size, pos, &pos)) {
		return nullptr;
	}
	if (!Jump(src, size, pos, &pos)) {
		return nullptr;
	}
	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	if (!Syntax::MatchLeftBrcket(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	std::vector<std::string> params;
	std::string paramName;
	while (Syntax::MatchName(src, size, pos, &pos, &paramName)) {
		params.emplace_back(paramName);
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchSplitSymbol(src, size, pos, &pos)) {
			break;
		}
		Jump(src, size, pos, &pos);
	}
	if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	auto contentSentence = _ParseBlock(src, size, pos, &pos);
	if (!contentSentence) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceFunctionDefine>(name, params, contentSentence);
}

std::shared_ptr<Sentence> Parse::_ParseEnumDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchEnum(src, size, pos, &pos)) {
		return nullptr;
	}
	if (!Jump(src, size, pos, &pos)) {
		return nullptr;
	}
	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	if (!Syntax::MatchObjectBegin(src, size, pos, &pos)) {
		return nullptr;
	}
	std::set<std::string> checkNameSet;
	std::set<double> checkValueSet;
	std::list<std::pair<std::string, std::shared_ptr<ValueNumber>>> valueList;
	double indexValue = 0;
	while (true) {
		if (pos >= size) {
			return nullptr;
		}
		Jump(src, size, pos, &pos);
		if (Syntax::MatchObjectEnd(src, size, pos, &pos)) {
			break;
		}
		std::string valueName = "";
		if (!Syntax::MatchName(src, size, pos, &pos, &valueName)) {
			return nullptr;
		}
		Jump(src, size, pos, &pos);
		if (Syntax::MatchAssign(src, size, pos, &pos)) {
			Jump(src, size, pos, &pos);
			if (!Syntax::MatchNumber(src, size, pos, &pos, &indexValue)) {
				return nullptr;
			}
			Jump(src, size, pos, &pos);
		}

		auto valueNumber = std::make_shared<ValueNumber>(indexValue);
		if (!ValueTool::IsInteger(valueNumber.get())) {
			return nullptr;
		}

		if (checkNameSet.find(valueName) != checkNameSet.end()) {
			return nullptr;
		}
		if (checkValueSet.find(indexValue) != checkValueSet.end()) {
			return nullptr;
		}
		checkNameSet.emplace(valueName);
		checkValueSet.emplace(indexValue);
		valueList.emplace_back(valueName, valueNumber);

		Jump(src, size, pos, &pos);
		if (!Syntax::MatchSplitSymbol(src, size, pos, &pos)) {
			if (Syntax::MatchObjectEnd(src, size, pos, &pos)) {
				break;
			}
			return nullptr;
		}

		indexValue += 1;
	}
	*nextPos = pos;
	return std::make_shared<SentenceEnumDefine>(name, valueList);
}

std::shared_ptr<Sentence> Parse::_ParseObjectDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchObject(src, size, pos, &pos)) {
		return nullptr;
	}
	if (!Jump(src, size, pos, &pos)) {
		return nullptr;
	}
	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	std::string parentName = "";
	if (Syntax::MatchExtends(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchName(src, size, pos, &pos, &parentName)) {
			return nullptr;
		}
		Jump(src, size, pos, &pos);
	}

	if (!Syntax::MatchObjectBegin(src, size, pos, &pos)) {
		return nullptr;
	}

	std::list<std::shared_ptr<Sentence>> sentenceList;
	while (true) {
		if (pos >= size) {
			return nullptr;
		}
		Jump(src, size, pos, &pos);
		if (Syntax::MatchObjectEnd(src, size, pos, &pos)) {
			break;
		}
		auto sentence = ParseSentence(src, size, pos, &pos);
		if (!sentence) {
			return nullptr;
		}
		sentenceList.emplace_back(sentence);
	}
	*nextPos = pos;
	return std::make_shared<SentenceObjectDefine>(name, parentName, sentenceList);
}

std::shared_ptr<Sentence> Parse::_ParseVariableDefine(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	bool bConst = Syntax::MatchConst(src, size, pos, &pos);
	if (bConst && !Jump(src, size, pos, &pos)) {
		return nullptr;
	}
	if (Syntax::MatchVariableDefine(src, size, pos, &pos)) {
		if (!Jump(src, size, pos, &pos)) {
			return nullptr;
		}
	} else {
		if (!bConst) {
			return nullptr;
		}
	}

	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}

	auto beginPos = pos;
	Jump(src, size, pos, &pos);

	auto attribute = bConst ? VariableAttribute::Const : VariableAttribute::None;

	if (!Syntax::MatchAssign(src, size, pos, &pos)) {
		pos = beginPos;
		if (bConst) {
			return nullptr;
		}
		if (JumpEnd(src, size, pos, &pos)) {
			*nextPos = pos;
			return std::make_shared<SentenceVariableDefine>(name, attribute, nullptr);
		}
		return nullptr;
	}

	Jump(src, size, pos, &pos);
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}

	*nextPos = pos;
	return std::make_shared<SentenceVariableDefine>(name, attribute, expression);
}

std::shared_ptr<Sentence> Parse::_ParseVariableAssign(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	auto variableExpression = _ParseVariable(src, size, pos, &pos);
	if (!variableExpression) {
		return nullptr;
	}

	Jump(src, size, pos, &pos);
	if (!Syntax::MatchAssign(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceVariableAssign>(variableExpression, expression);
}

std::shared_ptr<Sentence> Parse::_ParseVariableSet(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchVariableSet(src, size, pos, &pos)) {
		return nullptr;
	}
	if (!Jump(src, size, pos, &pos)) {
		return nullptr;
	}

	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}

	auto beginPos = pos;
	Jump(src, size, pos, &pos);

	if (!Syntax::MatchAssign(src, size, pos, &pos)) {
		pos = beginPos;
		if (!JumpEnd(src, size, pos, &pos)) {
			return nullptr;
		}
		*nextPos = pos;
		return std::make_shared<SentenceVariableSet>(name, nullptr);
	}

	Jump(src, size, pos, &pos);
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}

	*nextPos = pos;
	return std::make_shared<SentenceVariableSet>(name, expression);
}

std::shared_ptr<Sentence> Parse::_ParseExpressionToEnd(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return expression;
}

std::shared_ptr<Sentence> Parse::_ParseCondition(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchConditionIf(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	bool bBracket = Syntax::MatchLeftBrcket(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
	}
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (bBracket) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}
	Jump(src, size, pos, &pos);
	auto sentence = ParseSentence(src, size, pos, &pos);
	if (!sentence) {
		return nullptr;
	}

	std::shared_ptr<Sentence> sentenceElse { nullptr };

	Jump(src, size, pos, &pos);
	if (Syntax::MatchConditionElse(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		sentenceElse = _ParseCondition(src, size, pos, &pos);
		if (!sentenceElse) {
			sentenceElse = ParseSentence(src, size, pos, &pos);
			if (!sentenceElse) {
				return nullptr;
			}
		}
	}

	*nextPos = pos;
	return std::make_shared<SentenceCondition>(expression, sentence, sentenceElse);
}

std::shared_ptr<Sentence> Parse::_ParseLoop(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchLoop(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);

	bool bBracket = Syntax::MatchLeftBrcket(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
	}

	if (Syntax::MatchVariableDefine(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
	}

	std::string name;
	auto savePos = pos;
	bool bMatchName = Syntax::MatchName(src, size, pos, &pos, &name);
	if (bMatchName) {
		Jump(src, size, pos, &pos);
	}

	if (Syntax::MatchForIn(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
	} else {
		if (bMatchName) {
			pos = savePos;
			name = "";
		}
	}

	auto condition = _ParseExpression(src, size, pos, &pos);
	if (!condition) {
		return nullptr;
	}

	if (bBracket) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}

	Jump(src, size, pos, &pos);
	auto sentence = ParseSentence(src, size, pos, &pos);
	if (!sentence) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceLoop>(name, condition, sentence);
}

std::shared_ptr<Sentence> Parse::_ParseFor(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchFor(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	bool bBracket = Syntax::MatchLeftBrcket(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
	}

	auto sentence0 = ParseSentence(src, size, pos, &pos);
	if (!sentence0) {
		sentence0 = _ParseExpression(src, size, pos, &pos);
		Jump(src, size, pos, &pos);
		if ((pos >= size) || !Syntax::IsGrammarEndSign(src[pos])) {
			return nullptr;
		}
		++pos;
	}
	auto expression0 = _ParseExpression(src, size, pos, &pos);
	{
		Jump(src, size, pos, &pos);
		if ((pos >= size) || !Syntax::IsGrammarEndSign(src[pos])) {
			return nullptr;
		}
		++pos;
	}
	auto expression1 = _ParseExpression(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}
	Jump(src, size, pos, &pos);
	auto content = ParseSentence(src, size, pos, &pos);
	if (!content && !JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}

	*nextPos = pos;
	return std::make_shared<SentenceFor>(sentence0, expression0, expression1, content);
}

std::shared_ptr<Sentence> Parse::_ParseForeach(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchFor(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);

	bool bBracket = Syntax::MatchLeftBrcket(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
	}

	if (Syntax::MatchVariableDefine(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
	}
	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}

	Jump(src, size, pos, &pos);
	std::string indexParam;
	if (Syntax::MatchSplitSymbol(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchName(src, size, pos, &pos, &indexParam)) {
			return nullptr;
		}
	}

	Jump(src, size, pos, &pos);
	if (!Syntax::MatchForIn(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}

	if (bBracket) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}
	Jump(src, size, pos, &pos);
	auto sentence = ParseSentence(src, size, pos, &pos);
	if (!sentence) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceForeach>(name, indexParam, expression, sentence);
}

std::shared_ptr<Sentence> Parse::_ParseWhile(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchWhile(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	bool bBracket = Syntax::MatchLeftBrcket(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
	}
	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (bBracket) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}
	Jump(src, size, pos, &pos);
	auto sentence = ParseSentence(src, size, pos, &pos);
	if (!sentence && !JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceWhile>(expression, sentence);
}

std::shared_ptr<Sentence> Parse::_ParseDoWhile(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchDo(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	auto sentence = ParseSentence(src, size, pos, &pos);
	if (!sentence) {
		return nullptr;
	}

	Jump(src, size, pos, &pos);
	if (!Syntax::MatchWhile(src, size, pos, &pos)) {
		return nullptr;
	}

	Jump(src, size, pos, &pos);
	bool bBracket = Syntax::MatchLeftBrcket(src, size, pos, &pos);
	if (bBracket) {
		Jump(src, size, pos, &pos);
	}

	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}
	if (bBracket) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}
	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceDoWhile>(expression, sentence);
}

std::shared_ptr<Sentence> Parse::_ParseBlock(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchBlockBegin(src, size, pos, &pos)) {
		return nullptr;
	}

	auto sentenceBlock = std::make_shared<SentenceBlock>();
	while (true) {
		if (pos >= size) {
			return nullptr;
		}
		JumpEnd(src, size, pos, &pos);
		if (Syntax::MatchBlockEnd(src, size, pos, &pos)) {
			break;
		}

		auto parseSentence = ParseSentence(src, size, pos, &pos);
		if (!parseSentence) {
			return nullptr;
		}
		sentenceBlock->Push(parseSentence);
	}

	*nextPos = pos;
	return sentenceBlock;
}

std::shared_ptr<Sentence> Parse::_ParseEcho(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchEcho(src, size, pos, &pos)) {
		return nullptr;
	}
	if (!Jump(src, size, pos, &pos)) {
		return nullptr;
	}

	auto expression = _ParseExpression(src, size, pos, &pos);
	if (!expression) {
		return nullptr;
	}

	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}

	*nextPos = pos;
	return std::make_shared<SentenceEcho>(expression);
}

std::shared_ptr<Sentence> Parse::_ParseTryCatchFinally(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchTry(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	auto sentence = ParseSentence(src, size, pos, &pos);
	if (!sentence) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);

	decltype(sentence) catchSentence = nullptr;
	if (Syntax::MatchCatch(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		catchSentence = ParseSentence(src, size, pos, &pos);
		if (!catchSentence) {
			return nullptr;
		}
		Jump(src, size, pos, &pos);
	}
	decltype(sentence) finallySentence = nullptr;
	if (Syntax::MatchFinally(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		finallySentence = ParseSentence(src, size, pos, &pos);
		if (!finallySentence) {
			return nullptr;
		}
	}
	*nextPos = pos;
	return std::make_shared<SentenceTryCatchFinally>(sentence, catchSentence, finallySentence);
}

std::shared_ptr<Sentence> Parse::_ParseLoopControl(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	std::shared_ptr<Sentence> sentence = nullptr;
	do {
		if (Syntax::MatchContinue(src, size, pos, &pos)) {
			sentence = std::make_shared<SentenceLoopControlContinue>();
			break;
		}
		if (Syntax::MatchBreak(src, size, pos, &pos)) {
			sentence = std::make_shared<SentenceLoopControlBreak>();
			break;
		}
		return nullptr;
	} while (false);
	if (!JumpEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return sentence;
}

std::shared_ptr<SentenceExpression> Parse::_ParseString(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	std::string temp;
	if (Syntax::MatchString(src, size, pos, &pos, &temp)) {
		*nextPos = pos;
		return std::make_shared<SentenceExpression>(std::make_shared<ValueString>(temp));
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::_ParseNumber(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	double temp { 0 };
	if (Syntax::MatchNumber(src, size, pos, nextPos, &temp)) {
		return std::make_shared<SentenceExpression>(std::make_shared<ValueNumber>(temp));
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::_ParseBool(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	bool temp { false };
	if (Syntax::MatchBool(src, size, pos, nextPos, &temp)) {
		return std::make_shared<SentenceExpression>(std::make_shared<ValueBool>(temp));
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::_ParseNull(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (Syntax::MatchNull(src, size, pos, nextPos)) {
		return std::make_shared<SentenceExpression>(std::make_shared<ValueNull>());
	}
	return nullptr;
}
std::shared_ptr<SentenceExpression> Parse::_ParseArray(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchArrayBegin(src, size, pos, &pos)) {
		return nullptr;
	}
	std::vector<std::shared_ptr<SentenceExpression>> temp;
	while (true) {
		Jump(src, size, pos, &pos);
		auto expression = _ParseExpression(src, size, pos, &pos);
		if (!expression) {
			break;
		}
		temp.emplace_back(expression);
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchSplitSymbol(src, size, pos, &pos)) {
			break;
		}
	}
	if (!Syntax::MatchArrayEnd(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceExpressionValueArray>(temp);
}

std::shared_ptr<SentenceExpression> Parse::_ParseArrayItem(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::SearchNextArray(src, size, pos)) {
		return nullptr;
	}
	auto valueExpression = __ParseExpressionValueForArrayValue(src, size, pos, &pos);
	if (!valueExpression) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	if (!Syntax::MatchArrayBegin(src, size, pos, &pos)) {
		return nullptr;
	}
	std::vector<std::shared_ptr<SentenceExpression>> indexExpressionVec;
	while (true) {
		Jump(src, size, pos, &pos);
		auto indexExpression = _ParseExpression(src, size, pos, &pos);
		if (!indexExpression) {
			return nullptr;
		}
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchArrayEnd(src, size, pos, &pos)) {
			return nullptr;
		}
		indexExpressionVec.emplace_back(indexExpression);

		auto savePos = pos;
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchArrayBegin(src, size, pos, &pos)) {
			pos = savePos;
			break;
		}
	}
	if (indexExpressionVec.empty()) {
		return nullptr;
	}
	*nextPos = pos;

	auto analysis = std::make_shared<ExpressionVariableAnalysisArrayItem>(valueExpression, indexExpressionVec);
	return std::make_shared<SentenceExpressionVariable>(analysis);
}

std::shared_ptr<SentenceExpression> Parse::_ParseVariableName(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	std::string name;
	if (Syntax::MatchName(src, size, pos, nextPos, &name)) {
		auto analysis = std::make_shared<ExpressionVariableAnalysisName>(name);
		return std::make_shared<SentenceExpressionVariable>(analysis);
	}
	return nullptr;
}

std::shared_ptr<SentenceExpression> Parse::_ParseFunctioCall(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	if (!Syntax::MatchLeftBrcket(src, size, pos, &pos)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	std::vector<std::shared_ptr<SentenceExpression>> args;
	while (true) {
		auto argExpression = _ParseExpression(src, size, pos, &pos);
		if (!argExpression) {
			break;
		}
		args.emplace_back(argExpression);
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchSplitSymbol(src, size, pos, &pos)) {
			break;
		}
		Jump(src, size, pos, &pos);
	}
	if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceExpressionFunctionCall>(name, args);
}

std::shared_ptr<SentenceExpression> Parse::_ParseDoubleExpression(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	DoubleSymbol symbol;
	bool bDouble = Syntax::MatchDoubleSymbol(src, size, pos, &pos, &symbol);
	bool bLast = !bDouble;
	auto variableSentence = _ParseVariable(src, size, pos, &pos);
	if (!variableSentence) {
		return nullptr;
	}
	if (!bDouble && !Syntax::MatchDoubleSymbol(src, size, pos, &pos, &symbol)) {
		return nullptr;
	}
	auto calculate = _GetCalculate(symbol);
	if (!calculate) {
		return nullptr;
	}
	*nextPos = pos;
	return std::make_shared<SentenceExpressionDouble>(variableSentence, calculate, bLast);
}

std::shared_ptr<SentenceExpression> Parse::_ParseNotExpression(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchNotSymbol(src, size, pos, &pos)) {
		return nullptr;
	}
	auto expression = _ParseExpressionValue(src, size, pos, &pos);
	if (!expression) {
		if (!Syntax::MatchLeftBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
		expression = _ParseExpressionMathBracket(src, size, pos, &pos, true);
		if (!expression) {
			return nullptr;
		}
	}
	*nextPos = pos;
	return std::make_shared<SentenceExpressionNot>(expression);
}

std::shared_ptr<SentenceExpression> Parse::_ParseNew(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::MatchNew(src, size, pos, &pos)) {
		return nullptr;
	}
	if (!Jump(src, size, pos, &pos)) {
		return nullptr;
	}
	std::string name;
	if (!Syntax::MatchName(src, size, pos, &pos, &name)) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	if (Syntax::MatchLeftBrcket(src, size, pos, &pos)) {
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			return nullptr;
		}
	}
	*nextPos = pos;
	return std::make_shared<SentenceExpressionNew>(name);
}

std::shared_ptr<SentenceExpression> Parse::_ParseInside(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	if (!Syntax::SearchNextInside(src, size, pos)) {
		return nullptr;
	}
	auto header = __ParseExpressionInsideHeader(src, size, pos, &pos);
	if (!header) {
		return nullptr;
	}
	Jump(src, size, pos, &pos);
	if (!Syntax::MatchInsideSymbol(src, size, pos, &pos)) {
		return nullptr;
	}
	std::vector<std::shared_ptr<SentenceExpression>> insides;
	bool bTailCall = false;
	while (true) {
		Jump(src, size, pos, &pos);
		auto append = __ParseExpressionInsideAppend(src, size, pos, &pos);
		if (!append) {
			return nullptr;
		}
		insides.emplace_back(append);

		auto savePos = pos;
		Jump(src, size, pos, &pos);
		if (!Syntax::MatchInsideSymbol(src, size, pos, &pos)) {
			bTailCall = (append->GetExpressionType() == ExpressionType::Function);
			pos = savePos;
			break;
		}
	}
	if (insides.empty()) {
		return nullptr;
	}
	*nextPos = pos;
	if (bTailCall) {
		return std::make_shared<SentenceExpressionInside>(header, insides);
	}
	auto analysis = std::make_shared<ExpressionVariableAnalysisInside>(header, insides);
	return std::make_shared<SentenceExpressionVariable>(analysis);
}

std::shared_ptr<SentenceExpression> Parse::_ParseExpressionMath(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos) {
	return _ParseExpressionMathBracket(src, size, pos, nextPos, false);
}

std::shared_ptr<SentenceExpression> Parse::_ParseExpressionMathBracket(const std::string& src, std::size_t size, std::size_t pos, std::size_t* nextPos, bool bBracket) {
	std::stack<std::shared_ptr<SentenceExpression>> expressionStack;
	std::stack<MathSymbol> symbolStack;

	static const auto calcLoopFunc = [](MathSymbol symbol, decltype(expressionStack)* stack0, decltype(symbolStack)* stack1) {
		int level = Syntax::GetMathSymbolLevel(symbol);
		while (!stack1->empty()) {
			int preLevel = Syntax::GetMathSymbolLevel(stack1->top());
			if (preLevel < level) {
				break;
			}
			if (stack0->size() < 2) {
				return false;
			}
			auto topSymbol = stack1->top();
			stack1->pop();
			auto vr = stack0->top();
			stack0->pop();
			auto vl = stack0->top();
			stack0->pop();
			auto calculate = _GetCalculate(topSymbol);
			if (!calculate) {
				return false;
			}
			std::shared_ptr<SentenceExpressionMath> expression { nullptr };
			if (Syntax::IsVariableSelfAssignSymbol(topSymbol)) {
				expression = std::make_shared<SentenceExpressionSelfAssign>(vl, vr, calculate);
			} else {
				expression = std::make_shared<SentenceExpressionMath>(vl, vr, calculate);
			}
			stack0->emplace(expression);
		}
		return true;
	};

	while (true) {
		bool bRet = false;
		Jump(src, size, pos, &pos);
		if (Syntax::MatchLeftBrcket(src, size, pos, &pos)) {
			Jump(src, size, pos, &pos);
			auto priorityExpression = _ParseExpressionMathBracket(src, size, pos, &pos, true);
			if (!priorityExpression) {
				return nullptr;
			}
			expressionStack.emplace(priorityExpression);
			bRet = true;
		} else {
			auto value = _ParseExpressionValue(src, size, pos, &pos);
			if (value) {
				expressionStack.emplace(value);
				bRet = true;
			}
		}

		if (!bRet) {
			break;
		}

		auto saveJumpBeginPos = pos;
		Jump(src, size, pos, &pos);

		if (pos >= size) {
			break;
		}

		if (bBracket && Syntax::MatchRightBrcket(src, size, pos, &pos)) {
			break;
		}

		auto symbol = MathSymbol::None;
		if (Syntax::MatchMathSymbol(src, size, pos, &pos, &symbol)) {
			bRet = false;
			if (!calcLoopFunc(symbol, &expressionStack, &symbolStack)) {
				return nullptr;
			}
			symbolStack.emplace(symbol);
		} else {
			pos = saveJumpBeginPos;
			break;
		}
		if (bRet) {
			break;
		}
	}

	if (!calcLoopFunc(MathSymbol::None, &expressionStack, &symbolStack)) {
		return nullptr;
	}

	if (expressionStack.size() != 1) {
		return nullptr;
	}

	*nextPos = pos;

	return expressionStack.top();
}

IValueCalculate* Parse::_GetCalculate(MathSymbol symbol) {
	switch (symbol) {
	case MathSymbol::Mul:
		return ValueCalculateMul::GetInstance();
	case MathSymbol::Div:
		return ValueCalculateDiv::GetInstance();
	case MathSymbol::Mod:
		return ValueCalculateMod::GetInstance();
	case MathSymbol::Add:
		return ValueCalculateAdd::GetInstance();
	case MathSymbol::Sub:
		return ValueCalculateSub::GetInstance();
	case MathSymbol::Less:
		return ValueCalculateLess::GetInstance();
	case MathSymbol::LessEqual:
		return ValueCalculateSameOrLess::GetInstance();
	case MathSymbol::Equal:
		return ValueCalculateSame::GetInstance();
	case MathSymbol::NotEqual:
		return ValueCalculateNotSame::GetInstance();
	case MathSymbol::More:
		return ValueCalculateMore::GetInstance();
	case MathSymbol::MoreEqual:
		return ValueCalculateSameOrMore::GetInstance();
	case MathSymbol::LogicAnd:
		return ValueCalculateLogicAnd::GetInstance();
	case MathSymbol::LogicOr:
		return ValueCalculateLogicOr::GetInstance();
	case MathSymbol::AssignAdd:
		return ValueCalculateAdd::GetInstance();
	case MathSymbol::AssignSub:
		return ValueCalculateSub::GetInstance();
	case MathSymbol::AssignMul:
		return ValueCalculateMul::GetInstance();
	case MathSymbol::AssignDiv:
		return ValueCalculateDiv::GetInstance();
	case MathSymbol::AssignMod:
		return ValueCalculateMod::GetInstance();
	case MathSymbol::None:
		break;
	default:
		break;
	}

	return nullptr;
}

IValueCalculate* Parse::_GetCalculate(DoubleSymbol symbol) {
	switch (symbol) {
	case DoubleSymbol::AddAdd:
		return ValueCalculateAdd::GetInstance();
	case DoubleSymbol::SubSub:
		return ValueCalculateSub::GetInstance();
	default:
		break;
	}
	return nullptr;
}
