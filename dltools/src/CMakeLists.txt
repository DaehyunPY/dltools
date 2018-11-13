pybind11_add_module(
        hittype
        hittype_binder.cpp
        hittype.cpp
)

pybind11_add_module(
        sacla_model
        sacla_model_binder.cpp
        sacla_model_pickler.cpp
        sacla_model.cpp
        hittype.cpp
)
