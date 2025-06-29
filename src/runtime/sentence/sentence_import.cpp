#include "sentence_import.h"
#include "runtime/module.h"
#include "runtime/module_pool.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"

using namespace peak;

SentenceImport::SentenceImport(const std::string& moduleName, const std::string& alias)
	: _moduleName(moduleName), _alias(alias) {
	_aliasHashCode = HashFunction::String(_alias);
}
ExecuteResult SentenceImport::Execute(std::shared_ptr<Space> space) {
	if (space->GetSpaceType() != SpaceType::None) {
		ErrorLogger::LogRuntimeError(_moduleName);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Import, "The module \"" + _moduleName + "\" can't import here!");
		return ExecuteResult::Failed;
	}
	auto [code, module] = ModulePool::GetInstance()->UseModule(_moduleName);
	if (!module) {
		ErrorLogger::LogRuntimeError(_moduleName);
		if (code == RuntimeCodeEx::Import_Not_Found) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Import, "The module \"" + _moduleName + "\" not found!");
		} else if (code == RuntimeCodeEx::Import_Execute_Error) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Import, "The module \"" + _moduleName + "\" execute error!");
		}
		return ExecuteResult::Failed;
	}

	if (!_alias.empty()) {
		auto variable = std::make_shared<Variable>(_alias, _aliasHashCode, VariableAttribute::None, std::make_shared<ValueObject>(module));
		if (!space->AddVariable(variable)) {
			ErrorLogger::LogRuntimeError(_moduleName);
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Import, "The module \"" + _moduleName + "\" already exists!");
			return ExecuteResult::Failed;
		}
	} else {
		space->AddUsingSpace(module->GetSpace());
	}

	return ExecuteResult::Successed;
}