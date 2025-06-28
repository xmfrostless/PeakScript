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
	Space(SpaceType spaceType, std::shared_ptr<Space> outSpace);

public:
	std::shared_ptr<Space> CopySpace() const;
	void Clear();

	void AddUsingSpace(std::shared_ptr<Space> space);
	bool AddVariable(std::shared_ptr<Variable> value);

	std::shared_ptr<Variable> FindVariable(std::size_t hashCode) const;
	std::shared_ptr<Variable> GetVariableInSelf(std::size_t hashCode) const;

	std::shared_ptr<Variable> FindVariable(const std::string& name) const;
	std::shared_ptr<Variable> GetVariableInSelf(const std::string& name) const;

	std::shared_ptr<Value> FindVariableValue(const std::string& name) const;
	std::shared_ptr<ValueArray> FindVariableValueAsArray(const std::string& name) const;
	std::shared_ptr<ValueBool> FindVariableValueAsBool(const std::string& name) const;
	std::shared_ptr<ValueFunction> FindVariableValueAsFunction(const std::string& name) const;
	std::shared_ptr<ValueNull> FindVariableValueAsNull(const std::string& name) const;
	std::shared_ptr<ValueNumber> FindVariableValueAsNumber(const std::string& name) const;
	std::shared_ptr<ValueObject> FindVariableValueAsObject(const std::string& name) const;
	std::shared_ptr<ValueString> FindVariableValueAsString(const std::string& name) const;

	SpaceType GetSpaceType() const;
	std::unordered_map<std::size_t, std::shared_ptr<Variable>>& GetVariables();

private:
	SpaceType _spaceType { SpaceType::None };
	std::shared_ptr<Space> _outSpace { nullptr };
	std::vector<std::shared_ptr<Space>> _usingSpace;
	std::unordered_map<std::size_t, std::shared_ptr<Variable>> _variables;
};
} // namespace peak