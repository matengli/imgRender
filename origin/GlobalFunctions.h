//
// Created by someone on 2019/12/27.
//

#ifndef IMGRENDER_GLOBALFUNCTIONS_H
#define IMGRENDER_GLOBALFUNCTIONS_H

#include <vector>
#include <string>

std::vector<std::string> splice(const std::string &str, const std::string &splitStr) {
    std::vector<std::string> result;
    int lastIndex = 0;
    while (1) {
        int findIndex = str.find(splitStr, lastIndex);
        if (findIndex == -1) {
            result.push_back(str.substr(lastIndex, str.size() - lastIndex));
            break;
        } else {
            result.push_back(str.substr(lastIndex, findIndex - lastIndex));
            lastIndex = findIndex + splitStr.size();
        }
    }

    return result;
}

inline float str2float(std::string str) {
    float val = 0.;
    std::stringstream(str) >> val;
    return val;
}

inline float str2int(std::string str) {
    int val = 0.;
    std::stringstream(str) >> val;
    return val;
}

#endif //IMGRENDER_GLOBALFUNCTIONS_H
