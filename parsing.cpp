#include "Parser.h"

Parser::Parser() {
    commands = new Command();
}

Parser::~Parser(){
    delete commands;
}

Command* Parser::convertToCommand(string input){
    string word1 = "";
    string word2 = "";
    vector<string> words;

    // represent size of a string.
    string::size_type pos = 0, last_pos = 0;

    pos = input.find_first_of(' ', last_pos);
    if (pos == string::npos ) {
        words.push_back(input);
    } else {
        words.push_back(input.substr(0, pos));
        words.push_back(input.substr(pos + 1, input.size() - pos - 1));
    }

    if (words.size() == 1) //was only 1 word entered?
        word1 = words[0]; //get first word
    else if (words.size() >= 2) { //were at least 2 words entered?
        word1 = words[0]; //get first word
        word2 = words[1]; //get second word
    }

    return new Command(word1, word2);
}

string Parser::commandsInString(){
    return commands->validCommandsToString();
}
