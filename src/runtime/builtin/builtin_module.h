/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"

namespace peak {

class Module;

class BuiltinModule {
public:
	static BuiltinModule* GetInstance();

	std::shared_ptr<Module> FindModule(const std::string& name) const;

private:
	BuiltinModule();

private:
	std::unordered_map<std::string, std::shared_ptr<Module>> _modules;
};
} // namespace peak