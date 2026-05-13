#pragma once

namespace Mist {

enum class MouseButton : uint16_t {
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

inline std::ostream& operator<<(std::ostream& os, MouseButton mouseCode) {
    switch (mouseCode) {
        case MouseButton::Left:
            os << "LeftMouseButton";
            break;
        case MouseButton::Right:
            os << "RightMouseButton";
            break;
        case MouseButton::Middle:
            os << "MiddleMouseButton";
            break;
        case MouseButton::Last:
            os << "LastMouseButton";
            break;
        // case MouseButton::Button1:
        //     os << "MouseButton1";
        //     break;
        // case MouseButton::Button2:
        //     os << "MouseButton2";
        //     break;
        // case MouseButton::Button3:
        //     os << "MouseButton3";
        //     break;
        case MouseButton::Button4:
            os << "MouseButton4";
            break;
        case MouseButton::Button5:
            os << "MouseButton5";
            break;
        case MouseButton::Button6:
            os << "MouseButton6";
            break;
        case MouseButton::Button7:
            os << "MouseButton7";
            break;
            // case MouseButton::Button8:
            //     os << "MouseButton8";
            //     break;
    }
    return os;
}

#define MAX_CLICK_DURATION 0.15f

} // namespace Mist