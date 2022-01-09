#pragma once

#include "CKMathConstants.hpp"
#include "Units.hpp"
#include <map>

namespace ck
{
    namespace math
    {
        template <typename T>
        inline T max(T a, T b)
        {
            return a > b ? a : b;
        }

        template <typename T>
        inline T min(T a, T b)
        {
            return a < b ? a : b;
        }

        template <typename T>
        inline int signum(T val)
        {
            return (T(0) < val) - (val < T(0));
        }

        template <typename T>
        inline double degToRad(T val)
        {
            return (val * M_PI / 180.0);
        }

        template <typename T>
        inline double radToDeg(T val)
        {
            return (val * 180.0 / M_PI);
        }

        template <typename T>
        inline bool epsilonEquals(T const &a, T const &b, T epsilon)
        {
            return (a - epsilon <= b) && (a + epsilon >= b);
        }

        template <typename T>
        inline bool epsilonEquals(T const &a, T const &b)
        {
            return epsilonEquals(a, b, kEpsilon);
        }

        template <typename T>
        inline T limit(T v, T maxMagnitude)
        {
            return limit(v, -maxMagnitude, maxMagnitude);
        }

        template <typename T>
        inline T limit(T v, T minVal, T maxVal)
        {
            return min(maxVal, max(minVal, v));
        }

        template <typename T>
        inline T interpolate(T a, T b, T x)
        {
            x = limit(x, 0.0, 1.0);
            return a + (b - a) * x;
        }

        template <typename K, typename V>
        inline double interpolate(const std::map<K, V> &data, K x)
        {
            typedef typename std::map<K, V>::const_iterator i_t;

            i_t i = data.upper_bound(x);
            if (i == data.end())
            {
                return (--i)->second;
            }
            if (i == data.begin())
            {
                return i->second;
            }
            i_t l = i;
            --l;

            const K delta = (x - l->first) / (i->first - l->first);
            return delta * i->second + (1 - delta) * l->second;
        }

        template <typename T>
        T normalizeWithDeadband(T val, T deadband) {
            val = (std::fabs(val) > std::fabs(deadband)) ? val : 0.0;

            if (val != 0)
            {
                val = signum(val) * ((std::fabs(val) - deadband) / (1.0 - deadband));
            }

            return (std::fabs(val) > std::fabs(deadband)) ? val : 0.0;
        }
    } // namespace math
} // namespace ck
