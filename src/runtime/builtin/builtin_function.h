/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak {

class Variable;

class BuiltInFunction {
public:
	static BuiltInFunction* GetInstance();

	std::shared_ptr<Variable> FindVariable(const std::string& name) const;

private:
	BuiltInFunction();

private:
	std::unordered_map<std::string, std::shared_ptr<Variable>> _variables;
};
} // namespace peak