#pragma once
class KeyboardEvent
{
public:
	enum class EventType
	{
		Press,
		Release,
		Invalid
	};
	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPress() const;
	bool IsRelease() const; //멤버변수 변경 불가
	bool IsValid() const;
	unsigned char GetKeyCode() const;
private:
	EventType type;
	unsigned char key;
};

