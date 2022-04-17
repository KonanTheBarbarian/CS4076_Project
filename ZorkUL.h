#ifndef ZORKUL_H_
#define ZORKUL_H_

#include "Parser.h"
#include "Room.h"
#include "WordleMachine.h"
#include "mainwindow.h"

using namespace std;

struct Quantities{
    unsigned int KeysPresent : 4;
};

class ZorkUL {
private:
    static Room *currentRoom;
    static vector<Room*> allRooms;
    static bool canGoToSpace;

    void printWelcome();
    static string printHelp();
    static bool goRoom(Command command);
    void createItems();
    static string printAllItems();
    static int cash;
    const static int goalCash = 100;

    static Quantities *allQuantities;
    static Parser *parser;
    static Stack<Room*> recentRooms;

    static vector<Item*> itemsInInventory;
    static bool keysPresent[5];


    static int findItemIndex(const string& item);
    static void deleteItemByIndex(int& index);
    static void deleteAll();
    static vector<Item*> getAllItems();
    static string stringToLower(string& a);

public:


    ZorkUL();

    static void updateRoom(Room *room, MainWindow *window);
    static string processCommand(Command& command, MainWindow *window);
    static Room *getCurrentRoom();
    static vector<Room*> createRooms();
    static void setParser(Parser* parser);
    static string giveReward();
    static void deleteItemByName(const string& item);
    static void changeCash(int cashAmount);
    static int getCash();
    static void setAllRooms(vector<Room*> rooms);
    static vector<Room*> getAllRooms();
    static Parser* getParser();
    static int getGoalCash();
    static void enableSpace();
    static bool compareIgnoreCase(string a, string b);
    static void addItem(Item*);
};

#endif ZORKUL_H_
