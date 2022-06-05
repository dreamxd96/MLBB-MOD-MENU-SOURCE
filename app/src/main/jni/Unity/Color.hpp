#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>

struct Color
{
    union
    {
        struct
        {
			float R;
			float G;
			float B;
			float A;
        };
        float data[4];
    };

    inline Color();
    inline Color(float r, float g, float b, float a);

    static inline Color red();
    static inline Color green();
    static inline Color blue();
    static inline Color white();
    static inline Color black();
	static inline Color purple();
    static inline Color yellow();
	static inline Color yellowRGB(float A);
	static inline Color yellow_line();
    static inline Color magenta();
    static inline Color gray();
    static inline Color grey();
    static inline Color clear();
};

Color::Color() : R(0), G(0), B(0), A(0) {}
Color::Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}

Color Color::red() { return Color(0,0,255,255); }
Color Color::green() { return Color(255,0,255,0); }
Color Color::blue() { return Color(0,255,255,0); }
Color Color::white() { return Color(255,255,255,255); }
Color Color::black() { return Color(0,0,255,0); }
Color Color::purple() { return Color(0, 128, 255, 128); }
Color Color::yellow() { return Color(255,0,255,255); }
Color Color::yellowRGB(float A) { return Color(255,0,A,255); }
Color Color::yellow_line() { return Color(255, 255, 0, 255); }
Color Color::magenta() { return Color(0,255,255,249); }
Color Color::gray() { return Color(0,128,255,1); }
Color Color::grey() { return Color(128,128,255,128); }
Color Color::clear() { return Color(0,0,0,0); }
