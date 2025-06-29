#include "value_number.h"

using namespace peak;

ValueNumber::ValueNumber(double value)
	: _value(value) {
}

double ValueNumber::GetValue() const {
	return _value;
}
std::string ValueNumber::ToString() const {
	std::array<char, 32> buffer;
	auto [ptr, ec] = std::to_chars(buffer.data(), buffer.data() + buffer.size(), _value, std::chars_format::fixed);
	if (ec != std::errc()) {
		return "0";
	}
	return std::string(buffer.data(), ptr);
}
std::string ValueNumber::ToRawString() const {
	return ToString();
}

std::shared_ptr<Value> ValueNumber::Clone() const {
	return std::make_shared<ValueNumber>(_value);
}
