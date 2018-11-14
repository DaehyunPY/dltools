//
// Created by Daehyun You on 11/5/18.
//

#include <random>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include "hittype.h"


using std::move;
using std::fill;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::string;
using std::to_string;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::max;
using std::min_element;
using std::prev_permutation;
using dltools::AnalyzedHit;
using dltools::Hit;
using dltools::Hits;
using dltools::CombinedHit;
using dltools::CombinedHits;


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


dltools::Hits dltools::zip_to_hits(
        std::vector<double> t,
        std::vector<double> x,
        std::vector<double> y) {
    Hits ret;
    auto it = t.begin();
    auto ix = x.begin();
    auto iy = y.begin();
    for (; it != t.end() and ix != x.end() and iy != y.end(); ++it, ++ix, ++iy) {
        ret.push_back(Hit{.t=*it, .x=*ix, .y=*iy});
    }
    return ret;
}


dltools::Hits dltools::zip_to_hits(
        std::vector<double> t,
        std::vector<double> x,
        std::vector<double> y,
        std::vector<int> flag) {
    Hits ret;
    auto it = t.begin();
    auto ix = x.begin();
    auto iy = y.begin();
    auto iflag = flag.begin();
    for (; it != t.end() and ix != x.end() and iy != y.end() and iflag != flag.end(); ++it, ++ix, ++iy, ++iflag) {
        ret.push_back(Hit{
            .t=*it,
            .x=*ix,
            .y=*iy,
            .as_={},
            .flag=make_shared<int>(*iflag)
        });
    }
    return ret;
}


dltools::CombinedHits dltools::combine(dltools::Hits hits, int r) {
    Hits filtered;
    for (auto &h : hits) {
        if (not h.as_.empty()) {
            filtered.push_back(move(h));
        }
    }
    auto n = filtered.size();
    if (n < r) {
        return CombinedHits();
    }

    auto ret = CombinedHits();
    vector<bool> pool(n, false);
    fill(pool.begin(), pool.begin() + r, true);
    do {
        CombinedHit hit{};
        {  // Set member CombinedHit::comb
            vector<int> indexes;
            for (int j = 0; j < n; ++j) {
                if (pool[j]) {
                    indexes.push_back(j);
                }
            }
            shared_ptr<int> flag;
            for (auto j : indexes) {
                hit.comb.push_back(filtered[j]);
                if (flag) {
                    if (filtered[j].flag) {
                        flag = make_shared<int>(max(*flag, *filtered[j].flag));
                    }
                } else {
                    if (filtered[j].flag) {
                        flag = make_shared<int>(*filtered[j].flag);
                    }
                }
            }
            hit.flag = flag;
        }
        {  // Set member CombinedHit::as_
            int m = 1;  // Number of total loop
            vector<vector<const string *>> indexes;  // Vector of indexed key list
            for (const auto &h : hit.comb) {  // Set variable m and indexes
                m *= h.as_.size();
                vector<const string *> tmp;
                for (const auto &d : h.as_) {
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
                    auto k = tmp % h.as_.size();  // k: Index of the keys of as_
                    tmp /= h.as_.size();
                    const auto &found = h.as_.at(*indexes[j][k]);
                    key += *indexes[j][k] + ",";
                    summed += found;
                }
                key.pop_back();
                hit.as_.insert({move(key), summed});
            }
        }
        ret.push_back(move(hit));
    } while (prev_permutation(pool.begin(), pool.end()));
    return ret;
}


