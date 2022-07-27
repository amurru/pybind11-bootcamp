#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet
{
public:
    Pet(const std::string &name) : name(name) {}
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
    int age;
private:
    std::string name;
};

PYBIND11_MODULE(pet, m)
{
    // class_ creates bindings for C++ class or struct-style data structure.
    // init() : check custom constructors
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        // directly exposing a variable for read/write access
        .def_readwrite("age", &Pet::age)
        // def_property(name, getter, setter) : create a property
        // def_property_readonly(name, getter) : create a read-only property
        .def_property("name", &Pet::getName, &Pet::setName)
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        // __repr__() : printable representation of the object (Python)
        .def("__repr__",
             [](const Pet &a)
             {
                 return "<Pet name='" + a.getName() + "'>";
             });
}