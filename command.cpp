#include "Command.h"


Command::Command(const string& firstWord, const string& secondWord) {
	this->commandWord = firstWord;
	this->secondWord = secondWord;
}

string Command::getCommandWord() {
	return this->commandWord;
}


string Command::getSecondWord() {
	return this->secondWord;
}

bool Command::hasSecondWord() {
    return (!secondWord.empty());
}
