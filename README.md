# Pybind11-Bootcamp

I'm using this repo to document my study and experiments with Pybind11

## Notes

* Include ```#include <pybind11/pybind11.h>```

* Pybind11 is a header-only library, hence no necessary to link against any special libraries.

```bash
c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) simple.cpp -o example$(python3-config --extension-suffix)
```

* Building will result in a shared object (so) that can be used with Python

* Python side:

```python3
import example
example.add(4,1230)
```

will result in: 1234

* Named Arguments:

```cpp
// regular notation
m.def("add1", &add, py::arg("i"), py::arg("j"));
// shorthand
using namespace pybind11::literals;
m.def("add2", &add, "i"_a, "j"_a);
```

* Default arguments: Pybind11 cannot extract them automatically, append "= **DEFAULT_VALUE** " to the argument: `py::arg("i") = 2` for `int add(int i=2...etc)`

* Exporting variables
We can use the `attr` function to register variables in a module

```cpp
PYBIND11_MODULE(example,m) {
    m.attr("the_answer") = 555;
    py::object word = py::cast("World");
    m.attr("what") = world;
}
```

which makes them accessible from python: `example.the_answer` and `example.what`

## Supported data types

* <https://pybind11.readthedocs.io/en/latest/advanced/cast/index.html>

## Bindings for custom types (OOP)

* check `pet.cpp` for an example.

* compile with `c++ -O3 -Wall -shared -std=c++11 -fPIC -fpermissive $(python3 -m pybind11 --includes) pet.cpp -o pet$(python3-config --extension-suffix)`

* Python side:

```python3
import pet
p = pet.Pet("Rex")
p.getName()
p.setName("Maxi")
p.getName()
```
