#pragma once

#include "Mist/Core/KeyCodes.h"
#include "Mist/Core/MouseButtonCodes.h"
#include "Mist/Core/Core.h"

namespace Mist {

class Input {
public:
    static bool IsKeyPressed(KeyCode keycode);

    static bool IsMouseButtonPressed(MouseButtonCode button);
    static float GetMouseX();
    static float GetMouseY();
    static std::pair<float, float> GetMousePosition();
};

} // namespace Mist