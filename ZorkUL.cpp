#include <iostream>
#include <cstdlib>
#include "mainwindow.h"

#include <QApplication>

#include <QTextStream>
#include <QDebug>
#include "dialogues.h"
#include "constants.h"
#include "Errors.h"
#include "item.h"

using namespace std;
#include "ZorkUL.h"

Parser *ZorkUL::parser;
Room *ZorkUL::currentRoom;
Quantities *ZorkUL::allQuantities;
vector<Item*> ZorkUL::itemsInInventory;
bool ZorkUL::keysPresent[5];
int ZorkUL::cash;
vector<Room*> ZorkUL::allRooms;
Stack<Room*> ZorkUL::recentRooms;
bool ZorkUL::canGoToSpace;

int main(int argc, char *argv[]) {
    ZorkUL::changeCash(2);

    //ZorkUL::parser = new Parser();
    Parser* parser = new Parser();
    ZorkUL::setParser(parser);

    // For printing stuff out in the output pane and debugging
    //QTextStream out(stdout);

    WordleMachine *worldleMachine = new WordleMachine();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    MainWindow *windowPtr = &w;


    w.show();
    w.clearConsole();

    // For testing that all valid Wordle words are present.
    //    for (string s : worldleMachine->getAllWords()){
    //        w.addToConsole(s);
    //    }

    ZorkUL::setAllRooms(ZorkUL::createRooms());
    w.addStringToConsole(OtherDialogues::welcome);

    //ZorkUL::updateRoom(ZorkUL::getAllRooms().at(0), windowPtr);
    ZorkUL::updateRoom(ZorkUL::getCurrentRoom(), windowPtr);

    string roomDescription = ZorkUL::getCurrentRoom()->getShortDescription();
    w.addStringToConsole(Dialogues::printCurrentRoom(roomDescription));

    delete worldleMachine;
    delete ZorkUL::getParser();
    return a.exec();
}

ZorkUL::ZorkUL() {
    for(int i = 0; i < (int) (sizeof(ZorkUL::keysPresent)/sizeof(ZorkUL::keysPresent[0])); i++){
        this->keysPresent[i] = false;
    }
    ZorkUL::allQuantities->KeysPresent = 0;
    ZorkUL::money = 10;
    ZorkUL::canGoToSpace = false;

    createRooms();
}

void ZorkUL::setAllRooms(vector<Room*> rooms){
    ZorkUL::allRooms = rooms;
}

vector<Room*> ZorkUL::getAllRooms(){
    return ZorkUL::allRooms;
}

// Cleaning up heaps
void ZorkUL::deleteAll(){
    currentRoom = NULL;

    for(auto& room : ZorkUL::getAllRooms()){
        delete room;
    }

    for(auto& item : ZorkUL::itemsInInventory){
        delete item;
    }

    ZorkUL::getAllRooms().clear();
    ZorkUL::itemsInInventory.clear();

}

vector<Room*> ZorkUL::createRooms()  {
    using namespace Constants;
    using namespace GoalCheckFunctions;
    using namespace InteractFunctions;
    using namespace useItemFunctions;

    Room *solar_system, *mercury,
            *venus, *earth, *mars, *jupiter,
            *saturn;
    GoalRoom *uranus, *neptune, *pluto;

    Item *rock, *sweets, *stylus;
    rock = new Item(&(useItemDefault), "Rock", ItemDialogues::rock);
    sweets = new Item(&(useItemDefault), "Sweets", ItemDialogues::sweets);
    stylus = new Item(&(useItemDefault),"Stylus", ItemDialogues::stylus);

    vector<Room*> allRooms;
    // Adding all rooms
    mercury = new Room("Mercury", RoomDialogues::mercury, MERCURY_PIC
    venus = new inteactPlainGoal), 100, "Venus", RoomDialogues::venus, VENUS_PIC);
    solar_system = new WordleRoom(&(interactPlainGoal), rock, "Solar System", RoomDialogues::solar_system, SOLAR_SYSTEM_PIC);
    uranus = new GoalRoom(&(interactPlainGoal), &(checkFinalGoalFunc),"Uranus", URANUS_PIC);
    neptune = new GoalRoom(&(interactPlainGoal), &(checkPeiCompleteFunc),"Neptune", NEPTUNE_PIC);
    earth = new Room(&(interactPlain), "Earth", RoomDialogues::earth, EARTH_PIC);
    mars = new Room(&(interactPlain), "Mars", RoomDialogues::mars, MARS_PIC);
    jupiter = new Room(&(interactPlain), "Jupiter", RoomDialogues::jupiter, JUPITER_PIC);
    saturn = new Room(&(interactPlain), "Saturn", RoomDialogues::saturn, SATURN_PIC);
    pluto = new Room(&(interactPlain), "Pluto", RoomDialogues::pluto, PLUTO_PIC);
    
    *mercury + rock;
    *earth + stylus;

    // Setting exits for each room
    //             (N, E, S, W)
    mercury->setExits(venus, earth, NULL, mars);
    venus->setExits(uranus, neptune, mercury, saturn);
    earth->setExits(saturn, mars, NULL, jupiter);
    mars->setExits(neptune, pluto, NULL, mercury);
    jupiter->setExits(NULL, earth, NULL, NULL);
    saturn->setExits(NULL, venus, earth, NULL);
    uranus->setExits(NULL, NULL, venus, NULL);
    neptune->setExits(NULL, NULL, mars, venus);
    pluto->setExits(NULL, mars, NULL, NULL);

    allRooms.push_back(solar_system);
    allRooms.push_back(mercury);
    allRooms.push_back(venus);
    allRooms.push_back(earth);
    allRooms.push_back(mars);
    allRooms.push_back(jupiter);
    allRooms.push_back(saturn);
    allRooms.push_back(uranus);
    allRooms.push_back(neptune);
    allRooms.push_back(pluto);

    // Start off at this room.
    currentRoom = mercury;

    return allRooms;
}

