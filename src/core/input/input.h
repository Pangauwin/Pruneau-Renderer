#pragma once

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
    MOUSE_BUTTON_MIDDLE = 2,
    MOUSE_BUTTON_4 = 3,
    MOUSE_BUTTON_5 = 4,
    MOUSE_BUTTON_6 = 5,
    MOUSE_BUTTON_7 = 6,
    MOUSE_BUTTON_8 = 7,
};

class Input
{
public:
    static KEY_STATE GetKeyState(int _key);
    static MOUSE_BUTTON_STATE GetMouseButtonState(int _button);

    static void ChangeKeyState(int _key, KEY_STATE _state);
    static void ChangeMouseState(MOUSE_BUTTON _button, MOUSE_BUTTON_STATE _state);

    static const double* GetMouseDelta();
    static const double* GetMousePosition();

    static void UpdateMousePosition(double* mouse_position);
    static void UpdateMouseDelta();
    
    static void ChangeDisabledMouse(bool _disabled);
    static bool GetDisabledMouse();

private:
    Input() = default;
    ~Input() = default;

private:
    static KEY_STATE keyboard_map[349];
    static MOUSE_BUTTON_STATE mouse_map[8];
    static double mouse_delta[2];
    static double mouse_position[2];
    static double previous_mouse_position[2];
    static bool disabled_mouse;

};

}