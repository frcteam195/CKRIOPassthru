#pragma once

namespace ck
{
    template<class T>
    class Sendable
    {
    public:
        Sendable() { m_value = 0; }
        Sendable(T value) { m_value = value; }
        T m_value;
    };
}