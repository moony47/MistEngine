#pragma once

#include "Mist/Core/Core.h"
#include "Mist/Core/KeyCodes.h"
#include "Mist/Core/MouseButtonCodes.h"

namespace Mist {

class Input {
public:
    static bool IsKeyPressed(Key keycode);

    static bool IsMouseButtonPressed(MouseButton button);
    static float GetMouseX();
    static float GetMouseY();
    static std::pair<float, float> GetMousePosition();
};

} // namespace Mist