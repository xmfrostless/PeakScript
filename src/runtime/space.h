/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"
#include "value/value_tool.h"

namespace peak {

class Variable;
class Module;

enum class SpaceType {
	None,
	Condition,
	Loop,
	Function,
	Object,
};

class Space {
public:
	Space(SpaceType spaceType);
	Space(SpaceType spaceType, std::shared_ptr<Space> parent);

public:
	std::shared_ptr<Space> CopySpace() const;
	void Clear();

	bool AddVariable(std::shared_ptr<Variable> value);
	void AddSpaceOfUsing(std::shared_ptr<Space> space);
	std::shared_ptr<Variable> FindVariable(const std::string& name) const;
	std::shared_ptr<Variable> FindVariableFromTop(const std::string& name) const;
	std::shared_ptr<Value> FindVariableValue(const std::string& name) const;
	std::shared_ptr<ValueArray> FindVariableValueAsArray(const std::string& name) const;
	std::shared_ptr<ValueBool> FindVariableValueAsBool(const std::string& name) const;
	std::shared_ptr<ValueFunction> FindVariableValueAsFunction(const std::string& name) const;
	std::shared_ptr<ValueNull> FindVariableValueAsNull(const std::string& name) const;
	std::shared_ptr<ValueNumber> FindVariableValueAsNumber(const std::string& name) const;
	std::shared_ptr<ValueObject> FindVariableValueAsObject(const std::string& name) const;
	std::shared_ptr<ValueString> FindVariableValueAsString(const std::string& name) const;


	SpaceType GetSpaceType() const;
	std::unordered_map<std::string, std::shared_ptr<Variable>>& GetVariables();

private:
	SpaceType _spaceType{SpaceType::None};
	std::shared_ptr<Space> _parent{nullptr};
	std::list<std::shared_ptr<Space>> _spaceOfUsing;
	std::unordered_map<std::string, std::shared_ptr<Variable>> _variables;
};
} // namespace peak