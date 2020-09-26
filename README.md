# mangled
Constexpr mangled names of functions for msvc.

# Usage

````c++
namespace foo {
  namespace bar {
    int frobnicate(double*);
  }
}

using F = int(double*);
constexpr auto f = mangled::name<F>([]{ return "foo::bar::frobnicate"; });
````
