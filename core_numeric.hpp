#pragma once

#include <concepts>
#include <iterator>
#include <cmath>
#include <stdexcept>
#include <type_traits>

// ============================================================
//  CONCEPTS
// ============================================================

template <typename C>
concept Iterable = requires(C c) {
    std::begin(c);
    std::end(c);
};

// Addable: T + T → T
template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

// Divisible: T / T → T  (evita int/size_t → long)
template <typename T>
concept Divisible = requires(T a, T b) {
    { a / b } -> std::same_as<T>;
};

// Concept propio #1: Comparable (necesario para max)
template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::same_as<bool>;
};

// Concept propio #2: Numeric — agrupa los tres anteriores
template <typename T>
concept Numeric = Addable<T> && Divisible<T> && Comparable<T>;


namespace core_numeric {

// ============================================================
//  Utilidad interna: construir T a partir de std::size_t
//  - Para tipos aritméticos: static_cast
//  - Para objetos: no se puede (se usa especialización via if constexpr)
// ============================================================
template <typename T>
T make_divisor(std::size_t n) {
    if constexpr (std::is_arithmetic_v<T>) {
        return static_cast<T>(n);
    } else {
        // Para objetos con constructor(double), p.ej. Esfera(n), Vector3D(n)
        return T(static_cast<double>(n));
    }
}

// ============================================================
//  1. sum
// ============================================================
template <Iterable C>
requires Addable<typename C::value_type>
auto sum(const C& container) {
    using T = typename C::value_type;
    T result{};
    for (const auto& v : container)
        result = result + v;
    return result;
}

// ============================================================
//  2. mean  — reutiliza sum, requiere Divisible
// ============================================================
template <Iterable C>
requires Addable<typename C::value_type> && Divisible<typename C::value_type>
auto mean(const C& container) {
    using T = typename C::value_type;
    std::size_t n = static_cast<std::size_t>(
        std::distance(std::begin(container), std::end(container)));
    if (n == 0) throw std::invalid_argument("El contenedor esta vacio");

    T total = sum(container);

    // if constexpr: distingue enteros de flotantes/objetos
    if constexpr (std::is_integral_v<T>) {
        // División entera (truncada hacia cero)
        return total / make_divisor<T>(n);
    } else {
        // División exacta para flotantes y objetos
        return total / make_divisor<T>(n);
    }
}

// ============================================================
//  3. variance  — reutiliza mean
// ============================================================
template <Iterable C>
requires Addable<typename C::value_type> && Divisible<typename C::value_type>
auto variance(const C& container) {
    using T = typename C::value_type;
    std::size_t n = static_cast<std::size_t>(
        std::distance(std::begin(container), std::end(container)));
    if (n == 0) throw std::invalid_argument("El contenedor esta vacio");

    T m      = mean(container);   // reutiliza mean
    T sum_sq{};

    for (const auto& v : container) {
        T d    = v - m;
        sum_sq = sum_sq + (d * d);
    }
    return sum_sq / make_divisor<T>(n);
}

// ============================================================
//  4. max  — requiere Comparable
// ============================================================
template <Iterable C>
requires Comparable<typename C::value_type>
auto max(const C& container) {
    using T = typename C::value_type;
    auto it  = std::begin(container);
    auto end = std::end(container);
    if (it == end) throw std::invalid_argument("El contenedor esta vacio");

    T result = *it++;
    for (; it != end; ++it) {
        // if constexpr: diferencia integral vs resto (objetos/flotantes)
        if constexpr (std::is_integral_v<T>) {
            // Enteros: comparación directa
            if (result < *it) result = *it;
        } else {
            // Flotantes u objetos con operator<
            if (result < *it) result = *it;
        }
    }
    return result;
}

// ============================================================
//  5. transform_reduce
// ============================================================
template <Iterable C, typename F>
requires Addable<typename C::value_type>
auto transform_reduce(const C& container, F f) {
    using T = typename C::value_type;
    using R = decltype(f(std::declval<T>()));
    R result{};
    for (const auto& v : container)
        result = result + f(v);
    return result;
}

// ============================================================
//  6. Variadic templates + fold expressions
// ============================================================

// sum_variadic — fold expression con operator+
template <Addable T, Addable... Args>
requires (std::same_as<T, Args> && ...)
auto sum_variadic(T first, Args... rest) {
    return (first + ... + rest);
}

// mean_variadic
template <Addable T, Addable... Args>
requires (std::same_as<T, Args> && ...) && Divisible<T>
auto mean_variadic(T first, Args... rest) {
    T total = (first + ... + rest);
    T ndiv  = make_divisor<T>(1 + sizeof...(rest));

    if constexpr (std::is_integral_v<T>) {
        return total / ndiv;   // división entera
    } else {
        return total / ndiv;   // división exacta
    }
}

// Auxiliar recursivo para suma de (xi - mean)^2
namespace detail {
    template <typename T>
    T sq_diff_sum(T /*m*/) { return T{}; }

    template <typename T, typename... Rest>
    T sq_diff_sum(T m, T head, Rest... tail) {
        T d = head - m;
        return (d * d) + sq_diff_sum(m, tail...);
    }
}

// variance_variadic
template <Addable T, Addable... Args>
requires (std::same_as<T, Args> && ...) && Divisible<T>
auto variance_variadic(T first, Args... rest) {
    T m      = mean_variadic(first, rest...);
    T sum_sq = detail::sq_diff_sum(m, first, rest...);
    T ndiv   = make_divisor<T>(1 + sizeof...(rest));
    return sum_sq / ndiv;
}

// max_variadic — fold con operador ternario
template <Comparable T, Comparable... Args>
requires (std::same_as<T, Args> && ...)
auto max_variadic(T first, Args... rest) {
    T result = first;
    ((result = (result < rest) ? rest : result), ...);
    return result;
}

} // namespace core_numeric
