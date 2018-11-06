//
// Created by Daehyun You on 11/5/18.
//

#ifndef DLTOOLS_HITTYPE_H
#define DLTOOLS_HITTYPE_H


#include <math.h>
#include <vector>
#include <unordered_map>
#include <string>


namespace dltools {
    /**
     * Analyzed momentum (pz, px, py, ke) of a Hit.
     */
    struct AnalyzedHit {
        double pz, px, py, ke;

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
}


#endif //DLTOOLS_HITTYPE_H
