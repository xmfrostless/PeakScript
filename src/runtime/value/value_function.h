/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "value.h"

namespace peak {

class Space;

class ValueFunction : public TypeValue<ValueFunction> {
public:
	using FunctionType = std::function<std::shared_ptr<Value>(const std::vector<std::shared_ptr<Value>>&, std::shared_ptr<Space>)>;

public:
	ValueFunction() = default;
	ValueFunction(std::size_t paramSize, FunctionType func);
	ValueFunction(const std::vector<std::string>& params, FunctionType func);
	std::shared_ptr<Value> Call(const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space> space);
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;

	std::size_t GetParamSize() const;

private:
	std::vector<std::tuple<std::string, std::size_t>> _params;
	FunctionType _function;
};

} // namespace peak