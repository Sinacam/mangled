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
// f == "?frobnicate@bar@foo@@YA?AUshort_name@@Ulong_struct_name@@@Z"sv
````

Note that the string must not contain whitespace or the global namespace qualifier.


# Acknowledgements

[MSalters][1] for the original idea.

[1]: https://stackoverflow.com/a/16016860/4832499
