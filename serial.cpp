#include "serial.hpp"


void serialReadCAN()
{
    static std::string sentence;

    int32_t receivedCharacter = getchar_timeout_us(10000);  // 10ms timeout

    if (receivedCharacter == PICO_ERROR_TIMEOUT){
        return;
    }
    if (receivedCharacter != '\n' && receivedCharacter != '\r'){
        sentence.push_back((char)receivedCharacter);
        return;
    }

    std::vector<std::string> sentenceParts = split(sentence, ':');

    if (sentenceParts.size() > 9){
        sentence.clear();
        return;
    }
    can_frame NewCan;
    NewCan.can_id = stoi(sentenceParts[0], 0, 10);

    for (size_t i = 1; i < sentenceParts.size(); i++)   {NewCan.data[i-1] = stoi(sentenceParts[i], 0, 10);}
}


std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t pos = s.find(delimiter);
    while (pos != std::string::npos)
    {
        tokens.push_back(s.substr(start, pos - start));
        start = pos + 1;
        pos = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));
    return tokens;
}
