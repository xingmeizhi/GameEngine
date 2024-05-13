#include <pybind11/pybind11.h>
#include "Application.hpp"
#include "ResourceManager.h"

namespace py = pybind11;

PYBIND11_MODULE(mygameengine, m)
{
    py::class_<Application>(m, "Application")
        .def(py::init<int, int>(), py::arg("w"), py::arg("h"))
        .def("loop", &Application::Loop);
}