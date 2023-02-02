#include "KeyBoard.h"

KeyBoard::KeyBoard()
{
    for (int i = 0; i < 256; ++i)
    {
        keyStates[i] = false;
    }
}

bool KeyBoard::KeyIsPressed(const unsigned char keycode)
{
    return keyStates[keycode];
}

bool KeyBoard::KeyBufferIsEmpty()
{
    return keyBuffer.empty();
}

bool KeyBoard::CharBufferIsEmpty()
{
    return charBuffer.empty();
}

KeyboardEvent KeyBoard::ReadKey()
{
    if (!keyBuffer.empty())
    {
        KeyboardEvent e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    return KeyboardEvent();
}

unsigned char KeyBoard::ReadChar()
{
    if (!charBuffer.empty())
    {
        unsigned char c = charBuffer.front();
        charBuffer.pop();
        return c;
    }
    return 0u;
}

void KeyBoard::OnKeyPressed(const unsigned char key)
{
    keyStates[key] = true;
    keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyBoard::OnkeyReleased(const unsigned char key)
{
    keyStates[key] = false;
    keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyBoard::OnChar(const unsigned char key)
{
    charBuffer.push(key);
}

void KeyBoard::EnableAutoRepeatKeys()
{
    autoRepeatKeys = true;
}

void KeyBoard::DisableAutoRepeatKeys()
{
    autoRepeatKeys = false;
}

void KeyBoard::EnableAutoRepeatChars()
{
    autoRepeatChars = true;
}

void KeyBoard::DisableAutoRepeatChars()
{
    autoRepeatChars = false;
}

bool KeyBoard::IsKeysAutoRepeat()
{
    return autoRepeatKeys;
}

bool KeyBoard::IsCharsAutoRepeat()
{
    return autoRepeatChars;
}
