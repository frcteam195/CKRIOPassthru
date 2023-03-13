#include "utils/drivers/LEDMorseController.hpp"
#include <string>
#include <frc/Timer.h>
#include <deque>
#include <utils/MorseCodeTranslator.hpp>

static constexpr int MIN_LED_THREAD_LOOP_MS = 50;
static constexpr int kDefaultBlinkCount = 6;
static constexpr double kDefaultBlinkDuration = 0.2; // seconds for full cycle
static constexpr double kDefaultTotalBlinkDuration = kDefaultBlinkCount * kDefaultBlinkDuration;
static constexpr double kSlowBlinkDivisor = 1.5;
static constexpr double kFastBlinkDivisor = 3;
static constexpr double kLetterPause = 0.5;
static constexpr double kWordPause = 0.75;


enum class MorseState
{
    LOAD,
    NEXT_CHAR,
    NEXT_MORSE_CODE,
    BLINK_ON,
    BLINK_OFF,
    DONE
};

void setLEDOff(CANdle* candle, int num_leds)
{
    candle->SetLEDs(0, 0, 0, 0, 0, num_leds);
}

void setLEDOn(CANdle* candle, int num_leds, RGBColor& color)
{
    candle->SetLEDs(color.red, color.green, color.blue, color.white, 0, num_leds);
}

void update_morse_controller(CANdle* candle, int num_leds, const std::string& msg, const ck::RGBWColor& color)
{
    static std::string current_msg = "";
    static RGBColor current_color;
    static MorseState current_state = MorseState::LOAD;
    static double current_state_time = frc::Timer::GetFPGATimestamp().value();
    static std::deque<std::string> current_morse_code_list;
    static std::deque<char> current_morse_code;
    static char current_morse_char;
    static int current_num_leds = 0;

    current_num_leds = num_leds;

    if (current_color != color)
    {
        current_color = color;
    }

    if (current_msg != msg)
    {
        current_state = MorseState::LOAD;
        current_msg = msg;
    }

    switch (current_state)
    {
        case MorseState::LOAD:
        {
            current_morse_code_list.clear();

            if (current_msg.length() <= 0)
            {
                //END
            }

            MorseCodeTranslator::getTranslatedMsg(current_msg, current_morse_code_list);
            if (current_morse_code_list.size() > 0)
            {
                setLEDOff(candle, current_num_leds);
                current_state = MorseState::NEXT_CHAR;
            }
            else
            {
                //END
            }
            break;
        }
        case MorseState::NEXT_CHAR:
        {
            if (current_morse_code_list.size() > 0)
            {
                std::string morse_code = current_morse_code_list.front();
                current_morse_code_list.pop_front();
                current_morse_code = std::deque<char>(morse_code.begin(), morse_code.end());
                current_state = MorseState::NEXT_MORSE_CODE;
            }
            else
            {
                current_state = MorseState::DONE;
                current_state_time = frc::Timer::GetFPGATimestamp().value();
            }
        break;
        }
        case MorseState::NEXT_MORSE_CODE:
        {
            if (current_morse_code.size() > 0)
            {
                current_morse_char = current_morse_code.front();
                current_morse_code.pop_front();
                current_state = MorseState::BLINK_ON;
                current_state_time = frc::Timer::GetFPGATimestamp().value();
            }
            else
            {
                //Delay to delineate between letters in a string
                if ((frc::Timer::GetFPGATimestamp().value() - current_state_time) > kLetterPause)
                {
                    current_state = MorseState::NEXT_CHAR;
                    //morseStateTime = Timer.getFPGATimestamp();
                }
            }
            break;
        }
        case MorseState::BLINK_ON:
        {
            if (current_morse_char == '.' || current_morse_char == '-') {
                double divisorVal = current_morse_char == '.' ? kFastBlinkDivisor : kSlowBlinkDivisor;
                setLEDOn(candle, current_num_leds, current_color);
                if ((frc::Timer::GetFPGATimestamp().value() - current_state_time) > kDefaultBlinkDuration / divisorVal) {
                    current_state = MorseState::BLINK_OFF;
                    current_state_time = frc::Timer::GetFPGATimestamp().value();
                }
            } else {
                current_state = MorseState::NEXT_MORSE_CODE;
            }
            break;
        }
        case MorseState::BLINK_OFF:
        {
            if (current_morse_char == '.' || current_morse_char == '-') {
                double divisorVal = current_morse_char == '.' ? kFastBlinkDivisor : kSlowBlinkDivisor;
                setLEDOff(candle, current_num_leds);
                if ((frc::Timer::GetFPGATimestamp().value() - current_state_time) > kDefaultBlinkDuration / divisorVal) {
                    current_state = MorseState::NEXT_MORSE_CODE;
                    current_morse_char = ' ';
                    current_state_time = frc::Timer::GetFPGATimestamp().value();
                }
            } else {
                current_state = MorseState::NEXT_MORSE_CODE;
            }
            break;
        }
        case MorseState::DONE:
        {
            if ((frc::Timer::GetFPGATimestamp().value() - current_state_time) > kWordPause) {
                //END
            }
            break;
        }
    }

}