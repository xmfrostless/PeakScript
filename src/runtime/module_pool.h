/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak {

class Module;

class ModulePool {
public:
	static ModulePool* GetInstance();

	bool AddModule(const std::string& moduleName, std::shared_ptr<Module> module);
	void RemoveModule(const std::string& moduleName);

	std::shared_ptr<Module> UseModule(const std::string& moduleName);

private:
	std::pair<std::string, std::string>* _LoadSrc(const std::string& filename);

private:
	std::unordered_map<std::string, std::shared_ptr<Module>> _modulesMap;
};

} // namespace peak