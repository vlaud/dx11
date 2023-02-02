#pragma once
typedef unsigned char BYTE;
class Color
{
	union
	{
		BYTE rgba[4];
		unsigned int color;
	};
public:
	Color();
	Color(unsigned int val);
	Color(BYTE r, BYTE g, BYTE b);
	Color(BYTE r, BYTE g, BYTE b, BYTE a);
	Color(const Color& src);

	Color& operator=(const Color& scr);
	bool operator==(const Color& rhs) const;
	bool operator!=(const Color& rhs) const;
	
	constexpr BYTE GetR() const;
	constexpr BYTE GetG() const;
	constexpr BYTE GetB() const;
	constexpr BYTE GetA() const;

	void SetR(BYTE r);
	void SetG(BYTE g);
	void SetB(BYTE b);
	void SetA(BYTE a);
};

namespace COLORS
{
	const Color UnLoadedTextureColor(100, 100, 100);
	const Color UnHandledTextureColor(255, 0, 255);
}