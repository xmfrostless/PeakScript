#include "builtin_module.h"
#include "runtime/space.h"
#include "runtime/module.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"
#include "runtime/system.h"
#include "builtin_module_type.h"
#include "builtin_module_strings.h"
#include "builtin_module_arrays.h"
#include "builtin_module_time.h"
#include "builtin_module_console.h"
#include "builtin_module_file.h"
#include "builtin_module_math.h"
#include "builtin_module_random.h"
#include "builtin_module_algorithm.h"
#include "builtin_module_peak.h"

using namespace peak;

BuiltinModule* BuiltinModule::GetInstance() {
	static BuiltinModule _ins;
	return &_ins;
}
std::shared_ptr<Module> BuiltinModule::FindModule(const std::string& name) const {
	auto ite = _modules.find(name);
	if (ite != _modules.end()) {
		return ite->second;
	}
	return nullptr;
}
BuiltinModule::BuiltinModule() {
	_modules.emplace("type", BuiltModuleType::CreateModule());
	_modules.emplace("strings", BuiltModuleStrings::CreateModule());
	_modules.emplace("arrays", BuiltModuleArrays::CreateModule());
	_modules.emplace("time", BuiltModuleTime::CreateModule());
	_modules.emplace("console", BuiltModuleConsole::CreateModule());
	_modules.emplace("file", BuiltModuleFile::CreateModule());
	_modules.emplace("math", BuiltModuleMath::CreateModule());
	_modules.emplace("random", BuiltModuleRandom::CreateModule());
	_modules.emplace("algorithm", BuiltModuleAlgorithm::CreateModule());
	_modules.emplace("peak", BuiltModulePeak::CreateModule());
}