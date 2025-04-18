#include "vec2.h"
#include <math.h>

Vec2::Vec2() {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator==(const Vec2 &rhs) const
{
    return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2 &rhs) const
{
    return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 &Vec2::normalize()
{
    float l = length();
    x /= l;
    y /= l;
    return *this;
}

Vec2 Vec2::operator/(const float val) const
{
    return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
    return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
    x *= val;
    y *= val;
}

void Vec2::operator/=(const float val)
{
    x /= val;
    y /= val;
}

Vec2 &Vec2::add(const Vec2 &rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}
Vec2 &Vec2::scale(const float val)
{
    x += val;
    y += val;
    return *this;
}
Vec2 &Vec2::rotate(const float degree)
{
    // TODO: SEE HOW IT WORKS
    return *this;
}

float Vec2::dist(const Vec2 &rhs) const
{
    return sqrtf((rhs.x - x) * (rhs.x - x) - (rhs.y - y) * (rhs.y - y));
}
float Vec2::distSquare(const Vec2 &rhs) const
{
    return (rhs.x - x) * (rhs.x - x) + (rhs.y - y) * (rhs.y - y);
}

float Vec2::length() const
{
    return sqrtf(x * x + y * y);
}