/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak {

struct ParseData;
class Space;
class Variable;

class Executer {
public:
	static std::shared_ptr<Executer> Create(const std::string& src);
	Executer(std::shared_ptr<ParseData> data);
	~Executer();
	
	bool Execute();

	std::shared_ptr<Space> GetSpace() const;

	std::shared_ptr<Variable> FindVariable(const std::string& name) const;
	bool AddVariable(std::shared_ptr<Variable> variable);

private:
	std::shared_ptr<ParseData> _parseData{nullptr};
	std::shared_ptr<Space> _space{nullptr};
	std::shared_ptr<Space> _outsideSpace{nullptr};
};

} // namespace peak