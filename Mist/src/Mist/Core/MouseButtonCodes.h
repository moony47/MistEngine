#pragma once

namespace Mist {

enum class MouseButtonCode : uint16_t {
    Button1 /*    */ = 0,
    Button2 /*    */ = 1,
    Button3 /*    */ = 2,
    Button4 /*    */ = 3,
    Button5 /*    */ = 4,
    Button6 /*    */ = 5,
    Button7 /*    */ = 6,
    Button8 /*    */ = 7,

    Left /*       */ = Button1,
    Right /*      */ = Button2,
    Middle /*     */ = Button3,
    Last /*       */ = Button8
};

inline std::ostream& operator<<(std::ostream& os, MouseButtonCode keycode) {
    os << static_cast<uint16_t>(keycode);
    return os;
}

} // namespace Mist