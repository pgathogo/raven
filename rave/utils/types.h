#ifndef TYPES_H
#define TYPES_H

#include <string>

enum class ActionResultType {
    arSUCCESS,
    arERROR
};

using ActionResult = std::tuple<ActionResultType, std::string>;

enum class NotificationType {
    ntERROR,
    ntSUCCESS
};

enum class DBAction{
        dbaCREATE,
        dbaUPDATE,
        dbaDELETE,
        dbaNONE
};

#endif // TYPES_H
