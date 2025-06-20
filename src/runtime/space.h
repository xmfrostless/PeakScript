/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

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

	SpaceType GetSpaceType() const;
	std::unordered_map<std::string, std::shared_ptr<Variable>>& GetVariables();

private:
	SpaceType _spaceType{SpaceType::None};
	std::shared_ptr<Space> _parent{nullptr};
	std::list<std::shared_ptr<Space>> _spaceOfUsing;
	std::unordered_map<std::string, std::shared_ptr<Variable>> _variables;
};
} // namespace peak