//
// Created by Daehyun You on 11/5/18.
//

#ifndef DLTOOLS_HITTYPE_CASTER_H
#define DLTOOLS_HITTYPE_CASTER_H


#include "hittype.h"


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
            {  // as_
                if (d.contains("as_")) {
                    make_caster<std::unordered_map<std::string, dltools::AnalyzedHit>> v;
                    if (not v.load(d["as_"].ptr(), convert)) {
                        return false;
                    }
                    value.as_ = cast_op<std::unordered_map<std::string, dltools::AnalyzedHit> &&>(std::move(v));
                } else {
                    value.as_ = {};
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
            {  // as_
                return_value_policy p =
                        return_value_policy_override<std::unordered_map<std::string, dltools::AnalyzedHit>>
                        ::policy(policy);
                auto v = reinterpret_steal<object>(
                        make_caster<std::unordered_map<std::string, dltools::AnalyzedHit>>
                        ::cast(forward_like<T>(src.as_), p, parent)
                );
                if (not v) {
                    return handle();
                }
                d["as_"] = v;
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


    template<> struct type_caster<dltools::CombinedHit> {
        bool load(handle src, bool convert) {
            if (not isinstance<dict>(src)) {
                return false;
            }
            auto d = reinterpret_borrow<dict>(src);
            {  // comb
                make_caster<std::vector<dltools::Hit>> v;
                if (not(d.contains("comb") and v.load(d["comb"].ptr(), convert))) {
                    return false;
                }
                value.comb = cast_op<std::vector<dltools::Hit>>(v);
            }
            {  // as_
                if (d.contains("as_")) {
                    make_caster<std::unordered_map<std::string, dltools::AnalyzedHit>> v;
                    if (not v.load(d["as_"].ptr(), convert)) {
                        return false;
                    }
                    value.as_ = cast_op<std::unordered_map<std::string, dltools::AnalyzedHit> &&>(std::move(v));
                } else {
                    value.as_ = {};
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
            {  // comb
                return_value_policy p = return_value_policy_override<std::vector<dltools::Hit>>::policy(policy);
                auto v = reinterpret_steal<object>(
                        make_caster<std::vector<dltools::Hit>>
                        ::cast(forward_like<T>(src.comb), p, parent)
                );
                if (not v) {
                    return handle();
                }
                d["comb"] = v;
            }
            {  // as_
                return_value_policy p =
                        return_value_policy_override<std::unordered_map<std::string, dltools::AnalyzedHit>>
                        ::policy(policy);
                auto v = reinterpret_steal<object>(
                        make_caster<std::unordered_map<std::string, dltools::AnalyzedHit>>
                        ::cast(forward_like<T>(src.as_), p, parent)
                );
                if (not v) {
                    return handle();
                }
                d["as_"] = v;
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

        PYBIND11_TYPE_CASTER(dltools::CombinedHit, _("dltools.CombinedHit"));
    };
}}


#endif //DLTOOLS_HITTYPE_CASTER_H
