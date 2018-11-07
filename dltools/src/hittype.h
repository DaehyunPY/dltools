//
// Created by Daehyun You on 11/5/18.
//

#ifndef DLTOOLS_HITTYPE_H
#define DLTOOLS_HITTYPE_H


#include <math.h>
#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>


namespace dltools {
    /**
     * Analyzed momentum (pz, px, py, ke) of a Hit.
     */
    struct AnalyzedHit {
        double pz, px, py, ke;

        inline AnalyzedHit operator+(const AnalyzedHit &hit) {
            return AnalyzedHit{.pz=pz+hit.pz, .px=px+hit.px, .py=py+hit.py, .ke=ke+hit.ke};
        }

        inline AnalyzedHit &operator+=(const AnalyzedHit &hit) {
            pz += hit.pz;
            px += hit.px;
            py += hit.py;
            ke += hit.ke;
            return *this;
        }

        explicit operator std::string() const;
    };


    /**
     * Detected Hit (t, x, y) and map to analyzied momentum (as).
     */
    struct Hit {
        double t, x, y;
        std::unordered_map<std::string, AnalyzedHit> as;
        std::shared_ptr<int> flag;

        explicit operator std::string() const;
    };


    // TODO Add document
    std::vector<Hit> zip_to_hits(std::vector<double> t,
                                 std::vector<double> x,
                                 std::vector<double> y);

    // TODO Add document
    std::vector<Hit> zip_to_hits(std::vector<double> t,
                                 std::vector<double> x,
                                 std::vector<double> y,
                                 std::vector<int> flag);


    // TODO Add document
    struct CombinedHit {
        std::vector<Hit> comb;
        std::unordered_map<std::string, AnalyzedHit> as;
    };


    // TODO Add document
    std::vector<CombinedHit> combine(std::vector<Hit> hits, int r);

    // TODO Add document
    std::vector<CombinedHit> combine(std::vector<Hit> hits, int r,
                                     std::unordered_set<std::string> white_list);
}


#endif //DLTOOLS_HITTYPE_H
