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

## Return Value Policies

Python and C++ use different ways of memory management. This potentially leads to problems when dealing with functions that return non-trivial types.

Pybind11 provides several return value policy annotations that can be passed to the `module::def()` and `class_::def()` functions. The default policy is `return_value_policy::automatic`.

Return value policies can also be applied to properties:

```cpp
class_<MyClass>(m , "MyClass")
    .def_property("data",
        py::cpp_function(&MyClass::getData, py::return_value_policy::copy),
        py::cpp_function(&MyClass::setData)
    );
```

Return value policies are tricky, and it's very important to get them right.

| Return Value Policy | Description |
|---------------------|-------------|
| `return_value_policy::take_ownership` | Reference an existing object and take ownership. Python will call the destructor and delete operator when the object's reference count reaches zero. Undefined behavior ensues when the C++ side does the same, or when the data was not dynamically allocated. |
| `return_value_policy::copy` | Create a new copy of the returned object, which will be owned by Python. This policy is comparably safe because the lifetimes of the two instances are decoupled.|
| `return_value_policy::move` | Use `std::move` to move the return value contents into a new instance that will be owned by Python. This policy is comparably dafe because the lifetimes of the two instances (move source and destination) are decoupled. |
| `return_value_policy::reference` | Refrence an existing object, but do not take ownership. The C++ side is responsible for managing the object's lifetime and deallocating it when it is no longer used. **Warning:** _undefined behavior will ensue when the C++ side deletes an object that is still referenced and used by Python._ |
| `return_value_policy::reference_internal` | Indicates that the lifetime of the return value is tied to the lifetime of a parent object, namely the implicit `this` or `self` argument of the called method or property. Internally, this policy works just like `return_value_policy::reference` but additionally applies a `keep_alive<0, 1>` _call policy_ that prevents the parent object from being garbage collected as long as the return value is referenced by Python. This is the default policy for property getters created via `def_property`, `def_readwrite`, etc. |
| `return_value_policy::automatic` | This policy falls back to the policy `return_value_policy::take_ownership` when the return value is a pointer. Otherwise, it uses `return_value_policy::move` or `return_value_policy::copy` for rvalue and lvalue references, respectively. See above for a description of what all of these different policies do. This is the default policy for `py::class_` -wrapped types. |
| `return_value_policy::automatic_reference` | As above, but use policy `return_value_policy::reference` when the return value is a pointer. This is the default conversion policy for function arguments when calling Python functions manually from C++ code via `handle::operator()` and the casters in `pybind11/stl.h` . You probably won't need to use this explicitly. |

## Additional call policies

Call policies can be specified to indicate dependencies between parameters or ensure a certain state for the function call.

### Keep alive

In general, this policy is required when the C++ object is any kind of container and another object is being added to the container.

`keep_alive<Nurse, Patient>` indicates that the argument with index `Patient` should be kept alive at least until the argument with index `Nurse` is freed by the garbage collector.

Argument indices start at one, while zero refers to the return value.

For methods, index `1` refers to the implicit `this` pointer, while regular arguments begin at index `2`.

In this example, the binding code for a list append operation ties the lifetime of the newly added element to the underlying container:

```cpp
py::class_<List>(m, "List")
    .def("append", &List::append, py::keep_alive<1,2>());
```

Argument indexing for constructors is identical:

```cpp
py::class_<Nurse>(m, "Nurse")
    .def(py::init<Patient &>(), py::keep_alive<1, 2>());
```

### Call guard

The `call_guard<T>` policy allows any scope guard type `T` to be placed around the function:

```cpp
m.def("foo", foo, py::call_guard<T>());
```

Multiple guards can be specified as `py::guard<T1, T2, T3..>`. Constructor order is left to right and destruction happens in reverse.

## Python objects as arguments

Pybind11 allows us to work with native Python types on the C++ side. For example, the following statement iterates over a Python `dict`:

```cpp
void print_dict(const py::dict& dict) {
    for (auto item: dict)
    {
        std::cout << "key=" << std::string(py::str(item.first))
            << ", " << "value=" << std::string(py::str(item.second))
            << std::endl;
    }
}
// it can be exported
m.def("print_dict", &print_dict);
```
