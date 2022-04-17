#include "CommandWords.h"

vector<string> CommandWords::validCommands;

/**
 * Constructor - initialise the command words.
 */
CommandWords::CommandWords() {
	// Populate the vector if it hasn't already.
	if (validCommands.empty()) {
		validCommands.push_back("go");
		validCommands.push_back("quit");
		validCommands.push_back("info");
		validCommands.push_back("map");
		validCommands.push_back("take");
		validCommands.push_back("put");
        //validCommands.push_back("teleport");
        validCommands.push_back("use");
        validCommands.push_back("check");
        validCommands.push_back("interact");
	}
}

// Puts all valid commands in a string used for printing
string CommandWords::validCommandsToString(){
    string allValidCommands = "";

    //Loops through validCommands and prints each to the screen.
    for (unsigned int i = 0; i < validCommands.size() - 1; i++)
    {
        allValidCommands += validCommands.at(i) + ", ";
    }

    // Last command won't have any commas
    allValidCommands += validCommands.at(validCommands.size() - 1);

    return allValidCommands;
}
