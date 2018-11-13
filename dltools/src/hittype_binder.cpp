//
// Created by Daehyun You on 11/6/18.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "hittype_caster.h"
#include "hittype.h"


using namespace pybind11::literals;
namespace py = pybind11;


PYBIND11_MODULE(hittype, m) {
    m.attr("__all__") = std::vector<std::string>{"zip_to_hits", "combine"};
    m.def(
            "zip_to_hits",
            (dltools::Hits (*)(std::vector<double>, std::vector<double>, std::vector<double>)) &dltools::zip_to_hits,
            "t"_a, "x"_a, "y"_a,
            py::call_guard<py::gil_scoped_release>()
    );
    m.def(
            "zip_to_hits",
            (dltools::Hits (*)(std::vector<double>, std::vector<double>, std::vector<double>, std::vector<int>))
            &dltools::zip_to_hits,
            "t"_a, "x"_a, "y"_a, "flag"_a,
            py::call_guard<py::gil_scoped_release>()
    );
    m.def(
            "combine",
            (dltools::CombinedHits (*)(dltools::Hits, int)) &dltools::combine,
            "hits"_a, "r"_a,
            py::call_guard<py::gil_scoped_release>());
    m.def(
            "combine",
            (dltools::CombinedHits (*)(dltools::Hits, int, std::unordered_set<std::string>)) &dltools::combine,
            "hits"_a, "r"_a, "white_list"_a,
            py::call_guard<py::gil_scoped_release>());
}
