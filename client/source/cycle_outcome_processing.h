#pragma once

#include "common/source/query.h"
#include "common/source/answer.h"
#include "session.h"

#include <ostream>


void ProcessCycleOutcome(Query::Any, Answer::Any, SessionState &, std::ostream & output);
