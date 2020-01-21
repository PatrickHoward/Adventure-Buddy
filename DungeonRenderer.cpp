#include <vector>
#include <iostream>
#include <string>

#include "DungeonRenderer.h"
#include "Location.h"

#include <Windows.h>
#include <consoleapi2.h>
#include <handleapi.h>
#include <wincon.h>
#include <winbase.h>
#include <atlstr.h>

using namespace std;

char oldMap[101][101];

char statusMessage[100];
char oldStatusMessage[100];

void SetStatusMessage(const char* str)
{
    for (int i = 0; i < 100; ++i)
    {
        statusMessage[i] = ' ';
        oldStatusMessage[i] = ' ';
    }

    for (int i = 0; i < strlen(str); ++i)
    {
        statusMessage[i] = str[i];
    }
}

void WriteCharacter(int x, int y, char c)
{
    COORD pos = { x, y };
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    const char cstr[1] = { c };

    CString str(cstr);
    CStringW strw(str);
    LPCWSTR ptr = strw;

    DWORD len = 1;
    DWORD dwBytesWritten = 0;
    WriteConsoleOutputCharacter(hConsole_c, ptr, len, pos, &dwBytesWritten);
}

bool first = true;

void ForceRedraw()
{
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 100; ++j)
        {
            oldMap[i][j] = 0;
        }
    }
}

void DungeonRenderer::drawMap()
{
    cout.setf(ios::unitbuf);
    vector<char> printBuffer;

    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[i].size(); ++j)
        {
            if (map[i][j] != oldMap[i][j])
            {
                WriteCharacter(j, i, map[i][j]);
                oldMap[i][j] = map[i][j];
            }
        }
        //printBuffer.push_back('\n');
    }

    if (first)
    {


        for (int i = 0; i < 100; ++i)
        {
            oldStatusMessage[i] = ' ';
            statusMessage[i] = ' ';
            WriteCharacter(i, 30, ' ');
        }
    }

    for (int i = 0; i < 100; ++i)
    {
        if (statusMessage[i] != oldStatusMessage[i] || first)
        {
            WriteCharacter(i, 30, statusMessage[i]);
            oldStatusMessage[i] = statusMessage[i];
        }
    }

    first = false;
}

void DungeonRenderer::createEmptyMap(Vector2 size)
{


    int height = size.y;
    int width = size.x;

    for (int i = 0; i < height; ++i)
    {
        if (i == 0 || i == height - 1)
        {
            map.push_back(setHorizontalBorder(width));
        }
        else
        {
            vector<char> row;
            for (int j = 0; j < width; ++j)
            {
                if (j == 0 || j == width - 1)
                {
                    row.push_back('|');
                }
                else
                {
                    row.push_back('-');
                }
            }
            map.push_back(row);
        }
    }
}

bool DungeonRenderer::pawnHasExited(Vector2 position)
{
    --position.y;

    //This horrifying mess checks two spaces to the right and the two spaces to the right above of the exit position.
    if (map[position.y + 1][position.x] == 'X' || map[position.y + 1][position.x] == 'I' || map[position.y][position.x] == 'X' || map[position.y][position.x] == 'I')
    {
        Vector2 mapCenter(map[0].size() / 2, map.size() / 2);

        const char winMessage[8] = { 'Y', 'O', 'U', ' ', 'W', 'I', 'N', '!' };
        for (int i = 0; i < 8; ++i)
        {
            map[mapCenter.y][mapCenter.x + i] = winMessage[i];
        }

        ForceRedraw();
        drawMap();
        system("pause");
        system("cls");
        WriteCharacter(0, 0, ' ');
        ForceRedraw();

        return true;
    }

    return false;
}

bool DungeonRenderer::checkForKey(Vector2 position)
{
    //Check around player position.
    for (int i = -1; i <= 1; ++i)
    {
        for (int j = -1; j <= 1; ++j)
        {
            if (abs(i) != abs(j) && map[position.y + j][position.x + i] == 'k')
            {
                map[position.y + j][position.x + i] = ' ';

                return true;
            }
        }
    }

    return false;
}

void DungeonRenderer::initializeLevel(Dungeon dungeon)
{
    createEmptyMap(dungeon.getSize());

    auto level = dungeon.getFeats();
    for (int i = 0; i < level.size(); ++i)
    {
        put(level[i]);
    }
}

void DungeonRenderer::unlockDoor()
{
    map[exitPosition.y - 1][exitPosition.x] = 'E';
    map[exitPosition.y - 1][exitPosition.x + 1] = 'X';
    map[exitPosition.y - 1][exitPosition.x + 2] = 'I';
    map[exitPosition.y - 1][exitPosition.x + 3] = 'T';

    map[exitPosition.y][exitPosition.x + 1] = ' ';
    map[exitPosition.y][exitPosition.x + 2] = ' ';
}

