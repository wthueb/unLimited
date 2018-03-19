#pragma once

using clr_t = unsigned char;

struct clr_int
{
	int r;
	int g;
	int b;
	int a;
};

class Color
{
private:
	clr_t _r;
	clr_t _g;
	clr_t _b;
	clr_t _a;

public:
	Color()
	{
		*reinterpret_cast<unsigned int*>(this) = 0;
	}

	Color(clr_t r, clr_t g, clr_t b, clr_t a = 255)
		: _r(r), _g(g), _b(b), _a(a) {}

	Color(int r, int g, int b, int a = 255)
		: _r(clr_t(r)), _g(clr_t(g)), _b(clr_t(b)), _a(clr_t(a)) {}

	explicit Color(float r, float g, float b, float a = 1.f)
	{
		_r = unsigned char(r * 255.f);
		_g = unsigned char(g * 255.f);
		_b = unsigned char(b * 255.f);
		_a = unsigned char(a * 255.f);
	}

	explicit Color(clr_t rgb[4])
		: _r(rgb[0]), _g(rgb[1]), _b(rgb[2]), _a(rgb[3]) {}

	explicit Color(unsigned long argb)
	{
		_b = unsigned char((argb & 0x000000FF) >> (0 * 8));
		_g = unsigned char((argb & 0x0000FF00) >> (1 * 8));
		_r = unsigned char((argb & 0x00FF0000) >> (2 * 8));
		_a = unsigned char((argb & 0xFF000000) >> (3 * 8));
	}

	void SetRawColor(int color32)
	{
		*reinterpret_cast<unsigned int*>(this) = color32;
	}

	int GetRawColor() const
	{
		return *reinterpret_cast<unsigned int*>(const_cast<Color*>(this));
	}

	void SetColor(clr_t r, clr_t g, clr_t b, clr_t a = 255)
	{
		_r = r;
		_g = g;
		_b = b;
		_a = a;
	}

	void GetColor(clr_t& r, clr_t& g, clr_t& b, clr_t& a) const
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	clr_int to_int() const
	{
		return clr_int{ _r, _g, _b, _a };
	}

	clr_t r() const { return _r; }
	clr_t g() const { return _g; }
	clr_t b() const { return _b; }
	clr_t a() const { return _a; }

	clr_t& operator[](int index)
	{
		return reinterpret_cast<unsigned char*>(this)[index];
	}

	const clr_t& operator[](int index) const
	{
		return operator[](index);
	}

	bool operator==(const Color& rhs) const
	{
		return this->GetRawColor() == rhs.GetRawColor();
	}

	bool operator!=(const Color& rhs) const
	{
		return !operator==(rhs);
	}

	Color& operator=(const Color& rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}
};