void deleteAllRooms(){

}

/**
 * Given a command, process (that is: execute) the command.
 */
string ZorkUL::processCommand(Command& command, MainWindow *window) {
    string output = "";
    // If we're in a Wordle game, treat the input as a Wordle attempt
    if(WordleMachine::getWordleStatus() == WordleMachine::WORDLE_PROGRESS){
        output += WordleMachine::evaluateInput(command.getCommandWord());

        if(WordleMachine::getWordleStatus() == WordleMachine::WORDLE_PROGRESS){
            return output;
        }
        // If it's a success, give the reward of that particular room
        else if(WordleMachine::getWordleStatus() == WordleMachine::WORDLE_SUCCESS){
            output += ZorkUL::giveReward();
        }
    }

    string commandWord = command.getCommandWord();

//    if (commandWord.compare("info") == 0){
    if (compareIgnoreCase(commandWord, "info")){
        output += printHelp();
        //return printHelp();
    }
    else if (compareIgnoreCase(commandWord, "map"))
    {
        output += "Map to be implemented soon.\n";
        //return "Map to be implemented soon.\n";
    }

    else if (compareIgnoreCase(commandWord, "go")){
        if(compareIgnoreCase(currentRoom->getName(), "space")){
            return "You are all spaced out. Stay where"
                   " you are.";
        }

        try{
            // If the goRoom command is successful, return the room's long description.
            // Otherwise, throw an error message.
            if(goRoom(command)){
                ZorkUL::updateRoom(currentRoom, window);
                if((currentRoom->getTypeOfRoom() & Room::WORDLE) == Room::WORDLE){
                    output += OtherDialogues::welcomeWordle + "\n";
                }

                output += currentRoom->getLongDescription();
                //return currentRoom->longDescription();

            }
            else{
                throw NoRoomError();
            }
        }
        catch(NoRoomError& errorMessage){
            output += errorMessage.what();
            //return errorMessage.what();
        }
    }

    else if (compareIgnoreCase(commandWord, "use")){
        if(!command.hasSecondWord()){
            output += "What are you using at all?!";
            //return "What are you using at all?!";
        }
        else{
            int location = findItemIndex(command.getSecondWord());

            if(location == -1){
                output += "Item is not in your inventory.";
                //return "Item is not in your inventory.";
            }
            else{
                Item* itemToBeUsed = itemsInInventory.at(location);
                if((itemToBeUsed->getTypeOfItem() & Item::LUCK) == Item::LUCK){
                    Raffle* raffle = (Raffle*) itemToBeUsed;
                    output += raffle->useFunc(raffle);
                }
                else{
                    output += itemToBeUsed->useFunc(itemToBeUsed);
                }
            }
            //return "Using this item...";
            }
    }

    else if (compareIgnoreCase(commandWord, "take"))
    {
        if (!command.hasSecondWord()) {
            output += "Are ya trying to take something with ya?!\n";

            //            return "Are ya trying to take something with ya?!\n";
        }
        else{
            output += "You're trying to take " + command.getSecondWord() + "\n";
            int location = currentRoom->isItemInRoom(command.getSecondWord());
            if (location < 0 ){
                output += "Item is not in this room at this specific time.\n";
                //return output;
            }
            else{
                output += "My god! This item is in the room!\n";
                output += "Index number: " + to_string(location) + "\n";


                // Adding to player's inventory and removing from the room
                itemsInInventory.push_back(currentRoom->itemsInRoom.at(location));
                currentRoom->itemsInRoom.erase(currentRoom->itemsInRoom.begin() + location);
                output += currentRoom->getLongDescription();
                //return output;
            }

        }

    }

    else if (compareIgnoreCase(commandWord, "put"))
    {
        if (!command.hasSecondWord()) {
            //cout << "Are ya trying to take something with ya?!"<< endl;
            output += "What are you putting on?!\n";

            //            return "What are you putting on?!\n";
        }
        else{
            output += "You're trying to put " + command.getSecondWord() + "\n";
            int location = findItemIndex(command.getSecondWord());


            if(location == -1){
                output += "Item is not in inventory, sorry.";
            }
            else{
                // Adding to player's inventory and removing from the room
                currentRoom->itemsInRoom.push_back(itemsInInventory.at(location));
                itemsInInventory.erase(itemsInInventory.begin() + location);

                output += currentRoom->getLongDescription();
            }
            //return output;

        }
    }

    else if(compareIgnoreCase(commandWord, "interact")){
        if((currentRoom->getTypeOfRoom() & Room::GOAL) == Room::GOAL){
            GoalRoom* room = (GoalRoom*) currentRoom;
            output += room->interactFunc(room);
        }
        else{
            output += currentRoom->interactFunc(currentRoom);

        }


    }

    else if(compareIgnoreCase(commandWord, "check")){

        if(!command.hasSecondWord()){
            output += "What are ya checkin?!\n";

            //            return "What are ya checkin?!\n";
        }
        else{
            string secondWord = command.getSecondWord();
            if(compareIgnoreCase(secondWord, "inventory")|| compareIgnoreCase(secondWord, "inv")){
                // Print out inventory here
                output += printAllItems();

                //                return printAllItems();
            }
            else if(compareIgnoreCase(secondWord, "room") || compareIgnoreCase(secondWord, "area")){
                output += currentRoom->getLongDescription();
            }
        }
    }

    else if (compareIgnoreCase(commandWord, "quit")) {
        if (command.hasSecondWord())
            output += "If you're looking to quit, type 'quit' in the input console or click the 'quit' button.";

        //            return "If you're looking to quit, type 'quit' in the input console or click the 'quit' button";
        else{
            ZorkUL::deleteAll();
        }

        exit(0); /**signal to quit*/
    }

    if((currentRoom->roomType & RoomProperties::GOAL) == RoomProperties::GOAL){
        GoalRoom* goalRoom = (GoalRoom*) currentRoom;
        output += goalRoom->checkIfGoalCompleted(goalRoom);
    }

    return output;
}
/** COMMANDS **/
string ZorkUL::printHelp() {
    string output = "";
    output += "Valid inputs are: ";
    output += ZorkUL::parser->commandsInString();
    return output;
}

