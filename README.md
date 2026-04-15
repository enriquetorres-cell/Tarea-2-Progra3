Integrantes:


Enrique Eusebio Torres Chafloque

CoreNumeric
Librería de cabecera única (`core_numeric.hpp`) para operaciones numéricas genéricas en C++20, basada en concepts y templates variádicos.
---
Requisitos
Compilador con soporte C++20 (GCC 11+, Clang 13+, MSVC 19.29+)
CMake 3.20+ (opcional, para los ejecutables de demo y test)
---
Compilación
Con CMake
```bash
cmake -B build
cmake --build build
./build/main   # demo completa
./build/test   # casos válidos e inválidos
```
Manual
```bash
g++ -std=c++20 -o main main.cpp
g++ -std=c++20 -o test test.cpp
```
---
Concepts definidos
Concept	Requisito
`Iterable<C>`	`std::begin(c)` y `std::end(c)` válidos
`Addable<T>`	`T + T → T`
`Divisible<T>`	`T / T → T`
`Comparable<T>`	`T < T → bool`
`Numeric<T>`	`Addable && Divisible && Comparable`
---
Funciones
`core_numeric::sum(container)`
Suma todos los elementos de un contenedor iterable. Requiere `Addable`.
```cpp
std::vector<int> v{1, 2, 3};
core_numeric::sum(v);  // 6

std::vector<Vector3D> vv{{1,0,0},{0,1,0}};
core_numeric::sum(vv); // (1, 1, 0)
```
`core_numeric::mean(container)`
Calcula la media aritmética. Requiere `Addable` y `Divisible`. Lanza `std::invalid_argument` si el contenedor está vacío.
```cpp
std::vector<double> v{1.0, 2.0, 3.0, 4.0};
core_numeric::mean(v); // 2.5
```
`core_numeric::variance(container)`
Calcula la varianza poblacional. Requiere `Addable` y `Divisible`. Lanza `std::invalid_argument` si el contenedor está vacío.
```cpp
std::vector<double> v{2.0, 4.0, 4.0, 4.0, 5.0, 5.0, 7.0, 9.0};
core_numeric::variance(v); // 4.0
```
`core_numeric::max(container)`
Devuelve el elemento máximo. Requiere `Comparable`. Lanza `std::invalid_argument` si el contenedor está vacío.
```cpp
std::vector<int> v{5, 3, 9, 1};
core_numeric::max(v); // 9

std::vector<Esfera> e{{1.0},{3.0},{2.0}};
core_numeric::max(e); // Esfera(r=3)  — compara por volumen
```
`core_numeric::transform_reduce(container, f)`
Aplica una función `f` a cada elemento y suma los resultados. Requiere que el tipo de retorno de `f` sea `Addable`.
```cpp
std::vector<double> v{1.0, 2.0, 3.0};
core_numeric::transform_reduce(v, [](double x){ return x * x; }); // 14.0
```
Funciones variádicas
Versiones que aceptan un número arbitrario de argumentos del mismo tipo.
```cpp
core_numeric::sum_variadic(1, 2, 33, 4);              // 40
core_numeric::mean_variadic(0.1, 2.0, 3.0, 4.0);     // 2.275
core_numeric::variance_variadic(1.0, 2.0, 3.0, 4.0); // 1.25
core_numeric::max_variadic(1, 2, 3, 4);               // 4
```
> Todos los argumentos deben ser del mismo tipo (`same_as<T, Args>...`).
---
Tipos personalizados compatibles
El archivo `clases.hpp` incluye dos tipos que satisfacen `Numeric`:
`Vector3D`
Operaciones componente a componente. La comparación (`operator<`) se basa en la magnitud al cuadrado.
```cpp
Vector3D a{1, 0, 0}, b{0, 1, 0}, c{0, 0, 1};
std::vector<Vector3D> vv{a, b, c};
core_numeric::sum(vv);  // (1, 1, 1)
```
`Esfera`
Operaciones sobre el radio. La comparación (`operator<`) se basa en el volumen.
```cpp
std::vector<Esfera> es{{1.0}, {3.0}, {2.0}};
core_numeric::max(es);  // Esfera(r=3)
```
---
Extender con tipos propios
Para que un tipo personalizado funcione con todas las funciones de la librería debe cumplir `Numeric`:
```cpp
struct MiTipo {
    double val;

    MiTipo operator+(const MiTipo& o) const { return {val + o.val}; }
    MiTipo operator-(const MiTipo& o) const { return {val - o.val}; }
    MiTipo operator*(const MiTipo& o) const { return {val * o.val}; }
    MiTipo operator/(const MiTipo& o) const { return {val / o.val}; }
    bool   operator<(const MiTipo& o) const { return val < o.val; }

    // Constructor desde double (requerido por make_divisor en mean/variance)
    explicit MiTipo(double v) : val(v) {}
};
```
---
Errores de concepto comunes
Tipo	Función	Concept que falla
`std::string`	`mean`, `variance`	`Divisible`
Struct sin `operator<`	`max`, `max_variadic`	`Comparable`
Struct sin `operator+`	`sum`, `mean`, `variance`	`Addable`
Argumentos de tipos distintos	`sum_variadic`	`same_as<T, Args>`
---
Estructura del proyecto
```
.
├── core_numeric.hpp   # librería principal
├── clases.hpp         # tipos de ejemplo (Vector3D, Esfera)
├── main.cpp           # demo completa
├── test.cpp           # casos válidos e inválidos comentados
└── CMakeLists.txt
