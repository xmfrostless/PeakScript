/*
	PeakScript: https://github.com/Jenocn/PeakScript
	By Jenocn: https://jenocn.github.io
*/

#pragma once

#include "value.h"

namespace peak {

class Space;

class ValueFunction : public TypeValue<ValueFunction> {
public:
	using FunctionType = std::function<std::shared_ptr<Value>(const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>)>;

public:
	ValueFunction(std::size_t paramSize, FunctionType func);
	ValueFunction(const std::vector<std::string>& params, FunctionType func);
	std::shared_ptr<Value> Call(const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space> space);
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;

	std::size_t GetParamSize() const;

private:
	ValueFunction() = default;

private:
	std::vector<std::string> _params;
	FunctionType _function;
};

} // namespace peak