string ZorkUL::printAllItems(){
    string output = "";

    if(itemsInInventory.size() <= 0){
        output += "Theres no items!\n";
        return output;
    }

    output += "These are the items you have at the moment: ";

    for(int i = 0; i < (int) itemsInInventory.size() - 1; i++){
        output += itemsInInventory.at(i)->getShortDescription();
        output += ", ";
    }
    output += itemsInInventory.at(itemsInInventory.size() - 1)->getShortDescription();
//    for(Item* item : itemsInInventory){

//    }
    output += "\n";
    return output;
}

bool ZorkUL::goRoom(Command command) {
    if (!command.hasSecondWord()) {
        //cout << "incomplete input"<< endl;
        return false;
    }

    string direction = command.getSecondWord();

    Room* previousRoom = currentRoom;

    if(compareIgnoreCase(direction, "space") && canGoToSpace){
        Room *space = new Room(&(InteractFunctions::interactParadise), "Space", RoomDialogues::space, Constants::SPACE);
        space->setExits(NULL, NULL, NULL, NULL);
        allRooms.push_back(space);

        currentRoom = space;
        return true;
    }

    // Going back to the previous room
    if(compareIgnoreCase(direction, "back")){
        Room* nextRoom = recentRooms.pop();
        if(nextRoom != NULL){
            currentRoom = nextRoom;
            return true;
        }
        else{
            return false;
        }

    }
    else{
        // Try to leave current room.
        //Room* nextRoom = currentRoom->nextRoom(direction);
        Room* nextRoom = currentRoom->nextRoom(stringToLower(direction));

        if (nextRoom == NULL){
            //cout << "underdefined input"<< endl;
            return false;}
        else {
            currentRoom = nextRoom;
            recentRooms.push(previousRoom);
            //cout << currentRoom->longDescription() << endl;
            return true;
        }

    }
}

