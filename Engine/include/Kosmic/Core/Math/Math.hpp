#pragma once

#include <cmath>
#include <numbers>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Kosmic::Math {

using Mat4 = glm::mat4;

const float PI = 3.14159265359f;

// Vector2
struct Vector2 {
    float x{}, y{};
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}
    
    // Operators
    Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
    Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
    Vector2 operator*(float scalar)    const { return { x * scalar, y * scalar }; }
    Vector2 operator/(float scalar)    const { return { x / scalar, y / scalar }; }
    Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
    Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
};

// Vector3
struct Vector3 {
    float x{}, y{}, z{};
    Vector3() = default;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(float value) : x(value), y(value), z(value) {}

    // Operators
    Vector3 operator+(const Vector3& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vector3 operator-(const Vector3& other) const { return { x - other.x, y - other.y, z - other.z }; }
    Vector3 operator*(float scalar)    const { return { x * scalar, y * scalar, z * scalar }; }
    Vector3 operator/(float scalar)    const { return { x / scalar, y / scalar, z / scalar }; }
    Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

    float Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vector3 Cross(const Vector3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }
};

// Vector4
struct Vector4 {
    float x{}, y{}, z{}, w{};
    Vector4() = default;
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Operators
    Vector4 operator+(const Vector4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
    Vector4 operator-(const Vector4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
    Vector4 operator*(float scalar)    const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
    Vector4 operator/(float scalar)    const { return { x / scalar, y / scalar, z / scalar, w / scalar }; }
    Vector4& operator+=(const Vector4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
    Vector4& operator-=(const Vector4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
};

// Converting degrees to radians
constexpr float Deg2Rad(float deg) noexcept {
    return deg * (std::numbers::pi_v<float> / 180.f);
}

// Converting radius to degrees
constexpr float Rad2Deg(float rad) noexcept {
    return rad * (180.f / std::numbers::pi_v<float>);
}

// Quaternion
struct Quaternion {
    float x{}, y{}, z{}, w{1.f};
    Quaternion() = default;
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Multiplication between quaternions
    Quaternion operator*(const Quaternion& other) const {
        return {
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        };
    }
    // Quaternion normalization
    Quaternion& Normalize() {
        float mag = std::sqrt(x*x + y*y + z*z + w*w);
        if(mag > 0.0f) { x /= mag; y /= mag; z /= mag; w /= mag; }
        return *this;
    }
};

// Transform containing position, rotation and scale
struct Transform {
    Vector3 position;
    Quaternion rotation;
    Vector3 scale{1.f, 1.f, 1.f};

    // Returns the 4x4 transformation matrix
    std::array<float, 16> GetMatrix() const {
        // Convert quaternion to rotation matrix
        float xx = rotation.x * rotation.x;
        float yy = rotation.y * rotation.y;
        float zz = rotation.z * rotation.z;
        float xy = rotation.x * rotation.y;
        float xz = rotation.x * rotation.z;
        float yz = rotation.y * rotation.z;
        float wx = rotation.w * rotation.x;
        float wy = rotation.w * rotation.y;
        float wz = rotation.w * rotation.z;

        std::array<float, 16> m = {
            (1 - 2*(yy + zz)) * scale.x,  2*(xy - wz)       * scale.x, 2*(xz + wy)       * scale.x, 0,
            2*(xy + wz)       * scale.y,  (1 - 2*(xx + zz)) * scale.y, 2*(yz - wx)       * scale.y, 0,
            2*(xz - wy)       * scale.z,  2*(yz + wx)       * scale.z, (1 - 2*(xx + yy)) * scale.z, 0,
            position.x,                position.y,                   position.z,                    1
        };
        return m;
    }
};

// Normalize a vector
inline Vector3 Normalize(const Vector3& v) {
    float mag = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    if(mag > 0.0f) return { v.x / mag, v.y / mag, v.z / mag };
    return { 0.0f, 0.0f, 0.0f };
}

// Cross product between two vectors
inline Vector3 Cross(const Vector3& a, const Vector3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

// Translate a matrix
inline Mat4 Translate(const Mat4& matrix, const Vector3& translation) {
    return glm::translate(matrix, glm::vec3(translation.x, translation.y, translation.z));
}

// Scale a matrix
inline Mat4 Scale(const Mat4& matrix, const Vector3& scale) {
    return glm::scale(matrix, glm::vec3(scale.x, scale.y, scale.z));
}

// LookAt matrix
inline Mat4 LookAt(const Vector3& position, const Vector3& target, const Vector3& up) {
    return glm::lookAt(
        glm::vec3(position.x, position.y, position.z),
        glm::vec3(target.x, target.y, target.z),
        glm::vec3(up.x, up.y, up.z)
    );
}

} // namespace Kosmic::Math
