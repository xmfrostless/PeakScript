/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"
#include "runtime/executer.h"

namespace peak {

class Variable;
class Space;
class Value;
class ModulePool;

class Script {
	MAKE_NON_COPYABLE(Script);
public:
	static std::unique_ptr<Script> Load(const std::string& src);
	static std::unique_ptr<Script> LoadFile(const std::string& filename);
	static void LocateLogger(std::function<void(const std::string&)> logger);
	static void LocateLogger(std::function<void(const std::string&)> echo, std::function<void(const std::string&)> errorLogger);
	static void LocateLoadText(std::function<std::string(const std::string&)> func);
	static ModulePool* GetModulePool();

public:
	Script(std::unique_ptr<Executer> executer);
	bool Execute();
	std::shared_ptr<Value> ExecuteExpression(const std::string& src);
	std::shared_ptr<Space> GetSpace() const;
	std::shared_ptr<Variable> FindVariable(const std::string& name) const;
	bool AddVariable(std::shared_ptr<Variable> variable);

	std::shared_ptr<Value> ExecuteFunction(const std::string& name, const std::vector<std::shared_ptr<Value>>& args);

private:
	std::unique_ptr<Executer> _executer{nullptr};
};
} // namespace peak