// Update the background
void ZorkUL::updateRoom(Room *room, MainWindow *window){

    currentRoom = room;
    window->updateBackground(room->getBackgroundPath());

    if((room->getTypeOfRoom() & Room::WORDLE) == Room::WORDLE){
        WordleMachine::initialiseWordleMachine();
        WordleMachine::startWordleGame();
    }
}

// Finding the index of an item in the Zork inventory
int ZorkUL::findItemIndex(const string& item){
    int sizeItems = (int)(ZorkUL::itemsInInventory.size());

    if (ZorkUL::itemsInInventory.size() < 1) {
        return -1;
    }

    else if (ZorkUL::itemsInInventory.size() > 0) {
        for (int i = 0; i < sizeItems; i++) {
            // compare item with short description
            if (compareIgnoreCase(item, ZorkUL::itemsInInventory[i]->getShortDescription())) {
                return i;
            }
        }
    }

    return -1;
}

// Give reward of a particular room
string ZorkUL::giveReward(){
    string output = "";

    // Check to see if the current room is a Goal Room
    if((currentRoom->roomType & RoomProperties::GOAL) == RoomProperties::GOAL){
        GoalRoom* goalRoom = dynamic_cast<GoalRoom*>(currentRoom);

        // Checking to see if the goal has already been completed or not
        if(goalRoom->getGoalStatus() == false){
            // Casting to RewardRoom so that certain features can be accessed.
            RewardRoom* rewardRoom = dynamic_cast<RewardRoom*>(currentRoom);

            switch(rewardRoom->rewardType){
            case RewardRoom::CASH: {
                int rewardCash = rewardRoom->giveCashReward();
                output += "Congrats! You have got a bitta cash" + to_string(rewardCash) + "!\n";
                ZorkUL::changeCash(rewardCash);
                break;
            }

            case RewardRoom::ITEM : {
                ZorkUL::itemsInInventory.push_back(rewardRoom->giveItemReward());
                output += "You have received: " + ZorkUL::itemsInInventory.at(ZorkUL::itemsInInventory.size() - 1)->getShortDescription() + ".\n";
                break;
            }
            case RewardRoom::CLUE : {
                output += "Clue: " + rewardRoom->giveClueReward() + "\n";
                break;
            }
            case RewardRoom::NONE : break;
            default : break;
            }
        }
        goalRoom->setGoalStatus(true);
    }
    else{
        output += "No good! I guess this room isn't a goal room.\n";
    }



    return output;
}

void ZorkUL::changeCash(int cashAmount){
    ZorkUL::cash += cashAmount;
}

int ZorkUL::getCash(){
    return ZorkUL::cash;
}

int ZorkUL::getGoalCash(){
    return ZorkUL::goalCash;
}

Room *ZorkUL::getCurrentRoom(){
    return ZorkUL::currentRoom;
}

void ZorkUL::setParser(Parser *parser){
    ZorkUL::parser = parser;
}

inline Parser* ZorkUL::getParser(){
    return ZorkUL::parser;
}

inline vector<Item*> ZorkUL::getAllItems(){
    return ZorkUL::itemsInInventory;
}

void ZorkUL::addItem(Item *item){
    itemsInInventory.push_back(item);
}

void ZorkUL::deleteItemByIndex(int& index){
    if(index < (int)itemsInInventory.size()){
        delete itemsInInventory.at(index);
        itemsInInventory.erase(itemsInInventory.begin() + index);
    }
}

void ZorkUL::deleteItemByName(const string &item){
    int itemIndex = findItemIndex(item);
    deleteItemByIndex(itemIndex);
}

string ZorkUL::stringToLower(string& a){
    string output = a;
    for(int i = 0; i < (int) a.size(); i++){
        output[i] = tolower(output[i]);
    }
    return output;
}

// Returns true if 2 strings are equal - Not case sensitive
bool ZorkUL::compareIgnoreCase(string a, string b){

    if(a.size() != b.size()){
        return false;
    }

    for(int i = 0; i < (int) a.size(); i++){
        if(tolower(a[i]) != tolower(b[i])){
            return false;
        }
    }

    return true;
}

void ZorkUL::enableSpace(){
    canGoToSpace = true;
}
