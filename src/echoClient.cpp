#include <stdlib.h>
#include <connectionHandler.h>
#include <thread>
#include <EncoderDecoder.h>


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {
    //todo: change name
    //todo: add \n to all prints for the client
    //todo: check what to do with returned messages: process here of in encdec
    EncoderDecoder encDec1;
    EncoderDecoder encDec2;
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    //From here we will see the rest of the echo client implementation:

    bool terminated=false;
    std::thread readingThread([&connectionHandler,&terminated, &encDec1](){
        while (!terminated) {
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            encDec1.encode(line);
            if (!connectionHandler.sendLine(line)) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                terminated=true;
                break;
            }
            int spaceAt = line.find_first_of(' ');
            std::string opType = line.substr(0,spaceAt); //todo: remove?
//            if (line=="LOGOUT")
//                terminated=true;
        }
    });
    readingThread.detach();

    int running=1;
    while (running) {
        std::string answer;
        if (!connectionHandler.getLine(answer)) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            terminated=true;
            break;
        }
        encDec2.decode(answer);
        int spaceAt = answer.find_first_of(' ');
        std::string opType = answer.substr(0,spaceAt);
//		int len=answer.length();
//        answer.resize(len-1); //to remove the ; at the end
        std::cout << answer << std::endl;
        if(answer=="ACK 3\n")
        {
            terminated=true;
            running=0;
        }
        /*
        if(answer=="ACK"){
            std::string cut_answer(answer.substr(spaceAt+1, answer.length()));
            std::vector<std::string> words;
            std::stringstream ss(cut_answer);
            while(ss.good()) { //gets all words in the answer except the opType
                std::string subString;
                getline(ss, subString, ' ');
                words.push_back(subString);
            }
            if(words.size()<2){ //no editional content
                std::string messageOp = words.at(1);
                std::cout <<"ACK " + messageOp + "\n" << std::endl;
                if(messageOp=="3"){ //if logged out successfully
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    terminated=true;
                    break;
                }
            }
            else{ //has more content
                std::cout <<"ACK " + words.at(1) + " " + words.at(2) + "\n" << std::endl;
            }
        }
        else if(answer=="ERROR"){
            std::string cut_answer(answer.substr(spaceAt+1, answer.length()));
            std::vector<std::string> words;
            std::stringstream ss(cut_answer);
            while(ss.good()) { //gets all words in the answer except the opType
                std::string subString;
                getline(ss, subString, ' ');
                words.push_back(subString);
            }
            std::cout <<"ERROR " + words.at(1) + "\n" << std::endl;
        }
        else if(answer=="NOTIFICATION"){
            std::string cut_answer(answer.substr(spaceAt+1, answer.length()));
            std::vector<std::string> words;
            std::stringstream ss(cut_answer);
            while(ss.good()) { //gets all words in the answer except the opType
                std::string subString;
                getline(ss, subString, ' ');
                words.push_back(subString);
            }
            std::cout <<"NOTIFICATION " + words.at(1) + " " + words.at(2) + " " + words.at(3) + " " + "\n" << std::endl;
        }
        */
    }
    return 0;
}
