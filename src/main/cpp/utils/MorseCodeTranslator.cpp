#include "utils/MorseCodeTranslator.hpp"

void MorseCodeTranslator::getTranslatedMsg(std::string& msg, std::deque<std::string>& queue)
{
    for (auto it = msg.begin(); it != msg.end(); it++)
    {
        queue.push_back(morse_character_chart[*it]);
    }
}