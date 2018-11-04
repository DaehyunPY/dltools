//
// Created by Daehyun You on 11/5/18.
//

#ifndef DLTOOLS_CASTER_H
#define DLTOOLS_CASTER_H


#include "hit_types.h"


namespace pybind11 { namespace detail {
    template<> struct type_caster<dltools::AnalyzedHit> {
        bool load(handle src, bool convert) {
            if (not isinstance<dict>(src)) {
                return false;
            }
            auto d = reinterpret_borrow<dict>(src);
            {  // pz
                make_caster<double> v;
                if (not(d.contains("pz") and v.load(d["pz"].ptr(), convert))) {
                    return false;
                }
                value.pz = cast_op<double>(v);
            }
            {  // px
                make_caster<double> v;
                if (not(d.contains("px") and v.load(d["px"].ptr(), convert))) {
                    return false;
                }
                value.px = cast_op<double>(v);
            }
            {  // py
                make_caster<double> v;
                if (not(d.contains("py") and v.load(d["py"].ptr(), convert))) {
                    return false;
                }
                value.py = cast_op<double>(v);
            }
            {  // ke
                make_caster<double> v;
                if (not(d.contains("ke") and v.load(d["ke"].ptr(), convert))) {
                    return false;
                }
                value.ke = cast_op<double>(v);
            }
            return true;
        }

        template <typename T>
        static handle cast(T &&src, return_value_policy policy, handle parent) {
            dict d;
            return_value_policy p = return_value_policy_override<double>::policy(policy);
            {  // pz
                auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.pz), p, parent));
                if (not v) {
                    return handle();
                }
                d["pz"] = v;
            }
            {  // px
                auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.px), p, parent));
                if (not v) {
                    return handle();
                }
                d["px"] = v;
            }
            {  // py
                auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.py), p, parent));
                if (not v) {
                    return handle();
                }
                d["py"] = v;
            }
            {  // ke
                auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.ke), p, parent));
                if (not v) {
                    return handle();
                }
                d["ke"] = v;
            }
            return d.release();
        }

        PYBIND11_TYPE_CASTER(dltools::AnalyzedHit, _("dltools.AnalyzedHit"));
    };


    template<> struct type_caster<dltools::Hit> {
        bool load(handle src, bool convert) {
            if (not isinstance<dict>(src)) {
                return false;
            }
            auto d = reinterpret_borrow<dict>(src);
            {  // t
                make_caster<double> v;
                if (not(d.contains("t") and v.load(d["t"].ptr(), convert))) {
                    return false;
                }
                value.t = cast_op<double>(v);
            }
            {  // x
                make_caster<double> v;
                if (not(d.contains("x") and v.load(d["x"].ptr(), convert))) {
                    return false;
                }
                value.x = cast_op<double>(v);
            }
            {  // y
                make_caster<double> v;
                if (not(d.contains("y") and v.load(d["y"].ptr(), convert))) {
                    return false;
                }
                value.y = cast_op<double>(v);
            }
            {  // as
                if (d.contains("as")) {
                    make_caster<std::unordered_map<std::string, dltools::AnalyzedHit>> v;
                    if (not v.load(d["as"].ptr(), convert)) {
                        return false;
                    }
                    value.as = cast_op<std::unordered_map<std::string, dltools::AnalyzedHit> &&>(std::move(v));
                } else {
                    value.as = {};
                }
            }
            {  // flag
                if (d.contains("flag")) {
                    make_caster<int> v;
                    if (not v.load(d["flag"].ptr(), convert)) {
                        return false;
                    }
                    value.flag = std::make_shared<int>(cast_op<int>(v));
                } else {
                    value.flag = nullptr;
                }
            }
            return true;
        }

        template <typename T>
        static handle cast(T &&src, return_value_policy policy, handle parent) {
            dict d;
            {
                return_value_policy p = return_value_policy_override<double>::policy(policy);
                {  // t
                    auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.t), p, parent));
                    if (not v) {
                        return handle();
                    }
                    d["t"] = v;
                }
                {  // x
                    auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.x), p, parent));
                    if (not v) {
                        return handle();
                    }
                    d["x"] = v;
                }
                {  // y
                    auto v = reinterpret_steal<object>(make_caster<double>::cast(forward_like<T>(src.y), p, parent));
                    if (not v) {
                        return handle();
                    }
                    d["y"] = v;
                }
            }
            {  // as
                return_value_policy p =
                        return_value_policy_override<std::unordered_map<std::string, dltools::AnalyzedHit>>
                        ::policy(policy);
                auto v = reinterpret_steal<object>(
                        make_caster<std::unordered_map<std::string, dltools::AnalyzedHit>>
                        ::cast(forward_like<T>(src.as), p, parent)
                );
                if (not v) {
                    return handle();
                }
                d["as"] = v;
            }
            {  // flag
                if (src.flag) {
                    return_value_policy p = return_value_policy_override<int>::policy(policy);
                    auto v = reinterpret_steal<object>(make_caster<int>::cast(forward_like<T>(*src.flag), p, parent));
                    if (not v) {
                        return handle();
                    }
                    d["flag"] = v;
                }
            }
            return d.release();
        }

        PYBIND11_TYPE_CASTER(dltools::Hit, _("dltools.Hit"));
    };
}}


#endif //DLTOOLS_CASTER_H