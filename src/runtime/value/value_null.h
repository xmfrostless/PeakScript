/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "value.h"

namespace peak { 

class ValueNull : public TypeValue<ValueNull> {
public:
	static const std::shared_ptr<ValueNull> DEFAULT_VALUE;
	virtual std::string ToString() const override;
	virtual std::string ToRawString() const override;
	virtual std::shared_ptr<Value> Clone() const override;
};

} // namespace peak