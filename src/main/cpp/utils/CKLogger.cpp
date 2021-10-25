#include "utils/CKLogger.hpp"
#include <iostream>

CKLogger ckLogger;

CKLogger::CKLogger()
{

}

CKLogger::~CKLogger()
{

}

void CKLogger::messagePump()
{
    std::scoped_lock<std::mutex> lock(listLock);
    while (!msgList.empty())
    {
        std::cout << msgList.begin()->first << std::endl;
        msgList.erase(msgList.begin());
    }
}

template<typename T>
const CKLogger& CKLogger::operator<<(const T& v) const
{
    std::scoped_lock<std::mutex> lock(listLock);
    msgList[toString(v)] = 0;
    return *this;
}

const CKLogger& CKLogger::operator<<(std::ostream& (*F)(std::ostream&)) const
{
    return *this;
}

template<typename T>
std::string CKLogger::toString(const T& x)
{
    std::ostringstream os;
    os << x;
    return os.str();
}