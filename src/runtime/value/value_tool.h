/*
	PeakScript: https://github.com/Jenocn/PeakScript
	By Jenocn: https://jenocn.github.io
*/

#pragma once

#include "value.h"
#include "value_array.h"
#include "value_bool.h"
#include "value_function.h"
#include "value_null.h"
#include "value_number.h"
#include "value_object.h"
#include "value_string.h"

namespace peak {

class Value;

class ValueTool {
public:
	static bool IsNull(Value* value);
	static bool IsBool(Value* value);
	static bool IsNumber(Value* value);
	static bool IsString(Value* value);
	static bool IsFunction(Value* value);
	static bool IsArray(Value* value);
	static bool IsObject(Value* value);
	static std::string ToString(Value* value);
	static std::string ToTypeString(Value* value);
	static bool ToLogic(Value* value);
	static bool Equal(Value* a, Value* b);
	static bool More(Value* a, Value* b);
	static bool CanMore(Value* a, Value* b);

	static bool IsInteger(Value* value);
};

} // namespace peak