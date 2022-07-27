#include <pybind11/pybind11.h>

namespace py = pybind11;

// A simple function to add two integers
int add(int i, int j)
{
    return i+j;
}

long long fact(int n)
{
    long long acc = 1;
    for (int i = 1; i <= n; i++)
        acc *= i;
    return acc;
}

// binding code; for simplicity we are using the same file
// PYBIND11_MODULE is a macro to create a function that can be imported in python
// It takes two arguments
// 1. module name
// 2. m, defines a variable of type py::module_ which is the main interface for creating bindidngs.
PYBIND11_MODULE(example, m) {
    // doc() is a function that can be used to document the function
    m.doc() = "pybind11 simple example";
    // module::def() generates binding code that exposes the add() function to Python
    m.def("add", &add, "A simple function to add two integers");

    // we can add named arguments to the function
    m.def("fact", &fact, "A simple function to compute factorial", py::arg("n"));
}

int main(int argc, char** argv)
{

}