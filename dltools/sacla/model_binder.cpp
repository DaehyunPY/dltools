#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../hittype_caster.h"
#include "model_pickler.h"
#include "model.h"

using namespace pybind11::literals;
namespace py = pybind11;

PYBIND11_MODULE(model, m)
{
    m.attr("__all__") = std::vector<std::string>{"Model", "Models"};
    py::class_<dltools::sacla::Model>(m, "Model", "SACLA momentum model.")
        .def(
            py::init<double, std::array<double, 7>, std::array<double, 6>,
                     double, double,
                     double, double, double, double, double, double>(),
            R"pbdoc(
                Initialize SACLA momentum model.
                :param mass: Mass of the model particle, such as ion or electron, in aotmic units.
                :param pz_coeffs: Coefficients of momentum model in the z direction.
                :param pr_coeffs: Coefficients of momentum model in the r direction.
                :param fr: Lower limit of flight time, in atomic units.
                :param to: Upper limit of flight time, in atomic units.
                :param x1: A calibration factor in atomic units.
                :param y1: A calibration factor in atomic units.
            )pbdoc",
            "mass"_a, "pz_coeffs"_a, "pr_coeffs"_a,
            "fr"_a = 0, "to"_a = INFINITY,
            "t0"_a = 0, "th"_a = 0, "x0"_a = 0, "y0"_a = 0, "dx"_a = 1, "dy"_a = 1)
        .def(py::pickle(
            &dltools::sacla::_model__getstate__,
            &dltools::sacla::_model__setstate__));

    py::class_<dltools::sacla::Models>(m, "Models", "SACLA momentum models.")
        .def(
            py::init<std::unordered_map<std::string, dltools::sacla::Model>>(),
            R"pbdoc(
                Initialize SACLA momentum models.
                :param models: Map to Model.
                :param t0: A calibration factor in atomic units.
                :param th: A calibration factor in radians.
                :param x0: A calibration factor in atomic units.
                :param y0: A calibration factor in atomic units.
                :param dx: A calibration factor.
                :param dy: A calibration factor.
            )pbdoc",
            "models"_a)
        .def(py::pickle(
            &dltools::sacla::_models__getstate__,
            &dltools::sacla::_models__setstate__))
        .def(
            "__call__",
            (dltools::Hit(dltools::sacla::Models::*)(const dltools::Hit &) const) & dltools::sacla::Models::operator(),
            R"pbdoc(
                Analyze momentum of a Hit. Hit (t, x, y) will be calibrated to...
                    (t) -> (t - t0),
                    (x, y) -> ((x, y) * rot(th) - (x0, y0)) * (dx, dy).
                :param hit: Hit to be analyzed.
                :return: Hit with a map to momentum and kinetic energy.
            )pbdoc",
            "hit"_a)
        .def(
            "__call__",
            (dltools::Hits(dltools::sacla::Models::*)(const dltools::Hits &) const) & dltools::sacla::Models::operator(),
            R"pbdoc(
                Analyze momentum of Hits. All Hit (t, x, y) will be calibrated to...
                    (t) -> (t - t0),
                    (x, y) -> ((x, y) * rot(th) - (x0, y0)) * (dx, dy).
                :param hits: Hits to be analyzed.
                :return: Hits with maps to momentum and kinetic energy.
            )pbdoc",
            "hit"_a);
}
