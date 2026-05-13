#pragma once

#include "Event.h"
#include "Mist/Core/KeyCodes.h"

namespace Mist {

class KeyEvent : public Event {
public:
    inline Key GetKeyCode() const {
        return m_KeyCode;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    KeyEvent(Key keycode) :
        m_KeyCode(keycode) {
    }

    Key m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(Key keycode, int repeatCount) :
        KeyEvent(keycode),
        m_RepeatCount(repeatCount) {
    }

    inline int GetRepeatCount() const {
        return m_RepeatCount;
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent:     '" << (char)(int)m_KeyCode << "' (" << m_RepeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(Key keycode, int repeatCount) :
        KeyEvent(keycode) {
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent:    '" << (char)(int)m_KeyCode << "'";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(Key keycode) :
        KeyEvent(keycode) {
    }

    std::string ToString() const override {
        std::stringstream ss;
        ss << "KeyTypedEvent:    '" << (char)(int)m_KeyCode << "'";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace Mist