dltools::CombinedHits dltools::combine(dltools::Hits hits, int r, const std::unordered_set<string> &white_list) {
    Hits filtered;
    for (auto &h : hits) {
        bool found = false;
        for (const auto &d : h.as_) {
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
        return CombinedHits();
    }

    auto ret = CombinedHits();
    vector<bool> pool(n, false);
    fill(pool.begin(), pool.begin() + r, true);
    do {
        CombinedHit hit{};
        {  // Set member CombinedHit::comb
            vector<int> indexes;
            for (int j = 0; j < n; ++j) {
                if (pool[j]) {
                    indexes.push_back(j);
                }
            }
            shared_ptr<int> flag;
            for (auto j : indexes) {
                hit.comb.push_back(filtered[j]);
                if (flag) {
                    if (filtered[j].flag) {
                        flag = make_shared<int>(max(*flag, *filtered[j].flag));
                    }
                } else {
                    if (filtered[j].flag) {
                        flag = make_shared<int>(*filtered[j].flag);
                    }
                }
            }
            hit.flag = flag;
        }
        {  // Set member CombinedHit::as_
            int m = 1;  // Number of total loop
            vector<vector<const string *>> indexes;  // Vector of indexed key list
            for (const auto &h : hit.comb) {  // Set variable m and indexes
                m *= h.as_.size();
                vector<const string *> tmp;
                for (const auto &d : h.as_) {
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
                    auto k = tmp % h.as_.size();  // k: Index of the keys of as_
                    tmp /= h.as_.size();
                    const auto &found = h.as_.at(*indexes[j][k]);
                    key += *indexes[j][k] + ",";
                    summed += found;
                }
                key.pop_back();
                hit.as_.insert({move(key), summed});
            }
        }
        ret.push_back(move(hit));
    } while (prev_permutation(pool.begin(), pool.end()));
    return ret;
}


CombinedHit dltools::as_minsqsum(CombinedHit hit) {
    unordered_map<string, double> sqsum;
    for (const auto &p : hit.as_) {
        sqsum.insert(pair<string, double>(
                p.first,
                p.second.pz * p.second.pz
                + p.second.px * p.second.px
                + p.second.py * p.second.py
        ));
    }
    auto found = min_element(
            sqsum.begin(),
            sqsum.end(),
            [](const pair<string, double> &a, const pair<string, double> &b) {
                return a.second < b.second;
            }
    );
    unordered_map<string, AnalyzedHit> as_;
    if (found != sqsum.end()) {
        as_.insert({found->first, hit.as_.at(found->first)});
    }
    return CombinedHit{
        .comb=move(hit.comb),
        .as_=move(as_),
        .flag=hit.flag
    };
}


CombinedHit dltools::as_minsqsum(CombinedHit hit, const std::unordered_set<string> &white_list) {
    unordered_map<string, double> sqsum;
    for (const auto &p : hit.as_) {
        if (white_list.find(p.first) != white_list.end()) {
            sqsum.insert(pair<string, double>(
                    p.first,
                    p.second.pz * p.second.pz
                    + p.second.px * p.second.px
                    + p.second.py * p.second.py
            ));
        }
    }
    auto found = min_element(
            sqsum.begin(),
            sqsum.end(),
            [](const pair<string, double> &a, const pair<string, double> &b) {
                return a.second < b.second;
            }
    );
    unordered_map<string, AnalyzedHit> as_;
    if (found != sqsum.end()) {
        as_.insert({found->first, hit.as_.at(found->first)});
    }
    return CombinedHit{
            .comb=move(hit.comb),
            .as_=move(as_),
            .flag=hit.flag
    };
}


CombinedHits dltools::as_minsqsum(CombinedHits hits) {
    dltools::CombinedHits ret;
    for (auto &h : hits) {
        auto min = as_minsqsum(move(h));
        if (not min.as_.empty()) {
            ret.push_back(move(min));
        }
    }
    return ret;
}


CombinedHits dltools::as_minsqsum(CombinedHits hits, const std::unordered_set<std::string> &white_list) {
    dltools::CombinedHits ret;
    for (auto &h : hits) {
        auto min = as_minsqsum(move(h), white_list);
        if (not min.as_.empty()) {
            ret.push_back(move(min));
        }
    }
    return ret;
}


CombinedHits dltools::filter_duphits(CombinedHits hits) {
    auto n = hits.size();
    vector<int> index(n);
    for (auto i = 0; i < n; ++i) {
        index.push_back(i);
    }
    std::shuffle(index.begin(), index.end(), std::mt19937(std::random_device()()));
    unordered_set<double> pool;
    vector<bool> where(n, false);
    for (auto i : index) {
        bool found = false;
        for (const auto &h : hits[i].comb) {
            if (pool.find(h.t) != pool.end()) {
                found = true;
                break;
            }
        }
        if (not found) {
            where[i] = true;
            for (const auto &h : hits[i].comb) {
                pool.insert(h.t);
            }
        }
    }
    dltools::CombinedHits ret;
    for (auto i = 0; i < n; ++i) {
        if (where[i]) {
            ret.push_back(move(hits[i]));
        }
    }
    return ret;
}
