#include "letter.h"


bool operator ==(const Letter& lhs, const Letter& rhs) {
    return lhs.from == rhs.from &&
           lhs.to == rhs.to &&
           lhs.body == rhs.body &&
           lhs.date == rhs.date;
}
