#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>

#include "DungeonRenderer.h"
#include "Location.h"
#include "Level.h"
#include "Actions.h"
#include "Pawn.h"


using namespace std;

void ForceRedraw();

class GameLogic
{
public:
    GameLogic(DungeonRenderer& dungeonRenderer_, Dungeon& dungeon_, PlayerPawn& pawn_)
        : dungeonRenderer(dungeonRenderer_),
        dungeon(dungeon_),
        pawn(pawn_)
    {

    }

    void start()
    {
        Key levelKey = dungeon.getKey();
        Vector2 exitPositon;

        dungeonRenderer.initializeLevel(dungeon);
        dungeonRenderer.placeAThing(pawn.getPosition(), pawn.symbol);
        dungeonRenderer.placeAThing(levelKey.getPosition(), levelKey.symbol);

        std::vector<std::vector<char>> map = dungeonRenderer.getMap();

        pawn.onInstantiate(map, levelKey.getPosition(), exitPositon);
    }

    bool update()
    {
        dungeonRenderer.drawMap();

        Vector2 playerPos = pawn.getPosition();

        //Extract value.
        Action pawnAction = pawn.getAction(this->dungeonRenderer);

        if(pawnAction == Action::Quit)
        {
            return true;
        }

        switch(pawnAction) //Perform movement actions.
        {
        case Action::MoveNorth:
            playerPos = dungeonRenderer.moveAThing(NORTH, playerPos);
            break;
        case Action::MoveSouth:
            playerPos = dungeonRenderer.moveAThing(SOUTH, playerPos);
            break;
        case Action::MoveEast:
            playerPos = dungeonRenderer.moveAThing(EAST, playerPos);
            break;
        case Action::MoveWest:
            playerPos = dungeonRenderer.moveAThing(WEST, playerPos);
            break;
        case MoveNorthEast:
            playerPos = dungeonRenderer.moveAThing(NORTHEAST, playerPos);
            break;
        case MoveSouthEast:
            playerPos = dungeonRenderer.moveAThing(SOUTHEAST, playerPos);
            break;
        case MoveNorthWest:
            playerPos = dungeonRenderer.moveAThing(NORTHWEST, playerPos);
            break;
        case MoveSouthWest:
            playerPos = dungeonRenderer.moveAThing(SOUTHWEST, playerPos);
            break;
        case Action::TakeKey:
            {
                if(dungeonRenderer.checkForKey(playerPos))
                {
                    pawn.collectedKey();
                }
            }
        default:
            break;
        }

        pawn.setPosition(playerPos);

        if(pawn.possesesKey())
        {
            dungeonRenderer.unlockDoor();
        }

        return pawn.possesesKey() && dungeonRenderer.pawnHasExited(playerPos);
    }

    void exit()
    {
        
    }

    void instansiateObject()
    {
        
    }

    void movePlayer()
    {
        
    }
private:
    Dungeon& dungeon;
    DungeonRenderer& dungeonRenderer;

    PlayerPawn& pawn;
};

void clear()
{
    //system("CLS");
}

Dungeon getSampleLevel()
{
    Key key(Vector2(45, 20), 'k');

    Dungeon dungeon(Vector2(55, 30), key, Vector2(11, 11));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(10, 5), Vector2(20, 15)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(40, 10), Vector2(50, 24)));
    dungeon.addFeat(DungeonFeat(HHALL, Vector2(20, 10), Vector2(40, 15)));
    dungeon.addFeat(DungeonFeat(EXIT, Vector2(14, 5), Vector2(0, 0)));

    return dungeon;
}

Dungeon getZigZagLevel()
{
    Key key(Vector2(85, 19), 'k');

    Dungeon dungeon(Vector2(88, 22), key, Vector2(2));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2( 1, 1), Vector2(3)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 1), Vector2(85, 3)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 9), Vector2(85, 11)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(2, 9), Vector2(4, 11)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(2, 15), Vector2(5, 20)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 18), Vector2(86, 20)));
    dungeon.addFeat(DungeonFeat(HHALL, Vector2(3, 1), Vector2(80, 3)));
    dungeon.addFeat(DungeonFeat(HHALL, Vector2(3, 9), Vector2(80, 11)));
    dungeon.addFeat(DungeonFeat(HHALL, Vector2(5, 18), Vector2(80, 20)));
    dungeon.addFeat(DungeonFeat(VHALL, Vector2(81, 3), Vector2(85, 9)));
    dungeon.addFeat(DungeonFeat(VHALL, Vector2(2, 11), Vector2(5, 15)));
    dungeon.addFeat(DungeonFeat(EXIT, Vector2(8, 1), Vector2(0)));
    return dungeon;
}

