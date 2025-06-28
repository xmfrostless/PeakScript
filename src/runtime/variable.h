/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak {

class Value;

enum class VariableAttribute {
	None,
	Const,
};

class Variable {
public:
	Variable(const std::string& name, VariableAttribute attribute, std::shared_ptr<Value> value = nullptr);
	Variable(const std::string& name, std::size_t hashCode, VariableAttribute attribute, std::shared_ptr<Value> value = nullptr);

	VariableAttribute GetAttribute() const;
	std::size_t GetHashCode() const;
	const std::string& GetName() const;

	bool SetValue(std::shared_ptr<Value> value);
	std::shared_ptr<Value> GetValue() const;
	std::shared_ptr<Variable> Clone() const;

private:
	std::string _name;
	std::size_t _hashCode { 0 };
	std::shared_ptr<Value> _value { nullptr };
	VariableAttribute _attribute { VariableAttribute::None };
};
} // namespace peak