#include "failover/subsystems/LEDFailover.hpp"
#include "failover/FailoverMessageManager.hpp"

void LEDFailover::init()
{

}

void LEDFailover::uninit()
{

}

void LEDFailover::periodic()
{

}

void LEDFailover::run()
{

}

LEDFailover::LEDFailover()
{
    mLED = FailoverMessageManager::getInstance().addLEDControl();
    mLED->set_id(0);
    mLED->set_led_type(ck::LEDControl::LEDControlData::LEDStripType::LEDControl_LEDControlData_LEDStripType_GRBW);
    mLED->set_led_control_mode(ck::LEDControl::LEDControlData::LEDControlMode::LEDControl_LEDControlData_LEDControlMode_Static);
    ck::LEDControl::LEDColor* led_color = new ck::LEDControl::LEDColor();
    ck::RGBWColor* rgbw_color = new ck::RGBWColor();
    rgbw_color->set_r(255);
    rgbw_color->set_g(0);
    rgbw_color->set_b(0);
    rgbw_color->set_w(0);
    led_color->set_allocated_rgbw_color(rgbw_color);
    led_color->set_num_leds(NUM_LEDS);
    led_color->set_start_index(0);
    mLED->set_allocated_color(led_color);
    mLED->set_can_network(ck::CANNetwork::RIO_CANIVORE);
    mLED->set_vbat_config(ck::LEDControl::LEDControlData::VBATConfigType::LEDControl_LEDControlData_VBATConfigType_Off);
}

LEDFailover::~LEDFailover()
{
    uninit();
}