// test.cpp
// Casos que COMPILAN y casos que NO COMPILAN
// Para activar los casos inválidos, elimina el comentario //#error
// y descomenta la línea correspondiente.
//
// Compile con:
//   g++ -std=c++20 -o test test.cpp && ./test
//
// Para probar los casos inválidos (uno a la vez):
//   Descomenta la línea marcada y observa el error de compilación.

#include <iostream>
#include <vector>
#include <string>
#include "core_numeric.hpp"
#include "clases.hpp"

int main() {

    // ==========================================================
    //  SUM - casos válidos
    // ==========================================================
    {
        std::vector<int>    vi{1, 2, 3};
        std::vector<double> vd{1.1, 2.2};
        std::vector<Vector3D> vv{{1,0,0},{0,1,0}};

        auto a = core_numeric::sum(vi);   // OK: int  cumple Addable
        auto b = core_numeric::sum(vd);   // OK: double cumple Addable
        auto c = core_numeric::sum(vv);   // OK: Vector3D cumple Addable
        std::cout << "[sum] int=" << a << "  double=" << b
                  << "  Vector3D=" << c << "\n";
    }

    // sum - caso INVÁLIDO (descomenta para ver el error)
    // ERROR: std::string NO cumple Addable (operator+ de string devuelve string,
    //        pero operator+ entre dos const char* no está definido como
    //        string+string->string en el concepto exacto que exige same_as<T>)
    //        Más precisamente: std::string SÍ cumple Addable, pero el concept
    //        Iterable + Addable falla cuando el contenedor es de strings para
    //        funciones numéricas. En el caso de sum, std::string sí compilaría
    //        (concatenación). La restricción numérica se aplica en variance/mean.
    // {
    //     // Descomenta para ver el error de concepto Divisible:
    //     // std::vector<std::string> vs{"Hola", "Mundo"};
    //     // auto x = core_numeric::mean(vs);  // ERROR: string no cumple Divisible
    // }

    // ==========================================================
    //  MEAN - casos válidos
    // ==========================================================
    {
        std::vector<double> vd{1.0, 2.0, 3.0};
        std::vector<int>    vi{2, 4, 6};
        std::vector<Esfera> es{{1.0},{2.0},{3.0}};

        auto m1 = core_numeric::mean(vd);  // OK: double cumple Addable+Divisible
        auto m2 = core_numeric::mean(vi);  // OK: int    cumple Addable+Divisible
        auto m3 = core_numeric::mean(es);  // OK: Esfera cumple Addable+Divisible
        std::cout << "[mean] double=" << m1 << "  int=" << m2
                  << "  Esfera=" << m3 << "\n";
    }

    // mean - caso INVÁLIDO
    // ERROR: std::string NO cumple Divisible (no tiene operator/(size_t))
    // Concepto que falla: Divisible<std::string>
    // {
    //     std::vector<std::string> vs{"a", "b", "c"};
    //     auto x = core_numeric::mean(vs);   // ERROR: Divisible no se cumple
    // }

    // ==========================================================
    //  VARIANCE - casos válidos
    // ==========================================================
    {
        std::vector<double> vd{2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
        std::vector<Esfera> es{{1.0},{2.0},{3.0}};

        auto v1 = core_numeric::variance(vd);  // OK: double
        auto v2 = core_numeric::variance(es);  // OK: Esfera
        std::cout << "[variance] double=" << v1 << "  Esfera=" << v2 << "\n";
    }

    // variance - caso INVÁLIDO
    // ERROR: std::string NO cumple Addable ni Divisible
    // Concepto que falla: Addable<std::string> en el contexto numérico (operator+
    // de string no satisface same_as<string> cuando se combina con Divisible)
    // {
    //     std::vector<std::string> data{"a", "b", "c"};
    //     auto v = core_numeric::variance(data);  // ERROR: string no cumple Divisible
    // }

    // ==========================================================
    //  MAX - casos válidos
    // ==========================================================
    {
        std::vector<double> vd{1.0, 2.7, 0.3};
        std::vector<int>    vi{5, 3, 9, 1};
        std::vector<Vector3D> vv{{1,0,0},{0,0,3},{0,2,0}};
        std::vector<Esfera>   es{{1.0},{3.5},{2.0}};

        auto r1 = core_numeric::max(vd);  // OK: double  → 2.7
        auto r2 = core_numeric::max(vi);  // OK: int     → 9
        auto r3 = core_numeric::max(vv);  // OK: Vector3D usa operator<
        auto r4 = core_numeric::max(es);  // OK: Esfera  compara por volumen

        std::cout << "[max] double=" << r1 << "  int=" << r2
                  << "  Vector3D=" << r3 << "  Esfera=" << r4 << "\n";
    }

    // max - caso INVÁLIDO
    // ERROR: std::string cumple Comparable (tiene operator<),
    //        pero queremos mostrar que un tipo SIN operator< falla.
    //        Creamos un struct sin operator< para demostrar:
    // struct SinComparar { int x; };
    // std::vector<SinComparar> v{{1},{2}};
    // auto r = core_numeric::max(v);  // ERROR: SinComparar no cumple Comparable

    // ==========================================================
    //  TRANSFORM_REDUCE - casos válidos
    // ==========================================================
    {
        std::vector<double> vd{1.0, 2.0, 3.0};
        auto r = core_numeric::transform_reduce(vd, [](double x){ return x * x; });
        // 1 + 4 + 9 = 14
        std::cout << "[transform_reduce] x^2 sum=" << r << "\n";

        std::vector<int> vi{1, 2, 3, 4};
        auto r2 = core_numeric::transform_reduce(vi, [](int x){ return x * 2; });
        // 2+4+6+8 = 20
        std::cout << "[transform_reduce] x*2 sum=" << r2 << "\n";
    }

    // ==========================================================
    //  VARIADIC - casos válidos
    // ==========================================================
    {
        auto s1 = core_numeric::sum_variadic(1, 2, 33, 4);
        auto s2 = core_numeric::mean_variadic(0.1, 2.0, 3.0, 4.0);
        auto s3 = core_numeric::variance_variadic(1.0, 2.0, 3.0, 4.0);
        auto s4 = core_numeric::max_variadic(1, 2, 3, 4);

        std::cout << "[variadic] sum=" << s1
                  << "  mean=" << s2
                  << "  variance=" << s3
                  << "  max=" << s4 << "\n";
    }

    // variadic - caso INVÁLIDO
    // ERROR: sum_variadic requiere que todos los tipos sean iguales (same_as<T,Args>)
    // {
    //     auto x = core_numeric::sum_variadic(1, 2.5, 3);
    //     // ERROR: int y double no satisfacen same_as<int, double>
    // }

    // ==========================================================
    //  Tipos que NO tienen los concepts requeridos
    // ==========================================================

    // struct SinNada { double val; };   ← No tiene operator+, operator/, operator<
    // std::vector<SinNada> v{{1.0},{2.0}};
    // core_numeric::sum(v);       // ERROR: Addable no se cumple
    // core_numeric::mean(v);      // ERROR: Addable + Divisible no se cumplen
    // core_numeric::max(v);       // ERROR: Comparable no se cumple

    std::cout << "\nTodos los casos válidos pasaron correctamente.\n";
    return 0;
}
