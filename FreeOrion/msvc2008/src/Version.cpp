#include "../../util/Version.h"

namespace {
    static const std::string retval = "v0.3.14 [Rev 3520ish]";
}

const std::string& FreeOrionVersionString()
{
    return retval;
}
