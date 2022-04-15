#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
using namespace std;

class Command {
private:
    string commandWord;
    string secondWord;

public:
    Command(const string& firstWord, const string& secondWord);
    string getCommandWord();
    string getSecondWord();
    bool hasSecondWord();
};

#endif /*COMMAND_H_*/
