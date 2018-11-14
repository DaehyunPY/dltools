pybind11_add_module(
        model
        model_binder.cpp
        model_pickler.cpp
        model.cpp
        ../hittype.cpp
)
