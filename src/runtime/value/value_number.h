/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "value.h"

namespace peak { 

class ValueNumber : public TypeValue<ValueNumber> {
public:
	ValueNumber(double value);
	double GetValue() const;
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;

private:
	double _value{0};
};

} // namespace peak