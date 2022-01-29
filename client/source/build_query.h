#pragma once

#include "common/source/query.h"
#include "session.h"

#include <iostream>


Query::Any BuildQuery(std::istream & input, std::ostream & output,
                      const SessionState & session_state);
