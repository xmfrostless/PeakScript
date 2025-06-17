#include "value_tool.h"

using namespace peak;

bool ValueTool::IsNull(Value* value) {
	return !value || (value->GetType() == ValueNull::TYPE_INDEX);
}
bool ValueTool::IsBool(Value* value) {
	return value && (value->GetType() == ValueBool::TYPE_INDEX);
}
bool ValueTool::IsNumber(Value* value) {
	return value && (value->GetType() == ValueNumber::TYPE_INDEX);
}
bool ValueTool::IsString(Value* value) {
	return value && (value->GetType() == ValueString::TYPE_INDEX);
}
bool ValueTool::IsFunction(Value* value) {
	return value && (value->GetType() == ValueFunction::TYPE_INDEX);
}
bool ValueTool::IsArray(Value* value) {
	return value && (value->GetType() == ValueArray::TYPE_INDEX);
}
bool ValueTool::IsObject(Value* value) {
	return value && (value->GetType() == ValueObject::TYPE_INDEX);
}

std::string ValueTool::ToString(Value* value) {
	if (!value) {
		return ValueNull::DEFAULT_VALUE->ToString();
	}
	return value->ToString();
}

std::string ValueTool::ToTypeString(Value* value) {
	static const std::unordered_map<std::type_index, std::string> TypeStringMap = {
		{ValueNull::TYPE_INDEX, "null"},
		{ValueBool::TYPE_INDEX, "bool"},
		{ValueArray::TYPE_INDEX, "array"},
		{ValueString::TYPE_INDEX, "string"},
		{ValueNumber::TYPE_INDEX, "number"},
		{ValueFunction::TYPE_INDEX, "function"},
		{ValueObject::TYPE_INDEX, "object"},
	};
	if (!value) {
		return ToTypeString(ValueNull::DEFAULT_VALUE.get());
	}
	auto ite = TypeStringMap.find(value->GetType());
	if (ite != TypeStringMap.end()) {
		return ite->second;
	}
	return "unknown";
}

bool ValueTool::ToLogic(Value* value) {
	if (IsNull(value)) {
		return false;
	}
	if (IsNumber(value)) {
		return static_cast<ValueNumber*>(value)->GetValue() != 0;
	}
	if (IsBool(value)) {
		return static_cast<ValueBool*>(value)->GetValue();
	}
	return true;
}

bool ValueTool::Equal(Value* a, Value* b) {
	if (a == b) {
		return true;
	}
	if (a->GetType() != b->GetType()) {
		return false;
	}
	if (IsBool(a)) {
		return static_cast<ValueBool*>(a)->GetValue() ==
			static_cast<ValueBool*>(b)->GetValue();
	}
	if (IsNumber(a)) {
		return static_cast<ValueNumber*>(a)->GetValue() ==
			static_cast<ValueNumber*>(b)->GetValue();
	}
	if (IsString(a)) {
		return static_cast<ValueString*>(a)->GetValue() ==
			static_cast<ValueString*>(b)->GetValue();
	}
	if (IsNull(a)) {
		return true;
	}
	return false;
}

bool ValueTool::More(Value* a, Value* b) {
	if (a->GetType() != b->GetType()) {
		return false;
	}
	if (IsNumber(a)) {
		return static_cast<ValueNumber*>(a)->GetValue() >
			static_cast<ValueNumber*>(b)->GetValue();
	}
	if (IsString(a)) {
		return static_cast<ValueString*>(a)->GetValue() >
			static_cast<ValueString*>(b)->GetValue();
	}
	return false;
}

bool ValueTool::CanMore(Value* a, Value* b) {
	if (a->GetType() != b->GetType()) {
		return false;
	}
	return IsNumber(a) || IsString(a);
}


bool ValueTool::IsInteger(Value* value) {
	if (!IsNumber(value)) {
		return false;
	}
	auto d = static_cast<ValueNumber*>(value)->GetValue();
	return std::trunc(d) == d;
}
