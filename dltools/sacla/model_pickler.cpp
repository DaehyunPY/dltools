#include "model_pickler.h"

using namespace pybind11::literals;
using dltools::sacla::Model;
using dltools::sacla::Models;
using std::move;
using std::pair;
using std::string;
using std::unordered_map;

pybind11::dict dltools::sacla::_model__getstate__(const dltools::sacla::Model &m)
{
    return pybind11::dict(
        "mass"_a = m._mass(),
        "pz_coeffs"_a = m._pz_coeffs(),
        "pr_coeffs"_a = m._pr_coeffs(),
        "fr"_a = m._fr(), "to"_a = m._to(),
        "t0"_a = m._t0(), "th"_a = m._th(),
        "x0"_a = m._x0(), "y0"_a = m._y0(),
        "dx"_a = m._dx(), "dy"_a = m._dy()
    );
}

dltools::sacla::Model dltools::sacla::_model__setstate__(pybind11::dict d)
{
    return {
        d["mass"].cast<double>(),
        d["pz_coeffs"].cast<std::array<double, 7>>(),
        d["pr_coeffs"].cast<std::array<double, 6>>(),
        d["fr"].cast<double>(), d["to"].cast<double>(),
        d["t0"].cast<double>(), d["th"].cast<double>(),
        d["x0"].cast<double>(), d["y0"].cast<double>(),
        d["dx"].cast<double>(), d["dy"].cast<double>()
    };
}

pybind11::dict dltools::sacla::_models__getstate__(const dltools::sacla::Models &m)
{
    pybind11::dict d;
    for (const auto &p : m._models())
    {
        d[pybind11::cast(p.first)] = _model__getstate__(p.second);
    }
    return pybind11::dict("models"_a = move(d));
}

dltools::sacla::Models dltools::sacla::_models__setstate__(pybind11::dict d)
{
    unordered_map<string, Model> map;
    for (const auto &p : d["models"].cast<pybind11::dict>())
    {
        map.insert(
            pair<string, Model>{p.first.cast<string>(),
                                dltools::sacla::_model__setstate__(p.second.cast<pybind11::dict>())});
    }
    return dltools::sacla::Models(move(map));
}
