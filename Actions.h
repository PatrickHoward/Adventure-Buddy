#pragma once

enum Action
{
    Quit,
    MoveNorth,
    MoveSouth,
    MoveEast,
    MoveWest,
    MoveNorthEast,
    MoveSouthEast,
    MoveNorthWest,
    MoveSouthWest,
    TakeKey,
    None,
};

enum botStates
{
    FindKey,
    GetKey,
    FindExit,
    Nil,
};