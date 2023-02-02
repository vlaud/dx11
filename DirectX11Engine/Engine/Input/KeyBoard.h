#pragma once
#include "KeyboardEvent.h"
#include <queue>
using namespace std;

class KeyBoard
{
	bool autoRepeatKeys = false;
	bool autoRepeatChars = false;
	bool keyStates[256];
	queue<KeyboardEvent> keyBuffer;
	queue<unsigned char> charBuffer;
public:
	KeyBoard();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnkeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();
};

