//
// Created by Daehyun You on 11/5/18.
//

#include <algorithm>
#include "hittype.h"


using std::to_string;


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