void DungeonRenderer::placeAThing(Vector2 position, char thing)
{
    int x = position.x;
    int y = position.y;

    map[y][x] = thing;
}

vector<char> DungeonRenderer::setHorizontalBorder(int width)
{
    vector<char> border;

    for (int i = 0; i < width; ++i)
    {
        border.push_back('=');
    }

    return border;
}

Vector2 DungeonRenderer::attemptMove(Vector2 oldLocation, Vector2 newLocation)
{
    char thingAhead = map[newLocation.y][newLocation.x];

    if (thingAhead == '=' || thingAhead == '-' || thingAhead == '|' || thingAhead == 'E' || thingAhead == 'T' || thingAhead == '[' || thingAhead == ']' || thingAhead == 'k')
    {
        return oldLocation;
    }

    return newLocation;
}

Vector2 DungeonRenderer::moveAThing(MoveDirection direction, Vector2 position)
{
    char thing = map[position.y][position.x];

    map[position.y][position.x] = ' ';

    Vector2 newPosition = position;
    switch (direction)
    {
    case MoveDirection::NORTH:
        newPosition = attemptMove(position, Vector2(position.x, position.y - pawnSpeed));
        break;
    case MoveDirection::SOUTH:
        newPosition = attemptMove(position, Vector2(position.x, position.y + pawnSpeed));
        break;
    case MoveDirection::EAST:
        newPosition = attemptMove(position, Vector2(position.x + 1, position.y));
        break;
    case MoveDirection::WEST:
        newPosition = attemptMove(position, Vector2(position.x - 1, position.y));
        break;
    }

    map[newPosition.y][newPosition.x] = thing;

    return newPosition;
}

void DungeonRenderer::placeRoom(Vector2 topLeft, Vector2 bottomRight)
{
    for (int i = topLeft.y; i <= bottomRight.y; ++i)
    {
        if (i == topLeft.y || i == bottomRight.y)
        {
            vector<char> row = setHorizontalBorder(bottomRight.x);
            for (int j = topLeft.x; j <= bottomRight.x; ++j)
            {
                map[i][j] = '=';
            }
        }
        else
        {
            for (int j = topLeft.x; j <= bottomRight.x; ++j)
            {
                if (j == topLeft.x || j == bottomRight.x)
                {
                    map[i][j] = '|';
                }
                else
                {
                    map[i][j] = ' ';
                }

            }
        }
    }
}

void DungeonRenderer::placeHorizontalHallway(Vector2 topLeft, Vector2 bottomRight)
{
    for (int i = topLeft.y; i <= bottomRight.y; ++i)
    {
        if (i == topLeft.y || i == bottomRight.y)
        {
            for (int j = topLeft.x; j <= bottomRight.x; ++j)
            {
                map[i][j] = '=';
            }
        }
        else
        {
            for (int j = topLeft.x; j <= bottomRight.x; ++j)
            {
                map[i][j] = ' ';
            }
        }
    }
}

void DungeonRenderer::placeVerticalHallway(Vector2 topLeft, Vector2 bottomRight)
{
    for (int i = topLeft.y; i <= bottomRight.y; ++i)
    {
        if (i == topLeft.y || i == bottomRight.y)
        {
            for (int j = topLeft.x; j <= bottomRight.x; ++j)
            {
                if (map[i][j - 1] == '-')
                {
                    map[i][j] = '|';
                }
                else if (map[i][j + 1] == '-')
                {
                    map[i][j] = '|';
                }
                else
                {
                    map[i][j] = ' ';
                }
            }
        }
        else
        {
            for (int j = topLeft.x; j <= bottomRight.x; ++j)
            {
                if (j == topLeft.x || j == bottomRight.x)
                {
                    map[i][j] = '|';
                }
                else
                {
                    map[i][j] = ' ';
                }
            }
        }
    }
}

void DungeonRenderer::placeExit(Vector2 position)
{
    exitPosition = position;

    const char exitObj[4] = { '[', 'X', 'X', ']' };
    Vector2 exitCursor = position;
    for (int i = 0; i < 4; ++i)
    {
        map[exitCursor.y][exitCursor.x + i] = exitObj[i];
    }
}


void DungeonRenderer::put(DungeonFeat feat)
{
    switch (feat.element)
    {
    case ROOM:
        placeRoom(feat.topLeft, feat.bottomRight);
        break;
    case HHALL:
        placeHorizontalHallway(feat.topLeft, feat.bottomRight);
        break;
    case VHALL:
        placeVerticalHallway(feat.topLeft, feat.bottomRight);
        break;
    case EXIT:
        placeExit(feat.topLeft);
        break;
    }
}

