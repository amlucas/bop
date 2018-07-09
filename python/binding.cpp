#include <pybind11/pybind11.h>
#include "pybop.h"

namespace py = pybind11;

PYBIND11_MODULE(pybop, m) {
    m.doc() = "bop wrapper for python";

    py::class_<PyBop>(m, "PyBop")
        .def(py::init<>())
        .def("set_n", &PyBop::set_n)
        .def("get_n", &PyBop::get_n);
}
 
