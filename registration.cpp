
#include "registration.h"

std::map<int, Day*>& Day::all_days() noexcept {
    static std::map<int, Day*> value;
    return value;
}
