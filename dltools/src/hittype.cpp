//
// Created by Daehyun You on 11/5/18.
//

#include <algorithm>
#include "hittype.h"


using std::move;
using std::fill;
using std::make_shared;
using std::vector;
using std::string;
using std::to_string;
using std::prev_permutation;
using dltools::AnalyzedHit;
using dltools::Hit;
using dltools::CombinedHit;


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
}


vector<dltools::CombinedHit> dltools::combine(std::vector<Hit> hits, int r) {
    std::vector<Hit> filtered;
    for (auto &h : hits) {
        if (not h.as.empty()) {
            filtered.push_back(move(h));
        }
    }
    auto n = filtered.size();
    if (n < r) {
        return vector<CombinedHit>();
    }

    auto ret = vector<CombinedHit>();
    vector<bool> pool(n, false);
    fill(pool.begin(), pool.begin() + r, true);
    do {
        CombinedHit hit;
        {  // Set member CombinedHit::comb
            vector<int> indexes;
            for (int j = 0; j < n; ++j) {
                if (pool[j]) {
                    indexes.push_back(j);
                }
            }
            for (auto j : indexes) {
                hit.comb.push_back(filtered[j]);
            }
        }
        {  // Set member CombinedHit::as
            int m = 1;  // Number of total loop
            vector<vector<const string *>> indexes;  // Vector of indexed key list
            for (const auto &h : hit.comb) {  // Set variable m and indexes
                m *= h.as.size();
                vector<const string *> tmp;
                for (const auto &d : h.as) {
                    tmp.push_back(&d.first);
                }
                indexes.push_back(move(tmp));
            }
            for (int i = 0; i < m; i++) {  // i: Index of the total loop (= (j, k), roughly speaking)
                auto tmp = i;
                string key;
                AnalyzedHit summed{};
                for (auto j = 0; j < r; ++j) {  // j: Index of the member of combination
                    const auto &h = hit.comb[j];
                    auto k = tmp % h.as.size();  // k: Index of the keys of as
                    tmp /= h.as.size();
                    const auto &found = h.as.at(*indexes[j][k]);
                    key += *indexes[j][k] + ",";
                    summed += found;
                }
                key.pop_back();
                hit.as.insert({move(key), summed});
            }
        }
        ret.push_back(move(hit));
    } while (prev_permutation(pool.begin(), pool.end()));
    return ret;
};


vector<dltools::CombinedHit> dltools::combine(std::vector<Hit> hits, int r,
                                              std::unordered_set<std::string> white_list) {
    std::vector<Hit> filtered;
    for (auto &h : hits) {
        bool found = false;
        for (const auto &d : h.as) {
            if (white_list.find(d.first) != white_list.end()) {
                found = true;
                continue;
            }
        }
        if (found) {
            filtered.push_back(move(h));
        }
    }
    auto n = filtered.size();
    if (n < r) {
        return vector<CombinedHit>();
    }

    auto ret = vector<CombinedHit>();
    vector<bool> pool(n, false);
    fill(pool.begin(), pool.begin() + r, true);
    do {
        CombinedHit hit;
        {  // Set member CombinedHit::comb
            vector<int> indexes;
            for (int j = 0; j < n; ++j) {
                if (pool[j]) {
                    indexes.push_back(j);
                }
            }
            for (auto j : indexes) {
                hit.comb.push_back(filtered[j]);
            }
        }
        {  // Set member CombinedHit::as
            int m = 1;  // Number of total loop
            vector<vector<const string *>> indexes;  // Vector of indexed key list
            for (const auto &h : hit.comb) {  // Set variable m and indexes
                m *= h.as.size();
                vector<const string *> tmp;
                for (const auto &d : h.as) {
                    tmp.push_back(&d.first);
                }
                indexes.push_back(move(tmp));
            }
            for (int i = 0; i < m; i++) {  // i: Index of the total loop (= (j, k), roughly speaking)
                auto tmp = i;
                string key;
                AnalyzedHit summed{};
                for (auto j = 0; j < r; ++j) {  // j: Index of the member of combination
                    const auto &h = hit.comb[j];
                    auto k = tmp % h.as.size();  // k: Index of the keys of as
                    tmp /= h.as.size();
                    const auto &found = h.as.at(*indexes[j][k]);
                    key += *indexes[j][k] + ",";
                    summed += found;
                }
                key.pop_back();
                hit.as.insert({move(key), summed});
            }
        }
        ret.push_back(move(hit));
    } while (prev_permutation(pool.begin(), pool.end()));
    return ret;
};
