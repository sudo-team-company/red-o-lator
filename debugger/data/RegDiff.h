#ifndef RED_O_LATOR_REGDIFF_H
#define RED_O_LATOR_REGDIFF_H

#include <string>
#include <vector>

struct RegDiff {
    std::vector<std::pair<std::string, int>> entries;

    void addDiff(const std::string& name, int value) {
        entries.emplace_back(name, value);
    }
};

#endif  // RED_O_LATOR_REGDIFF_H