Dungeon getMazeLevel()
{


    Key key(Vector2(3, 13), 'k');

    Dungeon dungeon(Vector2(100, 30 ), key, Vector2(2));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(1, 1), Vector2(3, 5)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 1), Vector2(85, 5)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 8), Vector2(85, 12)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(1, 8), Vector2(3, 12)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 30), Vector2(85, 20)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(1, 12), Vector2(5, 20)));
    dungeon.addFeat(DungeonFeat(ROOM, Vector2(80, 26), Vector2(85, 28)));

    dungeon.addFeat(DungeonFeat(HHALL, Vector2(1, 1), Vector2(80, 5)));
    dungeon.addFeat(DungeonFeat(HHALL, Vector2(3, 8), Vector2(80, 12)));

    dungeon.addFeat(DungeonFeat(VHALL, Vector2(80, 5), Vector2(85, 8)));
    dungeon.addFeat(DungeonFeat(VHALL, Vector2(80, 12), Vector2(85, 26)));
    dungeon.addFeat(DungeonFeat(HHALL, Vector2(3, 15), Vector2(80, 20)));

    dungeon.addFeat(DungeonFeat(EXIT, Vector2(3, 8), Vector2(0)));

    return dungeon;
}

void humanPlayGame(Dungeon& dungeon)
{
    DungeonRenderer dungeonRenderer;

    //Get playerPawn
    Human player( dungeon.playerStart);

    dungeonRenderer.setPawnSpeed(1);

    //Initialize game logic
    GameLogic game(dungeonRenderer, dungeon, player);
    game.start();

    while(true)
    {
        clear();
        bool hasExited = game.update();

        if(hasExited)
        {
            game.exit();
            break;
        }
    }
}

void botPlayGame(Dungeon& dungeon)
{
    DungeonRenderer dungeonRenderer;

    //Get playerPawn
    FsmBot player(dungeon.playerStart);

    dungeonRenderer.setPawnSpeed(1);

    //Initialize game logic
    GameLogic game(dungeonRenderer, dungeon, player);

    game.start();

    while (true)
    {
        clear();
        bool hasExited = game.update();

        if (hasExited)
        {
            game.exit();
            break;
        }
    }
}

void aboutScreen()
{
    clear();
    cout << "About ---\n"
        << "This is Patrick and Corey's term project\n"
        << "for artificial intelligence and heuristic\n"
        << "programming, fall semester 2019. This game\n"
        << "Allows you or a fsm built (finite state machine\n"
        << "robot to visually explore a dungeon.\n"
        << "The goal of the game is to escape the dungeon."
        << endl;

    cout << "\n" << endl;
    system("pause");
}

Dungeon selectDungeon(Dungeon &currentDungeon)
{
    clear();
    cout << "Select Dungeon --\n"
        << "1.) Sample Dungeon (selected by default)\n"
        << "2.) ZigZag Dungeon\n"
        << "3.) Maze Dungeon\n"
        << "Press 'q' to go back.\n"
        << endl;

    char selection = getchar();
    cin.ignore();
    switch (selection)
    {
    case '1':
        return getSampleLevel();
    case '2':
        return getZigZagLevel();
    case '3':
        return getMazeLevel();
    case 'q':
        return currentDungeon;
    }
}

void mainMenu()
{
    Dungeon selectedLevel = getSampleLevel();

    while (true)
    {
        clear();
        ForceRedraw();
        system("cls");

        cout << "Adventure Buddy - The depths of Xak-Tsaroth\n";
        cout << "0.) Enter the dungeon yourself\n";
        cout << "1.) Send the robot into the dungeon\n";
        cout << "2.) Switch dungeons\n";
        cout << "3.) About\n";
        cout << "4.) Quit\n";
        cout << "Make a selection and press 'ENTER': ";

        char selection = getchar();
        cin.ignore();
        switch (selection)
        {
        case '0':
            humanPlayGame(selectedLevel);
            break;
        case '1':
            botPlayGame(selectedLevel);
            break;
        case '2':
            selectedLevel = selectDungeon(selectedLevel);
            break;
        case '3':
            aboutScreen();
            break;
        case '4':
            return;
        default:
            continue;
        }
    }
}

int main()
{
    std::cout.setf(std::ios::unitbuf);

    mainMenu();
    printf("Goodbye!");
}