/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "value.h"

namespace peak { 

class Variable;
	
class ValueArray : public TypeValue<ValueArray> {
public:
	ValueArray();
	ValueArray(const std::vector<std::shared_ptr<Variable>>& value);
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;

	std::vector<std::shared_ptr<Variable>>& GetArray();

	void Reserve(std::size_t size);
	void EmplaceBack(std::shared_ptr<Variable> variable);
	void EmplaceBack(std::shared_ptr<Value> value);

private:
	std::vector<std::shared_ptr<Variable>> _value;
};

} // namespace peak