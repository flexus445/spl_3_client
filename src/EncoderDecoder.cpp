#include <EncoderDecoder.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
//#include <stdio.h>
using namespace std;


EncoderDecoder::EncoderDecoder(){}

bool EncoderDecoder::encode(std::string& line) {
    //todo: insert byte 0 between words
    std::vector<std::string> words;
    std::stringstream ss(line);
    while(ss.good()) {
        std::string subString;
        getline(ss, subString, ' ');
        words.push_back(subString);
    }
    line=""; //todo: check if needed clear
    std::string opType(words.at(0));
    if(opType=="REGISTER"){
        std::string username = words.at(1);
        std::string password = words.at(2);
        std::string birthday = words.at(3);
        char zero = '\0';
        char opCodeBytes[2]; //todo: DO LIKE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//        shortToBytes(1,opCodeBytes);
        short opTypeShort = 1;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
//        cout << to_string(opCodeBytes[0]) + " + " + to_string(opCodeBytes[1]) << endl;

//        if (opCode == 1){
//            for (int i = 0; i < line.size(); ++i) {
//                bytes[i+2] = line[i];
//            }
//            changeToZero(bytes,len);
//            bytes[len-2] = '\0';
//        }
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]) + username + zero + password + zero + birthday);
    }
    else if(opType=="LOGIN"){
        std::string username = words.at(1);
        std::string password = words.at(2);
        std::string capcha = words.at(3);
        char zero = '\0';
        char opCodeBytes[2];
        short opTypeShort = 2;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]) + username + zero + password + zero + capcha);
    }
    else if(opType=="LOGOUT"){
        char opCodeBytes[2];
        short opTypeShort = 3;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]));
    }
    else if(opType=="FOLLOW"){
        std::string follow_unfollow = words.at(1);
        std::string username = words.at(2);
        char zero = '\0';
        char opCodeBytes[2];
        short opTypeShort = 4;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]) + follow_unfollow + zero + username + zero);
    }
    else if(opType=="POST"){
        std::string content;
        for(unsigned int i=0; i<words.size()-1; i++) {
            content.append(words.at(i+1));
            if(i<words.size()-2)
                content.append(" ");
        }
        char zero = '\0';
        char opCodeBytes[2];
        short opTypeShort = 5;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]) + content + zero);
    }
    else if(opType=="PM"){
        std::string username = words.at(1);
        std::string content;
        for(unsigned int i=0; i<words.size()-2; i++) {
            content.append(words.at(i+2));
            if(i<words.size()-3)
                content.append(" ");
        }

        char zero = '\0';
        char opCodeBytes[2];
        short opTypeShort = 6;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);

        time_t now = time(0);
        char* dt = ctime(&now); //format: Sat Jan  8 20:07:41 2011
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]) + username + zero + content + zero + dt + zero);
    }
    else if(opType=="LOGSTAT"){
        char opCodeBytes[2];
        short opTypeShort = 7;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]));
    }
    else if(opType=="STAT"){
        std::string usernames = words.at(1);
        char zero = '\0';
        char opCodeBytes[2];
        short opTypeShort = 8;
        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(to_string(opCodeBytes[0]) + to_string(opCodeBytes[1]) + usernames + zero);
    }
    else if(opType=="BLOCK"){
        std::string username = words.at(1);
        char zero = '\0';
        char opCodeBytes[2];
        short opTypeShort = 12;
//        opCodeBytes[0] = ((opTypeShort >> 8) & 0xFF);
        opCodeBytes[1] = (opTypeShort & 0xFF);
        line.append(/*to_string(opCodeBytes[0]) +*/ to_string(opCodeBytes[1]) + username + zero);
    }
    else{
        std::cout << "Illegal command" << std::endl;
        return false;
    }


    line.append(";");
    return true;
}


bool EncoderDecoder::decode(std::string& line) {
    line.resize(line.length()-1); //to remove the ; at the end

    std::vector<char> chars(line.begin(), line.end());
    char opCodeBytes[] {chars.at(0), chars.at(1)};

//    int first = opCodeBytes[0] - '0';
//    int second = opCodeBytes[1] - '0';
    int opCode = convertTwoBytesToInt(opCodeBytes);
    line="";


    if(opCode==9){ //notification
        char notificationType = chars.at(2);
        int i=3;
        std::string postingUser;
//        while (i<chars.size() && chars.at(i)!='\0'){
            postingUser.append(&chars.at(i));//todo: check it
//            i++;
//        }
        i=4+postingUser.length(); //todo: check if it skips the 0
        std::string content;
//        while (i<chars.size() && chars.at(i)!='\0'){
//            content.append(&chars.at(i));//todo: check it
//            i++;
//        }
        content.append(chars.begin()+i,chars.end()-1);
        std::string type = "PM";
        if(notificationType=='1')
            type="Public";
        line.append("NOTIFICATION " + type + " " + postingUser + " " + content + "\n");
//        printf("NOTIFICATION %s %s %s\n", type.c_str(), postingUser.c_str(), content.c_str());//todo: check it


    }
    else if(opCode==10){ //ack
        char messageOpBytes[] {chars.at(2), chars.at(3)};
        int messageOp = convertTwoBytesToInt(messageOpBytes);
        if (chars.size()>4){
            int i=4;
            std::string content;
            content.append(chars.begin()+i,chars.end());
            line.append("ACK " + std::to_string(messageOp) + " " + content + "\n");

//            printf("ERROR %d %s\n", messageOp, content.c_str());//todo: check it
        }
        else{
            line.append("ACK " + std::to_string(messageOp) + "\n");

//            printf("ERROR %d\n", messageOp);

        }
    }
    else if(opCode==11){ //error
        char messageOpBytes[] {chars.at(2), chars.at(3)};
        int messageOp = convertTwoBytesToInt(messageOpBytes);
//        printf("ERROR %d\n", messageOp);
        line.append("ERROR " + std::to_string(messageOp) + "\n");

    }
    else{
        std::cout << "Error, received bad message.\n" << std::endl;
        return false;
    }

    return true;
}






void EncoderDecoder::shortToBytes(short num, char* bytesArr)
{
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

short EncoderDecoder::bytesToShort(char* bytesArr)
{
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[1] & 0xff);
    return result;
}

int EncoderDecoder::convertTwoBytesToInt(char* bytes){
    int first = bytes[0] - '0';
    int second = bytes[1] - '0';
    return first*10+second;
}