#pragma once

class AutoStartPositionAnglePair
{
public:
    AutoStartPositionAnglePair(double red_start_angle)
    {
        m_red_start_angle = red_start_angle;
        m_blue_start_angle = (180.0 + m_red_start_angle);
        m_blue_start_angle = m_blue_start_angle > 180.0 ? m_blue_start_angle - 360.0 : m_blue_start_angle;
    }

    double get_red_start_angle() { return m_red_start_angle;};
    double get_blue_start_angle() { return m_blue_start_angle;};

private:
    double m_red_start_angle = 0;
    double m_blue_start_angle = 0;
};