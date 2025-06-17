#include "value_calculate.h"
#include "value_tool.h"

using namespace peak;

std::shared_ptr<Value> ValueCalculateAdd::Calculate(Value* left, Value* right) const {
	if (ValueTool::IsString(left) || ValueTool::IsString(right)) {
		return std::make_shared<ValueString>(ValueTool::ToString(left) + ValueTool::ToString(right));
	}
	if (ValueTool::IsNumber(left) && ValueTool::IsNumber(right)) {
		return std::make_shared<ValueNumber>(
			static_cast<ValueNumber*>(left)->GetValue() +
			static_cast<ValueNumber*>(right)->GetValue());
	}
	return nullptr;
}
std::shared_ptr<Value> ValueCalculateSub::Calculate(Value* left, Value* right) const {
	if (ValueTool::IsNumber(left) && ValueTool::IsNumber(right)) {
		return std::make_shared<ValueNumber>(
			static_cast<ValueNumber*>(left)->GetValue() -
			static_cast<ValueNumber*>(right)->GetValue());
	}
	return nullptr;
}
std::shared_ptr<Value> ValueCalculateMul::Calculate(Value* left, Value* right) const {
	if (ValueTool::IsNumber(left) && ValueTool::IsNumber(right)) {
		return std::make_shared<ValueNumber>(
			static_cast<ValueNumber*>(left)->GetValue() *
			static_cast<ValueNumber*>(right)->GetValue());
	}
	return nullptr;
}
std::shared_ptr<Value> ValueCalculateDiv::Calculate(Value* left, Value* right) const {
	if (ValueTool::IsNumber(left) && ValueTool::IsNumber(right)) {
		return std::make_shared<ValueNumber>(
			static_cast<ValueNumber*>(left)->GetValue() /
			static_cast<ValueNumber*>(right)->GetValue());
	}
	return nullptr;
}
std::shared_ptr<Value> ValueCalculateMod::Calculate(Value* left, Value* right) const {
	if (ValueTool::IsInteger(left) && ValueTool::IsInteger(right)) {
		return std::make_shared<ValueNumber>(static_cast<double>(
			static_cast<long long>(static_cast<ValueNumber*>(left)->GetValue()) %
			static_cast<long long>(static_cast<ValueNumber*>(right)->GetValue())));
	}
	return nullptr;
}

std::shared_ptr<Value> ValueCalculateSame::Calculate(Value* left, Value* right) const {
	return std::make_shared<ValueBool>(ValueTool::Equal(left, right));
}
std::shared_ptr<Value> ValueCalculateNotSame::Calculate(Value* left, Value* right) const {
	return std::make_shared<ValueBool>(!ValueTool::Equal(left, right));
}
std::shared_ptr<Value> ValueCalculateMore::Calculate(Value* left, Value* right) const {
	if (!ValueTool::CanMore(left, right)) {
		return nullptr;
	}
	return std::make_shared<ValueBool>(ValueTool::More(left, right));
}
std::shared_ptr<Value> ValueCalculateSameOrMore::Calculate(Value* left, Value* right) const {
	if (!ValueTool::CanMore(left, right)) {
		return nullptr;
	}
	return std::make_shared<ValueBool>(ValueTool::More(left, right) || ValueTool::Equal(left, right));
}
std::shared_ptr<Value> ValueCalculateLess::Calculate(Value* left, Value* right) const {
	if (!ValueTool::CanMore(left, right)) {
		return nullptr;
	}
	return std::make_shared<ValueBool>(ValueTool::More(right, left));
}
std::shared_ptr<Value> ValueCalculateSameOrLess::Calculate(Value* left, Value* right) const {
	if (!ValueTool::CanMore(left, right)) {
		return nullptr;
	}
	return std::make_shared<ValueBool>(ValueTool::More(right, left) || ValueTool::Equal(right, left));
}

std::shared_ptr<Value> ValueCalculateLogicAnd::Calculate(Value* left, Value* right) const {
	return std::make_shared<ValueBool>(ValueTool::ToLogic(left) && ValueTool::ToLogic(right));
}
std::shared_ptr<Value> ValueCalculateLogicOr::Calculate(Value* left, Value* right) const {
	return std::make_shared<ValueBool>(ValueTool::ToLogic(left) || ValueTool::ToLogic(right));
}