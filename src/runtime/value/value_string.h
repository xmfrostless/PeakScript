/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "value.h"

namespace peak { 

class ValueString : public TypeValue<ValueString> {
public:
	ValueString(const std::string& value);
	std::string& GetValue();
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;

private:
	std::string _value;
};

} // namespace peak