/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "value.h"

namespace peak { 

class ValueBool : public TypeValue<ValueBool> {
public:
	static const std::shared_ptr<ValueBool> TRUE;
	static const std::shared_ptr<ValueBool> FALSE;

	ValueBool(bool value);
	bool GetValue() const;
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;

private:
	bool _value{false};
};

} // namespace peak