#include <iostream>
#include <vector>
#include <string>
#include "core_numeric.hpp"
#include "clases.hpp"

// ============================================================
//  Helpers para imprimir
// ============================================================
template <typename T>
void print(const std::string& label, T value) {
    std::cout << label << ": " << value << "\n";
}

int main() {

    std::cout << "======================================\n";
    std::cout << "  CoreNumeric - Demo completa\n";
    std::cout << "======================================\n\n";

    // ----------------------------------------------------------
    //  1. sum
    // ----------------------------------------------------------
    std::cout << "--- sum ---\n";
    std::vector<int>    vi{1, 2, 3, 4, 5};
    std::vector<double> vd{1.1, 2.2, 3.3};
    print("sum(int)   ", core_numeric::sum(vi));   // 15
    print("sum(double)", core_numeric::sum(vd));   // 6.6

    std::vector<Vector3D> vv{{1,0,0},{0,1,0},{0,0,1}};
    print("sum(Vector3D)", core_numeric::sum(vv)); // (1,1,1)
    std::cout << "\n";

    // ----------------------------------------------------------
    //  2. mean
    // ----------------------------------------------------------
    std::cout << "--- mean ---\n";
    std::vector<double> vd2{1.0, 2.0, 3.0, 4.0};
    print("mean(double)", core_numeric::mean(vd2));   // 2.5
    print("mean(int)   ", core_numeric::mean(vi));    // 3
    std::cout << "\n";

    // ----------------------------------------------------------
    //  3. variance
    // ----------------------------------------------------------
    std::cout << "--- variance ---\n";
    std::vector<double> vd3{2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
    print("variance(double)", core_numeric::variance(vd3)); // 4.0
    std::cout << "\n";

    // ----------------------------------------------------------
    //  4. max
    // ----------------------------------------------------------
    std::cout << "--- max ---\n";
    std::vector<double> vd4{1.0, 2.7, 0.3};
    std::vector<int>    vi2{5, 3, 9, 1};
    print("max(double)", core_numeric::max(vd4));  // 2.7
    print("max(int)   ", core_numeric::max(vi2));  // 9

    std::vector<Esfera> esferas{{1.0}, {3.0}, {2.0}};
    print("max(Esfera)", core_numeric::max(esferas)); // Esfera(r=3)
    std::cout << "\n";

    // ----------------------------------------------------------
    //  5. transform_reduce
    // ----------------------------------------------------------
    std::cout << "--- transform_reduce ---\n";
    std::vector<double> vd5{1.0, 2.0, 3.0};
    auto r = core_numeric::transform_reduce(vd5, [](double x){ return x * x; });
    print("transform_reduce(x^2)", r);  // 1+4+9 = 14
    std::cout << "\n";

    // ----------------------------------------------------------
    //  6. Variadic
    // ----------------------------------------------------------
    std::cout << "--- variadic ---\n";
    auto s1 = core_numeric::sum_variadic(1, 2, 33, 4);
    auto s2 = core_numeric::mean_variadic(0.1, 2.0, 3.0, 4.0);
    auto s3 = core_numeric::variance_variadic(1.0, 2.0, 3.0, 4.0);
    auto s4 = core_numeric::max_variadic(1, 2, 3, 4);

    print("sum_variadic(1,2,33,4)        ", s1);  // 40
    print("mean_variadic(0.1,2,3,4)      ", s2);  // 2.275
    print("variance_variadic(1,2,3,4)    ", s3);  // 1.25
    print("max_variadic(1,2,3,4)         ", s4);  // 4
    std::cout << "\n";

    // ----------------------------------------------------------
    //  if constexpr  (ya integrado en mean y max, demo explícita)
    // ----------------------------------------------------------
    std::cout << "--- if constexpr demo ---\n";
    // mean con int  → división entera
    std::vector<int> vi3{1, 2, 3};
    print("mean<int> (división entera)", core_numeric::mean(vi3)); // 2
    // mean con double → división exacta
    std::vector<double> vd6{1.0, 2.0, 3.0};
    print("mean<double> (división exacta)", core_numeric::mean(vd6)); // 2.0
    std::cout << "\n";

    std::cout << "======================================\n";
    std::cout << "  Fin de la demo\n";
    std::cout << "======================================\n";

    return 0;
}
