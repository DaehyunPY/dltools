//
// Created by Daehyun You on 11/4/18.
//

#include <math.h>
#include <algorithm>
#include "sacla_model.h"


using std::move;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::to_string;
using std::pair;
using std::unordered_map;
using dltools::AnalyzedHit;
using dltools::Hit;


double dltools::sacla::_pz_model(double r, double t, const std::array<double, 7> &coeffs) {
    return coeffs[0] + coeffs[1] * t + coeffs[2] * pow(t, 2) + coeffs[3] * pow(t, 3) + coeffs[4] * pow(t, 4) +
           coeffs[5] * pow(r, 2) + coeffs[6] * pow(r, 4);
}


double dltools::sacla::_pr_model(double r, double t, const std::array<double, 6> &coeffs) {
    return coeffs[0] * r + coeffs[1] * r * t + coeffs[2] * pow(r, 3) * t + coeffs[3] * pow(r, 5) * t +
           coeffs[4] * pow(t, 3) + coeffs[5] * pow(t, 5);
}


dltools::sacla::Model::Model(
        double mass, std::array<double, 7> pz_coeffs, std::array<double, 6> pr_coeffs,
        double fr, double to, double x1, double y1) :
        __mass(mass), __fr(fr), __to(to), __x1(x1), __y1(y1),
        __pz_coeffs(pz_coeffs), __pr_coeffs(pr_coeffs) {
}


shared_ptr<dltools::AnalyzedHit> dltools::sacla::Model::operator()(const dltools::Hit &hit) const {
    if (not((__fr <= hit.t) and (hit.t <= __to))) {
        return nullptr;
    }
    auto calib = Hit{.t=hit.t, .x=hit.x + __x1, .y=hit.y + __y1};
    auto pz = _pz_model(pow(pow(calib.x, 2) + pow(calib.y, 2), 0.5), calib.t, __pz_coeffs);
    auto px = _pr_model(calib.x, calib.t, __pr_coeffs);
    auto py = _pr_model(calib.y, calib.t, __pr_coeffs);
    auto ke = (pow(pz, 2) + pow(px, 2) + pow(py, 2)) / 2 / __mass;
    return make_shared<AnalyzedHit>(AnalyzedHit{.pz=pz, .px=px, .py=py, .ke=ke});
}


dltools::sacla::Model::operator std::string() const {
    return "<Model {'mass': " + to_string(__mass)
           + ", 'fr': " + to_string(__fr)
           + ", 'to': " + to_string(__to)
           + ", 'x1': " + to_string(__x1)
           + ", 'y1': " + to_string(__y1) + "}>";
}


dltools::sacla::Models::Models(
        std::unordered_map<std::string, dltools::sacla::Model> models,
        double t0, double th, double x0, double y0, double dx, double dy) :
        __t0(t0), __th(th), __x0(x0), __y0(y0), __dx(dx), __dy(dy),
        __models(move(models)) {
}


dltools::Hit dltools::sacla::Models::operator()(const dltools::Hit &hit) const {
    auto ret = Hit{
            .t=hit.t - __t0,
            .x=__dx * (cos(__th) * hit.x - sin(__th) * hit.y - __x0),
            .y=__dy * (sin(__th) * hit.x + cos(__th) * hit.y - __y0),
            .as={},
            .flag=hit.flag
    };
    unordered_map<string, AnalyzedHit> map;
    for (const auto &m : __models) {
        auto ptr = m.second(ret);
        if (ptr) {
            map.insert(pair<string, AnalyzedHit>(m.first, *ptr));
        }
    }
    ret.as = move(map);
    return ret;
}


dltools::sacla::Models::operator std::string() const {
    return "<Models {'t0': " + to_string(__t0)
           + ", 'th': " + to_string(__th)
           + ", 'x0': " + to_string(__x0)
           + ", 'y0': " + to_string(__y0)
           + ", 'dx': " + to_string(__dx)
           + ", 'dy': " + to_string(__dy) + "}>";
}


std::vector<dltools::Hit> dltools::sacla::Models::operator()(const std::vector<dltools::Hit> &hits) const {
    auto ret = std::vector<Hit>{};
    for (const auto &h : hits) {
        ret.push_back((*this)(h));
    }
    return ret;
}

