#pragma once

#include "Singleton.hpp"
#include <mutex>
#include <ostream>
#include <sstream>
#include <unordered_map>


class CKLogger
{
public:
    template<typename T>
    const CKLogger& operator<<(const T& v) const;
    const CKLogger& operator<<(std::ostream& (*F)(std::ostream&)) const;
    void messagePump();

    CKLogger();
    ~CKLogger();
private:
    std::mutex listLock;
    std::unordered_map<std::string, int> msgList;

    template<typename T>
    static std::string toString(const T& x);
};

extern CKLogger ckLogger;