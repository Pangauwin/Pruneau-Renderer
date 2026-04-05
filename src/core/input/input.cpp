#include "input.h"
#include <unordered_map>

namespace Core {

MOUSE_BUTTON_STATE Input::mouse_map[3] = {MOUSE_BUTTON_STATE_RELEASED,  MOUSE_BUTTON_STATE_RELEASED, MOUSE_BUTTON_STATE_RELEASED};

std::unordered_map<char, KEY_STATE> Input::keyboard_map;


KEY_STATE Input::GetKeyState(char _key)
{
    auto it = keyboard_map.find(_key);
    if(it == keyboard_map.end())
    {
        return KEY_STATE_RELEASED;
    }
    return it->second;
}

MOUSE_BUTTON_STATE Input::GetMouseButtonState(int _button)
{
    if((_button >= 0) && (_button <= 2))
        return mouse_map[_button];
    
    return MOUSE_BUTTON_STATE_RELEASED;
}

}