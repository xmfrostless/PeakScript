#include "error_logger.h"

using namespace peak;

std::function<void(const std::string&)> ErrorLogger::_logger = [](const std::string& message) {
	std::cout << message << std::endl;
};

std::unordered_map<ErrorRuntimeCode, std::string> ErrorLogger::_errorCodeNameMap = {
	{ErrorRuntimeCode::Space, "Space"},
	{ErrorRuntimeCode::Variable, "Variable"},
	{ErrorRuntimeCode::Block, "Block, \"{ }\""},
	{ErrorRuntimeCode::Condition, "Condition, \"if-else\""},
	{ErrorRuntimeCode::DoWhile, "DoWhile, \"do-while\""},
	{ErrorRuntimeCode::Echo, "Echo, \"echo\""},
	{ErrorRuntimeCode::Expression, "Expression"},
	{ErrorRuntimeCode::ExpressionDouble, "ExpressionDouble, \"++\",\"--\""},
	{ErrorRuntimeCode::FunctionCall, "FunctionCall, \"function()\""},
	{ErrorRuntimeCode::Math, "Math, \"math expression\""},
	{ErrorRuntimeCode::ExpressionNot, "ExpressionNot, \"!\""},
	{ErrorRuntimeCode::SelfAssign, "SelfAssign, \"+=, -=, *=, /=, %= ...\""},
	{ErrorRuntimeCode::Array, "Array, \"[ ]\""},
	{ErrorRuntimeCode::ExpressionVariable, "ExpressionVariable, \"variable\""},
	{ErrorRuntimeCode::For, "For, \"for\""},
	{ErrorRuntimeCode::Foreach, "Foreach, \"for-in\""},
	{ErrorRuntimeCode::FunctionDefine, "FunctionDefine, \"function { }\""},
	{ErrorRuntimeCode::Loop, "Loop, \"loop\""},
	{ErrorRuntimeCode::Return, "Return, \"return\""},
	{ErrorRuntimeCode::TryCatchFinally, "TryCatchFinally, \"try-catch-finally\""},
	{ErrorRuntimeCode::VariableAssign, "VariableAssign, \"=\""},
	{ErrorRuntimeCode::VariableDefine, "VariableDefine, \"var, const\""},
	{ErrorRuntimeCode::VariableSet, "VariableSet, \"set\""},
	{ErrorRuntimeCode::While, "While, \"while\""},
	{ErrorRuntimeCode::VariableNameAnalysis, "VariableNameAnalysis, \"variable-name\""},
	{ErrorRuntimeCode::VariableArrayItemAnalysis, "VariableArrayItemAnalysis, \"array [ ]\""},
	{ErrorRuntimeCode::VariableInsideAnalysis, "VariableInsideAnalysis, \"inside.value\""},
	{ErrorRuntimeCode::New, "New, \"new\""},
	{ErrorRuntimeCode::ObjectDefine, "ObjectDefine, \"object\""},
	{ErrorRuntimeCode::Inside, "Inside, \"inside.value\""},
	{ErrorRuntimeCode::EnumDefine, "EnumDefine, \"enum\""},
	{ErrorRuntimeCode::Import, "Import, \"import\""},
};

void ErrorLogger::Locate(std::function<void(const std::string&)> logger) {
	_logger = logger;
}

void ErrorLogger::Log(const std::string& message) {
	_logger("[ERROR]: " + message);
}

void ErrorLogger::LogParseError(const std::string& src, std::size_t size, std::size_t pos) {
	std::size_t lineNum = 0;
	std::size_t save0 = 0;
	std::size_t save1 = 0;
	for (std::size_t i = 0; i < size; ++i) {
		if ((src[i] == '\n') || (i == size - 1)) {
			++lineNum;
			if (i >= pos) {
				save1 = i + 1;
				break;
			}
			save0 = i + 1;
			save1 = save0;
		}
	}
	Log("[" + std::to_string(lineNum) + "," + std::to_string(save1 - save0) + "]: " + src.substr(save0, save1 - save0));
}

void ErrorLogger::LogRuntimeError(ErrorRuntimeCode code, const std::string& desc) {
	auto ite = _errorCodeNameMap.find(code);
	if (ite != _errorCodeNameMap.end()) {
		LogRuntimeError(ite->second + ": " + desc);
	} else {
		LogRuntimeError(desc);
	}
}

void ErrorLogger::LogRuntimeError(const std::string& desc) {
	Log("RUNTIME: " + desc);
}
