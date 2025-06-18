/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak { 

class Value {
public:
	virtual ~Value() {}
	virtual std::type_index GetType() const = 0;
	virtual std::shared_ptr<Value> Clone() const = 0;
	virtual std::string ToString() const = 0;
	virtual std::string ToRawString() const = 0;
};

template <typename T>
class TypeValue : public Value {
public:
	static std::type_index TYPE_INDEX;
	virtual std::type_index GetType() const final {
		return TypeValue<T>::TYPE_INDEX;
	}
};

template <typename T>
std::type_index TypeValue<T>::TYPE_INDEX = typeid(T);

} // namespace peak