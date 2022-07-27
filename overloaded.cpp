#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet
{
    Pet(const std::string &name, int age) : name(name), age(age) {}

    void set(int age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    std::string name;
    int age;
};

struct Widget {
    int foo(int x, float y);
    int foo(int x, float y) const;
};

PYBIND11_MODULE(overloaded, m)
{
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &, int>())
        // TODO: read more about static_cast and why it looks weird
        //  it seems this is a pre-C++14 form
        .def("set", static_cast<void (Pet::*)(int)>(&Pet::set), "Set the pet's age")
        .def("set", static_cast<void (Pet::*)(const std::string &)>(&Pet::set), "Set the pet's name");
    // C++14 compatible compilers can do it alternatively this way:
    // py::class_<Pet>(m, "Pet")
    //     .def("set", py::overload_cast<int>(&Pet::set), "Set age")
    //     .def("set", py::overload_cast<const std::string &>(&Pet::set), "Set name");

    //Constness-based overload
    py::class_<Widget>(m, "Widget")
    .def("foo_mutable", py::overload_cast<int, float>(&Widget::foo))
    // py::const_ tag is used to demostrate constness overload
    .def("foo_const", py::overload_cast<int, float>(&Widget::foo, py::const_));
}