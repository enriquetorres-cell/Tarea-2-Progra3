#pragma once
#include <cmath>
#include <ostream>

// ============================================================
//  Vector3D
//  Cumple: Addable, Divisible (T/T→T), Comparable, Numeric
// ============================================================
struct Vector3D {
    double x, y, z;
    Vector3D(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}

    double mag2() const { return x*x + y*y + z*z; }

    // Addable
    Vector3D operator+(const Vector3D& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vector3D operator-(const Vector3D& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vector3D operator*(const Vector3D& o) const { return {x*o.x, y*o.y, z*o.z}; }

    // Divisible: T/T → T  (divide componente a componente)
    Vector3D operator/(const Vector3D& o) const { return {x/o.x, y/o.y, z/o.z}; }

    // Comparable: por magnitud
    bool operator<(const Vector3D& o) const { return mag2() < o.mag2(); }

    // Para static_cast<Vector3D>(n) en mean usamos un constructor desde double
    explicit Vector3D(double v) : x(v), y(v), z(v) {}

    friend std::ostream& operator<<(std::ostream& os, const Vector3D& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};

// ============================================================
//  Esfera
//  Cumple: Addable, Divisible (T/T→T), Comparable, Numeric
// ============================================================
struct Esfera {
    double radio;
    Esfera(double r=0) : radio(r) {}

    double volumen() const { return (4.0/3.0) * M_PI * radio*radio*radio; }

    // Addable
    Esfera operator+(const Esfera& o) const { return {radio + o.radio}; }
    Esfera operator-(const Esfera& o) const { return {radio - o.radio}; }
    Esfera operator*(const Esfera& o) const { return {radio * o.radio}; }

    // Divisible: T/T → T
    Esfera operator/(const Esfera& o) const { return {radio / o.radio}; }

    // Comparable: por volumen
    bool operator<(const Esfera& o) const { return volumen() < o.volumen(); }

    friend std::ostream& operator<<(std::ostream& os, const Esfera& e) {
        return os << "Esfera(r=" << e.radio << ")";
    }
};
