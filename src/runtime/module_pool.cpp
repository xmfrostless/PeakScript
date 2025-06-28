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

std::shared_ptr<Module> ModulePool::UseModule(const std::string& moduleName) {
	std::shared_ptr<Module> retModule { nullptr };
	std::string saveKey;
	auto ite = _modulesMap.find(moduleName);
	if (ite != _modulesMap.end()) {
		saveKey = moduleName;
		retModule = ite->second;
	} else {
		std::string absPath;
		const std::string& src = System::OpenSrc(moduleName, absPath);
		if (!absPath.empty()) {
			auto srcIte = _modulesMap.find(absPath);
			if (srcIte != _modulesMap.end()) {
				retModule = srcIte->second;
				saveKey = std::move(absPath);
			} else {
				auto executer = Executer::Create(src);
				if (executer) {
					retModule = std::make_shared<Module>(std::move(executer));
					_modulesMap.emplace(absPath, retModule);
					saveKey = std::move(absPath);
				}
			}
		}
	}
	if (!retModule) {
		retModule = BuiltinModule::GetInstance()->FindModule(moduleName);
		if (!retModule) {
			return nullptr;
		}
	}
	if (!retModule->IsExecuted()) {
		if (!retModule->Execute()) {
			if (!saveKey.empty()) {
				_modulesMap.erase(saveKey);
			}
			return nullptr;
		}
	}
	return retModule;
}
