#include "mistpch.h"
#include "Mist/Core/Input.h"

#include "Mist/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Mist {

bool Input::IsKeyPressed(KeyCode keycode) {
    auto window = static_cast<GLFWwindow*>(MIST_APP.GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, (int)keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(uint16_t button) {
    auto window = static_cast<GLFWwindow*>(MIST_APP.GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

float Input::GetMouseX() {
    auto [x, y] = GetMousePosition();
    return x;
}

float Input::GetMouseY() {
    auto [x, y] = GetMousePosition();
    return y;
}

std::pair<float, float> Input::GetMousePosition() {
    auto window = static_cast<GLFWwindow*>(MIST_APP.GetWindow().GetNativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {(float)x, (float)y};
}

} // namespace Mist