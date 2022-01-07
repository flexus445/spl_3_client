#ifndef CLIENT_ENCODERDECODER_H
#define CLIENT_ENCODERDECODER_H

#include <string>
#include <iostream>


class EncoderDecoder{
public:
    bool encode(std::string& line);
    bool decode(std::string& line);
    int convertTwoBytesToInt(char* bytes);
    EncoderDecoder();

private:
    void shortToBytes(short num, char* bytesArr);
    short bytesToShort(char* bytesArr);

};


#endif
