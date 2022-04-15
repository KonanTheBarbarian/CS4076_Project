#include "WordleMachine.h"
#include <QTextStream>
#include <QFile>
#include <mainwindow.h>
#include "dialogues.h"

using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

vector<string> WordleMachine::allWords;
int WordleMachine::triesLeft;
WordleMachine::gameStatus WordleMachine::wordleStatus = WordleMachine::WORDLE_STOP;
string WordleMachine::targetWord;

bool WordleMachine::isEqualIgnoreCase(char& first, char& second){
    return tolower(first) == tolower(second);
}

void WordleMachine::initWords(string filename){
    if(filename.empty()){
        string text;

        QFile file(":/text/wordle_words.txt");
        QTextStream in(&file);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            while(!in.atEnd()){
                //qDebug() << in.readLine();
                WordleMachine::allWords.push_back(in.readLine().toStdString());
            }

        }

        file.close();
    }
}

void WordleMachine::initialiseWordleMachine(){
    initWords("");
    WordleMachine::wordleStatus = WordleMachine::WORDLE_PROGRESS;
}

string WordleMachine::evaluateInput(const string &input){
    string output = "";
    std::unordered_map<char, int> letter_counts;
    string correctWord = WordleMachine::targetWord;
    int correctLettersAmount = 0;

    if(input.size() != correctWord.size()){
        return "";
    }

    // Initialising the letter_counts map, pairing each letter with 1
    for(int i = 0; i < (int) correctWord.size(); i++){
        char currentChar = tolower(correctWord.at(i));
        // If the letter does not exist yet, add it and pair it with the int 1
        // Otherwise, add 1 to the entry.
        if(letter_counts.count(currentChar)){
            letter_counts[currentChar] += 1;
        }
        else{
            letter_counts.insert({currentChar, 1});
        }
    }

    for(int i = 0; i < (int) input.size(); i++){
        char currentInputLetter = input.at(i);

        // If the character is at the correct index
        if(WordleMachine::isEqualIgnoreCase(currentInputLetter, correctWord.at(i))){
            letter_counts[currentInputLetter] -= 1;
            output.push_back('[');
            output.push_back(currentInputLetter);
            output.push_back(']');
            correctLettersAmount++;

        }
        // If the character exists but is in the wrong index
        // In this case, we also check the letter_counts map to see if there are any of that particular letter remaining.
        else if(letter_counts[currentInputLetter] > 0){
            letter_counts[currentInputLetter] -= 1;
            output.push_back('{');
            output.push_back(currentInputLetter);
            output.push_back('}');
        }
        else{
            output.push_back(currentInputLetter);
        }
    }

    output = output + " DEV: " + correctWord + '\n';

    // If the guess is correct, set status to success
    if(correctLettersAmount == (int) correctWord.size()){
        output += OtherDialogues::wordleSuccess;
        WordleMachine::wordleStatus = WordleMachine::WORDLE_SUCCESS;
    }
    else{
        WordleMachine::triesLeft--;

        if(WordleMachine::triesLeft == 0){
            output += OtherDialogues::wordleOutOfAttempts;
            WordleMachine::wordleStatus = WordleMachine::WORDLE_STOP;
        }
        else{
            output += Dialogues::printAttemptsLeft(WordleMachine::triesLeft);
        }
    }

    return output;
}

vector<string> WordleEngine::getAllWords(){
    return WordleMachine::allWords;
}

void WordleEngine::startWordleGame(){
    // Randomising
    srand(time(NULL));
    WordleMachine::triesLeft = 5;
    // Get a random word
    WordleMachine::targetWord = allWords.at(rand() % allWords.size());
}

WordleMachine::gameStatus WordleMachine::getWordleStatus(){
    return WordleMachine::wordleStatus;
}
