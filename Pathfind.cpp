
#include "Pathfind.h"
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct node
{
    node()
        :
        position(-1, -1),
        visited(false),
        open(false),
        prev(nullptr)
    {

    }

    Vector2 position;

    bool visited;
    bool open;
    node* prev;
};

vector<Vector2> pathByBFS(vector<vector<int>>& pathmap, Vector2 start, Vector2 end)
{
    vector<vector<node>> nodes;
    node* startNode = nullptr;
    node* endNode = nullptr;

    for (int i = 0; i < 100; ++i)
    {
        vector<node> row;
        for (int j = 0; j < 100; ++j)
        {
            row.push_back(node());
        }
        nodes.push_back(row);
    }

    for (int y = 0; y < 100; ++y)
    {
        for (int x = 0; x < 100; ++x)
        {
            nodes[y][x].position = Vector2(x, y);
            nodes[y][x].prev = nullptr;

            if (start.x == x && start.y == y)
            {
                startNode = &nodes[y][x];
            }
            else if (end.x == x && end.y == y)
            {
                endNode = &nodes[y][x];
            }

            if (pathmap[y][x] == 1)
            {
                nodes[y][x].open = true;
            }
            else
            {
                nodes[y][x].open = false;
            }
        }
    }

    queue<node*> processing;

    processing.push(startNode);

    int directions[4][2] =
    {
        { -1, 0 },
        { 1, 0 },
        { 0, -1},
        { 0, 1 }
    };

    node* realEndNode = nullptr;

    while (!processing.empty())
    {
        node* visitingNode = processing.front();
        processing.pop();

        if(!visitingNode->open || visitingNode->visited)
        {
            continue;
        }

        visitingNode->visited = true;

        if(visitingNode == endNode)
        {
            realEndNode = visitingNode;
        }

        for (int i = 0; i < 4; ++i)
        {
            auto newPosition = Vector2(visitingNode->position.x + directions[i][0], visitingNode->position.y + directions[i][1]);
            auto& node = nodes[newPosition.y][newPosition.x];

            if (node.prev == nullptr && !node.visited)
            {
                node.prev = visitingNode;
                processing.push(&node);
            }
        }

    }

    vector<Vector2> path;

    while (realEndNode != nullptr)
    {
        path.push_back(realEndNode->position);
        realEndNode = realEndNode->prev;
    }

    reverse(path.begin(), path.end());

    return path;
}