#pragma once

#include "general/source/query.h"
#include "general/source/answer.h"
#include "session.h"

#include <ostream>


void ProcessCycleOutcome(Query::Any, Answer::Any, SessionState &, std::ostream & output);
