#ifndef MATH_HPP
#define MATH_HPP

#include <array>
#include <cmath>

struct float3
{
    union
    {
        struct
        {
            float r, g, b;
        };
        struct
        {
            float x, y, z;
        };
    };
    float3() : x(0), y(0), z(0) {}
    float3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct float2
{
    float x, y;

    float2() : x(0), y(0) {}
    float2(float _x, float _y) : x(_x), y(_y) {}
};

struct mat3
{
    std::array<std::array<float, 3>, 3> m;

    mat3()
    {
        m = {{
            {{1, 0, 0}},
            {{0, 1, 0}},
            {{0, 0, 1}},
        }};
    };

    mat3(std::initializer_list<std::initializer_list<float>> values)
    {
        int row = 0;
        for (auto &r : values)
        {
            int col = 0;
            for (auto &v : r)
            {
                m[row][col++] = v;
            }
            row++;
        }
    }
    mat3 operator*(const mat3 &other) const
    {
        mat3 result;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 3; ++k)
                    result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
        return result;
    }
    float3 operator*(const float3 &v) const
    {
        return {
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z};
    }
    mat3 transpose() const {
        mat3 result;
        for (int i=0; i < 3; i++) {
            for (int j=0; j<3; j++) {
                result.m[j][i]=m[i][j];
            }
        }
        return result;
    }
};

inline float3 operator-(const float3 &a, const float3 &b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
inline float3 operator+(const float3 &a, const float3 &b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
inline float3 operator*(const float3 &a, const float &b) { return {a.x * b, a.y * b, a.z * b}; }
inline float3 operator*(const float a, const float3 &b) { return {a * b.x, a * b.y, a * b.z}; }
inline float3 operator/(const float3 &a, const float b) { return {a.x / b, a.y / b, a.z / b}; }
inline float3 operator-(const float3 &a) { return float3{-a.x, -a.y, -a.z}; }

inline float2 operator-(const float2 &a, const float2 &b) { return {a.x - b.x, a.y - b.y}; }
inline float2 operator+(const float2 &a, const float2 &b) { return {a.x + b.x, a.y + b.y}; }
inline float2 operator*(const float2 &a, const float &b) { return {a.x * b, a.y * b}; }
inline float2 operator/(const float2 &a, const float b) { return {a.x / b, a.y / b}; }
inline float dot(const float2 &a, const float2 &b) { return {a.x * b.x + a.y * b.y}; }
inline float dot(const float3 &a, const float3 &b) { return {a.x * b.x + a.y * b.y + a.z * b.z}; }
inline float3 cross(const float3 &a, const float3 &b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; };
inline float3 normalize(const float3 &v)
{
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0)
        return {0, 0, 0}; // Avoid division by zero
    return {v.x / length, v.y / length, v.z / length};
}
inline float2 orth(const float2 &a) { return {a.y, -a.x}; }

namespace MATH
{
    bool PointInTriangle(float2 a, float2 b, float2 c, float2 p, float3 *weights);
}
float3 random_color();
#endif