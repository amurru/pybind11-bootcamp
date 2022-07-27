#include<pybind11/pybind11.h>
namespace py=pybind11;

struct Pet {
    enum Kind {
        Dog = 0,
        Cat
    };

    struct Attributes {
        float age =0;
    };

    Pet(const std::string &name, Kind type): name(name), type(type) { }

    std::string name;
    Kind type;
    Attributes attr;
};

PYBIND11_MODULE(internal, m)
{
    py::class_<Pet> pet(m, "Pet");

    pet.def(py::init<const std::string &, Pet::Kind>())
    .def_readwrite("name", &Pet::name)
    .def_readwrite("type", &Pet::type)
    .def_readwrite("attr", &Pet::attr);

// To ensure that the nested types Kind and Attributes are created
// within the scope of Pet, the pet class_ instance must be supplied
// to the enum_ and class_ constructor.
    py::enum_<Pet::Kind>(pet, "Kind")
    .value("Dog", Pet::Kind::Dog)
    .value("Cat", Pet::Kind::Cat)
// The enum_::export_values() function exports the enum entries into the partent scope
// which should be skipped for newer C++11-style strongly typed enums
    .export_values();
    py::class_<Pet::Attributes>(pet, "Attributes")
    .def(py::init<>())
    .def_readwrite("age", &Pet::Attributes::age);
}

// entries defined by enumeration type are exposed (in python) in the __members__ property.
// eg. Pet.Kind.__members__ => {'Dog': Kind.Dog, 'Cat': Kind.Cat}
