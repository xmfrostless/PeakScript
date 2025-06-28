/*
	PeakScript: https://github.com/xmfrostless/PeakScript
	By xmfrostless
*/

#pragma once

#include "base/error_logger.h"
#include "executer.h"

namespace peak {

class Space;

class Module {
	MAKE_NON_COPYABLE(Module);
public:
	Module(std::unique_ptr<Executer> executer);
	Module(std::shared_ptr<Space> space);
	bool Execute();

	bool IsExecuted() const;

	std::shared_ptr<Space> GetSpace() const;

private:
	std::unique_ptr<Executer> _executer { nullptr };
	std::shared_ptr<Space> _space { nullptr };
	bool _bExecuted { false };
};

} // namespace peak