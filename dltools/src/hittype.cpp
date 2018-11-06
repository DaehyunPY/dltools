//
// Created by Daehyun You on 11/5/18.
//

#include <algorithm>
#include "hittype.h"


using std::make_shared;
using std::vector;
using std::to_string;
using dltools::Hit;


dltools::AnalyzedHit::operator std::string() const {
    return "<AnalyzedHit {'pz': " + to_string(pz)
           + ", 'px': " + to_string(px)
           + ", 'py': " + to_string(py)
           + ", 'ke': " + to_string(ke) + "}>";
}


dltools::Hit::operator std::string() const {
    return "<Hit {'t': " + to_string(t)
           + ", 'x': " + to_string(x)
           + ", 'y': " + to_string(y) + "}>";
}


std::vector<Hit> dltools::zip_to_hits(std::vector<double> t,
                                      std::vector<double> x,
                                      std::vector<double> y) {
    std::vector<Hit> ret;
    auto it = t.begin();
    auto ix = x.begin();
    auto iy = y.begin();
    for (; it != t.end() and ix != x.end() and iy != y.end(); ++it, ++ix, ++iy) {
        ret.push_back(Hit{.t=*it, .x=*ix, .y=*iy});
    }
    return ret;
};


std::vector<Hit> dltools::zip_to_hits(std::vector<double> t,
                                      std::vector<double> x,
                                      std::vector<double> y,
                                      std::vector<int> flag) {
    std::vector<Hit> ret;
    auto it = t.begin();
    auto ix = x.begin();
    auto iy = y.begin();
    auto iflag = flag.begin();
    for (; it != t.end() and ix != x.end() and iy != y.end() and iflag != flag.end(); ++it, ++ix, ++iy, ++iflag) {
        ret.push_back(Hit{.t=*it, .x=*ix, .y=*iy, .flag=make_shared<int>(*iflag)});
    }
    return ret;
};
