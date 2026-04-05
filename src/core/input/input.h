#pragma once

#include "core/layer.h"
#include <unordered_map>

namespace Core
{

enum KEY_STATE {
    KEY_STATE_RELEASED,
    KEY_STATE_PRESSED
};

enum MOUSE_BUTTON_STATE {
    MOUSE_BUTTON_STATE_RELEASED,
    MOUSE_BUTTON_STATE_PRESSED
};

enum MOUSE_BUTTON {
    MOUSE_BUTTON_LEFT = 0,
    MOUSE_BUTTON_RIGHT = 1,
    MOUSE_BUTTON_MIDDLE = 2
};

class Input
{
    friend class Core::Layer;

public:
    static KEY_STATE GetKeyState(char _key);
    static MOUSE_BUTTON_STATE GetMouseButtonState(int _button);

private:
    Input() = default;
    ~Input() = default;

private:
    static std::unordered_map<char, KEY_STATE> keyboard_map;
    static MOUSE_BUTTON_STATE mouse_map[3];
};

}