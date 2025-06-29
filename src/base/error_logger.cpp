#include "error_logger.h"

using namespace peak;

std::function<void(const std::string&)> ErrorLogger::_logger = [](const std::string& message) {
	std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
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
	std::size_t left = 0;
	std::size_t right = 0;
	for (std::size_t i = 0; i < size; ++i) {
		if ((src[i] == '\n') || (i == size - 1)) {
			++lineNum;
			if (i >= pos) {
				right = i;
				break;
			}
			left = i + 1;
			right = left;
		}
	}
	auto line = std::regex_replace(src.substr(left, right - left), std::regex("^\\s+|\\s+$"), "");
	std::ostringstream oss;
	oss << "|line:" << std::setw(3) << std::setfill(' ') << std::right << lineNum << "|: " << line;
	Log(oss.str());
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
