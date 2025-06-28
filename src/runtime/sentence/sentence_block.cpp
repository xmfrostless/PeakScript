
#include "sentence_block.h"

using namespace peak;

ExecuteResult SentenceBlock::Execute(std::shared_ptr<Space> space) {
	auto tempSpace = std::make_shared<Space>(space->GetSpaceType(), space);
	for (auto& sentence : _list) {
		auto executeRet = sentence->Execute(tempSpace);
		if (!IsSuccess(executeRet)) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::Block, "The sentence execute failed!");
			return ExecuteResult::Failed;
		}
		if (executeRet == ExecuteResult::Return) {
			SetReturnValue(static_cast<SentenceReturn*>(sentence.get())->GetReturnValue());
			return ExecuteResult::Return;
		}
		if (executeRet == ExecuteResult::Break) {
			return ExecuteResult::Break;
		}
		if (executeRet == ExecuteResult::Continue) {
			return ExecuteResult::Continue;
		}
	}
	tempSpace->Clear();
	return ExecuteResult::Successed;
}
void SentenceBlock::Push(std::unique_ptr<Sentence> sentence) {
	_list.emplace_back(std::move(sentence));
}
