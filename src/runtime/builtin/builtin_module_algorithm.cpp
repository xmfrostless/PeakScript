#include "builtin_module_algorithm.h"
#include "base/error_logger.h"
#include "runtime/space.h"
#include "runtime/variable.h"
#include "runtime/value/value_tool.h"

using namespace peak;

static void _Merge(std::vector<std::shared_ptr<Variable>>& arr, std::size_t start, std::size_t mid, std::size_t size, ValueFunction* comp, std::shared_ptr<Space> space, std::vector<std::shared_ptr<Variable>>& buffers) {
    auto i = start;
    auto j = mid;
    auto k = 0u;
    while (i < mid && j < size) {
        auto ret = comp->Call({ arr[i]->GetValue(), arr[j]->GetValue() }, space);
        if (ValueTool::ToLogic(ret.get())) {
            buffers[k++] = arr[i++];
        } else {
            buffers[k++] = arr[j++];
        }
    }
    while (i < mid) {
        buffers[k++] = arr[i++];
    }
    while (j < size) {
        buffers[k++] = arr[j++];
    }
    k = 0;
    while (start < size) {
        arr[start++] = buffers[k++];
    }
}

static void _MergeRecursion(std::vector<std::shared_ptr<Variable>>& arr, std::size_t start, std::size_t size, ValueFunction* comp, std::shared_ptr<Space> space, std::vector<std::shared_ptr<Variable>>& buffers) {
    if (start >= size - 1) {
        return;
    }
    auto mid = start + (size - start) / 2;
    _MergeRecursion(arr, start, mid, comp, space, buffers);
    _MergeRecursion(arr, mid, size, comp, space, buffers);
    _Merge(arr, start, mid, size, comp, space, buffers);
}

static ValueArray* _MergeSort(ValueArray* value_array, ValueFunction* comp, std::shared_ptr<Space> space) {
    auto& arr = value_array->GetArray();
    const auto n = arr.size();
    std::vector<std::shared_ptr<Variable>> buffers(n);
    _MergeRecursion(arr, 0, n, comp, space, buffers);
    return value_array;
}

std::shared_ptr<Module> BuiltModuleAlgorithm::CreateModule() {
    auto _InsertConst = [](std::shared_ptr<Space> space, const std::string& name, std::shared_ptr<Value> value) {
        space->AddVariable(std::make_shared<Variable>(name, VariableAttribute::Const, value));
    };
    auto _InsertFunction = [_InsertConst](std::shared_ptr<Space> space, const std::string& name, std::size_t paramSize, ValueFunction::FunctionType func) {
        _InsertConst(space, name, std::make_shared<ValueFunction>(paramSize, func));
    };

    auto space = std::make_shared<Space>(SpaceType::None);

    _InsertFunction(space, "merge_sort", 2, [](const std::vector<std::shared_ptr<Value>>& args, std::shared_ptr<Space> space) -> std::shared_ptr<Value> {
        if (ValueTool::IsArray(args[0].get()) && ValueTool::IsFunction(args[1].get())) {
            auto func = std::static_pointer_cast<ValueFunction>(args[1]).get();
            if (func->GetParamSize() != 2) {
                return nullptr;
            }
            if (!_MergeSort(std::static_pointer_cast<ValueArray>(args[0]).get(), func, space)) {
                return nullptr;
            }
            return ValueNull::DEFAULT_VALUE;
        }
        return nullptr;
    });

    return std::make_shared<Module>(space);
}
