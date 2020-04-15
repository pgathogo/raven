#ifndef TYPES_H
#define TYPES_H

#include <string>

using ErrorMessage = std::tuple<bool, std::string>;

enum class NotificationType {
    ntERROR,
    ntSUCCESS
};

#endif // TYPES_H
