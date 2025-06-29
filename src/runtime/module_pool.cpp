#include "module_pool.h"
#include "executer.h"
#include "module.h"
#include "system.h"
#include "builtin/builtin_module.h"

using namespace peak;

ModulePool* ModulePool::GetInstance() {
	static ModulePool _ins;
	return &_ins;
}

bool ModulePool::AddModule(const std::string& moduleName, std::shared_ptr<Module> module) {
	if (!module) {
		return false;
	}
	auto ite = _modulesMap.find(moduleName);
	if (ite == _modulesMap.end()) {
		_modulesMap.emplace(moduleName, module);
		return true;
	}
	return false;
}

void ModulePool::RemoveModule(const std::string& moduleName) {
	auto ite = _modulesMap.find(moduleName);
	if (ite != _modulesMap.end()) {
		_modulesMap.erase(ite);
	}
}

std::tuple<RuntimeCodeEx, std::shared_ptr<Module>> ModulePool::UseModule(const std::string& moduleName) {
	std::shared_ptr<Module> retModule { nullptr };
	auto ite = _modulesMap.find(moduleName);
	if (ite != _modulesMap.end()) {
		retModule = ite->second;
	} else {
		std::string absPath;
		const std::string& src = System::OpenSrc(moduleName, absPath);
		if (!absPath.empty()) {
			auto srcIte = _modulesMap.find(absPath);
			if (srcIte != _modulesMap.end()) {
				retModule = srcIte->second;
			} else {
				auto executer = Executer::Create(src);
				if (executer) {
					retModule = std::make_shared<Module>(std::move(executer));
					_modulesMap.emplace(absPath, retModule);
				} else {
					return { RuntimeCodeEx::Import_Execute_Error, nullptr };
				}
			}
		} else {
			retModule = BuiltinModule::GetInstance()->FindModule(moduleName);
			if (!retModule) {
				return { RuntimeCodeEx::Import_Not_Found, nullptr };
			}
		}
	}

	if (!retModule->IsExecuted()) {
		if (!retModule->Execute()) {
			return { RuntimeCodeEx::Import_Execute_Error, nullptr };
		}
	}
	return { RuntimeCodeEx::None, retModule };
}
