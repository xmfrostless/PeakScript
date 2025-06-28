#include "sentence_echo.h"
#include "runtime/system.h"
#include "runtime/value/value_tool.h"
#include "sentence_expression.h"

using namespace peak;

SentenceEcho::SentenceEcho(std::unique_ptr<SentenceExpression> expression)
	: _expression(std::move(expression)) {
}

ExecuteResult SentenceEcho::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_expression->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Echo, "The parameter expression execute failed!");
		return ExecuteResult::Failed;
	}
	auto value = _expression->GetValue();
	System::Echo(ValueTool::ToString(value.get()));

	return ExecuteResult::Successed;
}