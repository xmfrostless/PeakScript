#include "expression_variable_analysis.h"
#include "runtime/value/value_tool.h"
#include "runtime/variable.h"
#include "runtime/sentence/sentence_expression_variable.h"
#include "runtime/sentence/sentence_expression_function_call.h"

using namespace peak;

ExpressionVariableAnalysisName::ExpressionVariableAnalysisName(const std::string& name)
	: _name(name) {
}
std::shared_ptr<Variable> ExpressionVariableAnalysisName::Execute(std::shared_ptr<Space> space) {
	auto ret = space->FindVariable(_name);
	if (!ret) {
		ErrorLogger::LogRuntimeError(_name);
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableNameAnalysis, "The variable \"" + _name + "\" not found!");
	}
	return ret;
}
void ExpressionVariableAnalysisName::SetName(const std::string& name) {
	_name = name;
}

ExpressionVariableAnalysisArrayItem::ExpressionVariableAnalysisArrayItem(std::unique_ptr<SentenceExpression> valueExpression, std::vector<std::unique_ptr<SentenceExpression>> indexExpressionVec)
	: _valueExpression(std::move(valueExpression)), _indexExpressionVec(std::move(indexExpressionVec)) {
}

std::shared_ptr<Variable> ExpressionVariableAnalysisArrayItem::Execute(std::shared_ptr<Space> space) {
	if (!Sentence::IsSuccess(_valueExpression->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The expression execute failed!");
		return nullptr;
	}
	auto retValue = _valueExpression->GetValue();
	std::shared_ptr<Variable> retVariable { nullptr };

	auto expressionVecSize = _indexExpressionVec.size();
	for (auto i = 0u; i < expressionVecSize; ++i) {
		if (ValueTool::IsArray(retValue.get())) {
			auto& arr = std::static_pointer_cast<ValueArray>(retValue)->GetArray();
			auto& expression = _indexExpressionVec[i];
			if (!Sentence::IsSuccess(expression->Execute(space))) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The expression execute failed!");
				return nullptr;
			}
			auto indexValue = expression->GetValue();
			if (!ValueTool::IsInteger(indexValue.get())) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The index isn't a integer value!");
				return nullptr;
			}
			auto index = static_cast<std::size_t>(std::static_pointer_cast<ValueNumber>(indexValue)->GetValue());
			if (index >= arr.size()) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The index out of range!");
				return nullptr;
			}
			retVariable = arr[index];
			if (!retVariable) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The array value is invalid!");
				return nullptr;
			}
			retValue = retVariable->GetValue();
		} else if (ValueTool::IsString(retValue.get())) {
			auto& str = std::static_pointer_cast<ValueString>(retValue)->GetValue();
			auto& expression = _indexExpressionVec[i];
			if (!Sentence::IsSuccess(expression->Execute(space))) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The expression execute failed!");
				return nullptr;
			}
			auto indexValue = expression->GetValue();
			if (!ValueTool::IsInteger(indexValue.get())) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The index isn't a integer value!");
				return nullptr;
			}
			auto index = static_cast<std::size_t>(std::static_pointer_cast<ValueNumber>(indexValue)->GetValue());
			if (index >= str.size()) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The index out of range!");
				return nullptr;
			}
			retVariable = std::make_shared<Variable>("", VariableAttribute::None);
			retValue = std::make_shared<ValueString>(std::string() + str[index]);
			retVariable->SetValue(retValue);
		} else {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableArrayItemAnalysis, "The result of expression isn't a array or string!");
			return nullptr;
		}
	}

	return retVariable;
}

ExpressionVariableAnalysisInside::ExpressionVariableAnalysisInside(std::unique_ptr<SentenceExpression> header, std::vector<std::unique_ptr<SentenceExpression>> insides)
	: _header(std::move(header)), _insides(std::move(insides)) {
}
std::shared_ptr<Variable> ExpressionVariableAnalysisInside::Execute(std::shared_ptr<Space> space) {
	if (!Sentence::IsSuccess(_header->Execute(space))) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableInsideAnalysis, "The header expression execute failed!");
		return nullptr;
	}
	auto headerValue = _header->GetValue();
	if (!ValueTool::IsObject(headerValue.get())) {
		ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableInsideAnalysis, "The header expression isn't a object!");
		return nullptr;
	}
	auto tempValue = headerValue;
	std::shared_ptr<Variable> retVariable { nullptr };
	for (auto& expression : _insides) {
		if (!ValueTool::IsObject(tempValue.get())) {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableInsideAnalysis, "The expression isn't a object!");
			return nullptr;
		}
		auto objSpace = std::static_pointer_cast<ValueObject>(tempValue)->GetSpace();
		auto expressionType = expression->GetExpressionType();
		auto executeRet = ExecuteResult::Failed;
		if (expressionType == ExpressionType::Function) {
			executeRet = static_cast<SentenceExpressionFunctionCall*>(expression.get())->ExecuteFromInside(objSpace, space);
			if (!Sentence::IsSuccess(executeRet)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableInsideAnalysis, "The inside expression execute failed!");
				return nullptr;
			}
			retVariable = std::make_shared<Variable>("", VariableAttribute::Const);
		} else if (expressionType == ExpressionType::Variable) {
			executeRet = expression->Execute(objSpace);
			if (!Sentence::IsSuccess(executeRet)) {
				ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableInsideAnalysis, "The inside expression execute failed!");
				return nullptr;
			}
			retVariable = static_cast<SentenceExpressionVariable*>(expression.get())->GetVariable();
		} else {
			ErrorLogger::LogRuntimeError(ErrorRuntimeCode::VariableInsideAnalysis, "The inside expression can't return a variable!");
			return nullptr;
		}
		tempValue = expression->GetValue();
	}
	return retVariable;
}