#include "sentence_try_catch_finally.h"

using namespace peak;

SentenceTryCatchFinally::SentenceTryCatchFinally(std::unique_ptr<Sentence> senTry,
												 std::unique_ptr<Sentence> senCatch,
												 std::unique_ptr<Sentence> senFinally)
	: _senTry(std::move(senTry)), _senCatch(std::move(senCatch)), _senFinally(std::move(senFinally)) {
}
ExecuteResult SentenceTryCatchFinally::Execute(std::shared_ptr<Space> space) {
	if (!IsSuccess(_senTry->Execute(std::make_shared<Space>(SpaceType::None, space)))) {
		if (_senCatch) {
			if (!IsSuccess(_senCatch->Execute(std::make_shared<Space>(SpaceType::None, space)))) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::TryCatchFinally, "The sentence of catch execute failed!");
				return ExecuteResult::Failed;
			}
		}
	}
	if (_senFinally) {
		if (!IsSuccess(_senFinally->Execute(std::make_shared<Space>(SpaceType::None, space)))) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::TryCatchFinally, "The sentence of finally execute failed!");
			return ExecuteResult::Failed;
		}
	}
	return ExecuteResult::Successed;
}