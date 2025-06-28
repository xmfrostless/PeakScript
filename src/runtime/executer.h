/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"
#include "grammar/parse.h"

namespace peak {

class Space;
class Variable;
class Value;

class Executer {
	MAKE_NON_COPYABLE(Executer);
public:
	static std::unique_ptr<Executer> Create(const std::string& src);

public:
	Executer(std::unique_ptr<ParseData> data);
	~Executer();
	
	bool Execute();
	std::shared_ptr<Value> ExecuteExpression(const std::string& src);

	std::shared_ptr<Space> GetSpace() const;

	std::shared_ptr<Variable> FindVariable(const std::string& name) const;
	bool AddVariable(std::shared_ptr<Variable> variable);

private:
	std::unique_ptr<ParseData> _parseData{nullptr};
	std::shared_ptr<Space> _space{nullptr};
	std::shared_ptr<Space> _outsideSpace{nullptr};
};

} // namespace peak