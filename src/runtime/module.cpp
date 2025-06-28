#include "module.h"
#include "executer.h"

using namespace peak;

Module::Module(std::unique_ptr<Executer> executer)
	: _executer(std::move(executer)) {
	_space = _executer->GetSpace();
}
Module::Module(std::shared_ptr<Space> space)
	: _space(space),
	_bExecuted(true) {
}

bool Module::Execute() {
	_bExecuted = true;
	if (_executer) {
		return _executer->Execute();
	}
	return true;
}

bool Module::IsExecuted() const {
	return _bExecuted;
}

std::shared_ptr<Space> Module::GetSpace() const {
	return _space;
}
