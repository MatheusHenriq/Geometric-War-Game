#ifndef VEC2_H
#define VEC2_H
class Vec2
{
public:
    float x = 0;
    float y = 0;
    Vec2();
    Vec2(float xin, float yin);

    bool operator==(const Vec2 &rhs) const;
    bool operator!=(const Vec2 &rhs) const;

    Vec2 operator+(const Vec2 &rhs) const;
    Vec2 operator-(const Vec2 &rhs) const;
    Vec2 operator/(const float val) const;
    Vec2 operator*(const float val) const;

    void operator+=(const Vec2 &rhs);
    void operator-=(const Vec2 &rhs);
    void operator*=(const float val);
    void operator/=(const float val);

    Vec2 &add(const Vec2 &rhs);
    Vec2 &scale(const float val);
    Vec2 &rotate(const float degree);

    float dist(const Vec2 &rhs) const;
    Vec2 &normalize();
    float distSquare(const Vec2 &rhs) const;

    float length() const;
};

#endif
