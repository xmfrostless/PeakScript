#include "script.h"
#include "runtime/executer.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"
#include "runtime/space.h"
#include "runtime/module_pool.h"
#include "runtime/system.h"

using namespace peak;

std::shared_ptr<Script> Script::Load(const std::string& src) {
	auto executer = Executer::Create(src);
	if (executer) {
		return std::make_shared<Script>(executer);
	}
	return nullptr;
}

std::shared_ptr<Script> Script::LoadFile(const std::string& filename) {
	std::string absPath;
	return Load(System::OpenSrc(filename, absPath));
}

void Script::LocateLogger(std::function<void(const std::string&)> logger) {
	LocateLogger(logger, logger);
}
void Script::LocateLogger(std::function<void(const std::string&)> echo, std::function<void(const std::string&)> errorLogger) {
	System::LocateEcho(echo);
	ErrorLogger::Locate(errorLogger);
}

void Script::LocateLoadText(std::function<std::string(const std::string&)> func) {
	System::LocateLoadText(func);
}

ModulePool* Script::GetModulePool() {
	return ModulePool::GetInstance();
}

Script::Script(std::shared_ptr<Executer> executer)
	: _executer(executer) {
}

bool Script::Execute() {
	return _executer->Execute();
}

std::shared_ptr<Value> Script::ExecuteExpression(const std::string& src) {
	return _executer->ExecuteExpression(src);
}

std::shared_ptr<Space> Script::GetSpace() const {
	return _executer->GetSpace();
}

std::shared_ptr<Variable> Script::FindVariable(const std::string& name) const {
	return _executer->FindVariable(name);
}
bool Script::AddVariable(std::shared_ptr<Variable> variable) {
	return _executer->AddVariable(variable);
}
std::shared_ptr<Value> Script::ExecuteFunction(const std::string& name, const std::vector<std::shared_ptr<Value>>& args) {
	auto variable = _executer->FindVariable(name);
	if (variable && ValueTool::IsFunction(variable->GetValue().get())) {
		auto valueFunc = std::static_pointer_cast<ValueFunction>(variable->GetValue());
		return valueFunc->Call(args, GetSpace());
	}
	return ValueNull::DEFAULT_VALUE;
}

