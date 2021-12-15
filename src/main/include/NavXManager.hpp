#pragma once

#include "utils/Singleton.hpp"
#include <vector>
#include <map>
#include <mutex>
#include <functional>
#include "utils/drivers/NavX.hpp"

class NavXManager : public Singleton<NavXManager>
{
    friend Singleton;
public:
    NavX& getNavX();

private:
    NavX mNavX;
};