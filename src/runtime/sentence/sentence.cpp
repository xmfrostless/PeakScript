#include "sentence.h"

bool peak::Sentence::IsSuccess(ExecuteResult value) {
    return value != ExecuteResult::Failed;
}
