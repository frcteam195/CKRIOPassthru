#pragma once

#include "Singleton.hpp"
#include <mutex>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include "GlobalConfig.hpp"

namespace std {
    template<typename _CharT, typename _Traits>
    inline basic_ostream<_CharT, _Traits>&
    endl2(basic_ostream<_CharT, _Traits>& __os)
    {
        flush(__os.put(__os.widen('\n')));
        return flush(__os.put(__os.widen('\n')));
    }
}


class CKLogger
{
public:
    template<class T>
    CKLogger& operator<<(const T& v)
    {
#ifdef CONSOLE_REPORTING
        std::scoped_lock<std::mutex> lock(listLock);
        std::string s = toString(v);
        if (!msgList.count(s))
        {
            msgList[s] = 0;
            mStream << v;
            mCurrMsg.clear();
            mCurrMsg << v;
        }
#endif
        return *this;
    }

    CKLogger& operator<<(std::ostream& (*F)(std::ostream&))
    {
#ifdef CONSOLE_REPORTING
        std::scoped_lock<std::mutex> lock(listLock);
        if (mPrevMsg.str() != mCurrMsg.str())
        {
            mStream << F;
            mPrevMsg.clear();
            mPrevMsg << mCurrMsg.rdbuf();
        }
#endif
        return *this;
    }

    void messagePump()
    {
#ifdef CONSOLE_REPORTING
        std::scoped_lock<std::mutex> lock(listLock);
        // mStream.flush();
        std::cout << mStream.rdbuf() << std::endl;
        msgList.clear();
#endif
    }

    CKLogger() {}
    ~CKLogger() {}
private:
    std::mutex listLock;
    std::unordered_map<std::string, int> msgList;
    std::stringstream mStream;
    std::stringstream mCurrMsg;
    std::stringstream mPrevMsg;

    template<typename T>
    std::string toString(const T& x)
    {
        std::ostringstream os;
        os << x;
        return os.str();
    }
};

extern CKLogger ckLogger;