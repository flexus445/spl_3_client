#include <EncoderDecoder.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
//#include <stdio.h>


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
        char* opCodeBytes;
        shortToBytes(1,opCodeBytes);
        line.append(opCodeBytes + username + zero + password + zero + birthday);
    }
    else if(opType=="LOGIN"){
        std::string username = words.at(1);
        std::string password = words.at(2);
        std::string capcha = words.at(3);
        char zero = '\0';
        char* opCodeBytes;
        shortToBytes(2,opCodeBytes);
        line.append(opCodeBytes + username + zero + password + zero + capcha);
    }
    else if(opType=="LOGOUT"){
        char* opCodeBytes;
        shortToBytes(3,opCodeBytes);
        line.append(opCodeBytes);
    }
    else if(opType=="FOLLOW"){
        std::string follow_unfollow = words.at(1);
        std::string username = words.at(2);
        char zero = '\0';
        char* opCodeBytes;
        shortToBytes(4,opCodeBytes);
        line.append(opCodeBytes + follow_unfollow + zero + username + zero);
    }
    else if(opType=="POST"){
        std::string content = words.at(1);
        char zero = '\0';
        char* opCodeBytes;
        shortToBytes(5,opCodeBytes);
        line.append(opCodeBytes + content + zero);
    }
    else if(opType=="PM"){
        std::string username = words.at(1);
        std::string content = words.at(2);
        char zero = '\0';
        char* opCodeBytes;
        shortToBytes(6,opCodeBytes);

        time_t now = time(0);
        char* dt = ctime(&now); //format: Sat Jan  8 20:07:41 2011
        line.append(opCodeBytes + username + zero + content + zero + dt + zero);
    }
    else if(opType=="LOGSTAT"){
        char* opCodeBytes;
        shortToBytes(7,opCodeBytes);
        line.append(opCodeBytes);
    }
    else if(opType=="STAT"){
        std::string usernames = words.at(1);
        char zero = '\0';
        char* opCodeBytes;
        shortToBytes(8,opCodeBytes);
        line.append(opCodeBytes + usernames + zero);
    }
    else if(opType=="BLOCK"){
        std::string username = words.at(1);
        char zero = '\0';
        char* opCodeBytes;
        shortToBytes(12,opCodeBytes);
        line.append(opCodeBytes + username + zero);
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
    short opCode = bytesToShort(opCodeBytes);
    line="";


    if(opCode==9){ //notification
        char notificationType = chars.at(2);
        int i=3;
        std::string postingUser;
        while (i<chars.size() && chars.at(i)!='0'){
            postingUser.append(&chars.at(i));//todo: check it
            i++;
        }
        i++; //todo: check if it skips the 0
        std::string content;
        while (i<chars.size() && chars.at(i)!='0'){
            content.append(&chars.at(i));//todo: check it
            i++;
        }
        std::string type = "PM";
        if(notificationType=='1')
            type="Public";
        line.append("NOTIFICATION " + type + " " + postingUser + " " + content + "\n");
//        printf("NOTIFICATION %s %s %s\n", type.c_str(), postingUser.c_str(), content.c_str());//todo: check it


    }
    else if(opCode==10){ //ack
        char messageOpBytes[] {chars.at(0), chars.at(1)};
        short messageOp = bytesToShort(messageOpBytes);
        if (chars.size()>4){
            int i=4;
            std::string content;
            while (i<chars.size()){
                content.append(&chars.at(i));//todo: check it
                i++;
            }
            line.append("ACK " + std::to_string(messageOp) + " " + content + "\n");

//            printf("ERROR %d %s\n", messageOp, content.c_str());//todo: check it
        }
        else{
            line.append("ACK " + std::to_string(messageOp) + "\n");

//            printf("ERROR %d\n", messageOp);

        }
    }
    else if(opCode==11){ //error
        char messageOpBytes[] {chars.at(0), chars.at(1)};
        short messageOp = bytesToShort(messageOpBytes);
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