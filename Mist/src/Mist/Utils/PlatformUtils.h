#pragma once

#include <string>

namespace Mist {

class FileDialogs {
public:
    // Both return empty std::string() if cancelled
    static std::string OpenFile(const char* filter);
    static std::string SaveFile(const char* filter);
};

} // namespace Mist