#include "executer.h"
#include "sentence/sentence_expression.h"
#include "runtime/value/value_tool.h"
#include "space.h"

using namespace peak;

std::unique_ptr<Executer> Executer::Create(const std::string& src) {
	auto parseData = Parse::Load(src);
	if (!parseData->bSuccess) {
		return nullptr;
	}
	return std::make_unique<Executer>(std::move(parseData));
}

Executer::Executer(std::unique_ptr<ParseData> data)
	: _parseData(std::move(data)) {
	_space = std::make_shared<Space>(SpaceType::None);
	_outsideSpace = std::make_shared<Space>(SpaceType::None);
}

Executer::~Executer() {
	_space->Clear();
	_outsideSpace->Clear();
}

bool Executer::Execute() {
	_space->Clear();
	_space->AddUsingSpace(_outsideSpace);
	for (auto& sentence : _parseData->sentenceList) {
		if (!Sentence::IsSuccess(sentence->Execute(_space))) {
			return false;
		}
	}
	return true;
}

std::shared_ptr<Value> Executer::ExecuteExpression(const std::string& src) {
	auto exp = Parse::LoadExpression(src);
	if (!exp) { return nullptr; }
	if (!Sentence::IsSuccess(exp->Execute(_space))) {
		return nullptr;
	}
	return exp->GetValue();
}

std::shared_ptr<Space> Executer::GetSpace() const {
	return _space;
}

std::shared_ptr<Variable> Executer::FindVariable(const std::string& name) const {
	return _space->FindVariable(name);
}
bool Executer::AddVariable(std::shared_ptr<Variable> variable) {
	return _outsideSpace->AddVariable(variable);
}
