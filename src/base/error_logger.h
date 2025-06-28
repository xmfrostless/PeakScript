/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "common.h"

namespace peak {

enum class ErrorRuntimeCode {
	None = 0,
	Space,
	Variable,
	Block,
	Condition,
	DoWhile,
	Echo,
	Expression,
	ExpressionDouble,
	FunctionCall,
	Math,
	ExpressionNot,
	SelfAssign,
	Array,
	ExpressionVariable,
	For,
	Foreach,
	FunctionDefine,
	Loop,
	Return,
	TryCatchFinally,
	VariableAssign,
	VariableDefine,
	VariableSet,
	While,
	VariableNameAnalysis,
	VariableArrayItemAnalysis,
	VariableInsideAnalysis,
	New,
	ObjectDefine,
	Inside,
	EnumDefine,
	Import,
	Export,
};

class ErrorLogger {
public:
	static void Locate(std::function<void(const std::string&)> logger);
	static void Log(const std::string& message);
	static void LogParseError(const std::string& src, std::size_t size, std::size_t pos);
	static void LogRuntimeError(ErrorRuntimeCode code, const std::string& desc);
	static void LogRuntimeError(const std::string& desc);

private:
	static std::function<void(const std::string&)> _logger;
	static std::unordered_map<ErrorRuntimeCode, std::string> _errorCodeNameMap;
};

} // namespace peak