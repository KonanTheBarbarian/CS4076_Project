#ifndef WORDLEMACHINE_H
#define WORDLEMACHINE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;



class WordleMachine{
public:
    enum gameStatus {WORDLE_STOP, WORDLE_PROGRESS, WORDLE_FAIL, WORDLE_SUCCESS};

private:
    // All valid words
    static vector<string> allWords;
    static int triesLeft;
    static void initWords(string filename);
    static string targetWord;
    static gameStatus wordleStatus;


public:
    static string evaluateInput(const string& input);

    static vector<string> getAllWords();
    static void startWordleGame();
    static string checkGameStatus();
    static void initialiseWordleEngine();
    static gameStatus getWordleStatus();

    static bool isEqualIgnoreCase(char& first, char& second);
};

#endif // WORDLEMACHINE_H
