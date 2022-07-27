#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet
{
    Pet(const std::string &name) : name(name) {}
    std::string name;
};

PYBIND11_MODULE(dynamic, m)
{
    // adding dynamic_attr() to class_ constructor enables us to add attributes to the class in Python on the fly.
    py::class_<Pet>(m, "Pet", py::dynamic_attr())
        .def(py::init<const std::string &>())
        .def_readwrite("name", &Pet::name);
}