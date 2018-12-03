#ifndef DLTOOLS_SACLA_MODEL_PICKLER_H
#define DLTOOLS_SACLA_MODEL_PICKLER_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "model.h"

namespace dltools
{
namespace sacla
{
pybind11::dict _model__getstate__(const dltools::sacla::Model &m);
dltools::sacla::Model _model__setstate__(pybind11::dict d);

pybind11::dict _models__getstate__(const dltools::sacla::Models &m);
dltools::sacla::Models _models__setstate__(pybind11::dict d);
} // namespace sacla
} // namespace dltools

#endif //DLTOOLS_SACLA_MODEL_PICKLER_H
