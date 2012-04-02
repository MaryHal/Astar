#include "PathFinder.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glfw.h>

int nodeMinCompare(const void* a, const void* b)
{
    return ((Node*)a)->f - ((Node*)b)->f;
}

void InitPathFinder(PathFinder* pathFinder, Map* map)
{
    pathFinder->map = map;

    pathFinder->openList = CreateQueue(nodeMinCompare, map->width * map->height);

    pathFinder->nodeMap = malloc(sizeof(Node*) * map->width);
    for (int i = 0; i < map->width; ++i)
    {
        pathFinder->nodeMap[i] = malloc(sizeof(Node) * map->height);
        memset(pathFinder->nodeMap[i], 0, sizeof(Node) * map->height);
    }

    for (int i = 0; i < map->width; i++)
    {
        for (int e = 0; e < map->height; e++) 
        {
            pathFinder->nodeMap[i][e].x = i;
            pathFinder->nodeMap[i][e].y = e;
            pathFinder->nodeMap[i][e].open = NoList;
        }
    }

    pathFinder->path = malloc(sizeof(PathNode) * map->height * map->width);
    pathFinder->pathSize = 0;
}

void DeinitPathFinder(PathFinder* pathFinder)
{
    DeleteQueue(pathFinder->openList);
    for (int i = 0; i < pathFinder->map->width; ++i)
        free(pathFinder->nodeMap[i]);
    free(pathFinder->nodeMap);

    free(pathFinder->path);
}

void SetOrigin(PathFinder* pf, int x, int y)
{
    pf->startx = x;
    pf->starty = y;
}

void SetDestination(PathFinder* pf, int x, int y)
{
    pf->endx = x;
    pf->endy = y;
}

void FindPath(PathFinder* pf)
{
    //printf("(%d, %d) -> (%d, %d)\t", pf->startx, pf->starty, pf->endx, pf->endy);
    for (int i = 0; i < pf->map->width; i++)
    {
        for (int e = 0; e < pf->map->height; e++) 
        {
            pf->nodeMap[i][e].f = 0;
            pf->nodeMap[i][e].g = 0;
            pf->nodeMap[i][e].h = 0;
            pf->nodeMap[i][e].open = NoList;
            pf->nodeMap[i][e].parent = NULL;
        }
    }
    DeleteQueue(pf->openList);
    pf->openList = CreateQueue(nodeMinCompare, pf->map->width * pf->map->height);

    int x = pf->startx;
    int y = pf->starty;

    // Step 1: Add starting node to the open list.
    AddOpen(pf, x, y, NULL);

    // Step 2: Repeat
    // If there's nothing in the open list anymore or we arrive at the destination, stop
    while (GetQueueSize(pf->openList) > 0)
    {
        // a) Look for the lowest F cost square on the open list.
        Node* current = (Node*)QueueRemove(pf->openList);

        // b) Switch it to the closed list. 
        pf->nodeMap[x][y].open = OnClosedList;

        x = current->x;
        y = current->y;

        // c) For each of the 8 squares adjacent to this current square, attempt to add it
        //    to the open list.
        // AddOpen checks for adjacent-validity and whether the node is a wall or not.
        AddOpen(pf, x - 1, y - 1, current);
        AddOpen(pf, x - 0, y - 1, current);
        AddOpen(pf, x + 1, y - 1, current);
        AddOpen(pf, x - 1, y - 0, current);
        AddOpen(pf, x + 1, y - 0, current);
        AddOpen(pf, x - 1, y + 1, current);
        AddOpen(pf, x - 0, y + 1, current);
        AddOpen(pf, x + 1, y + 1, current);

        // Are we done?
        if (pf->nodeMap[pf->endx][pf->endy].open == OnOpenList)
        {
            // Step 3: Save Path
            int i = 0;
            while (current)
            {
                pf->path[i].x = current->x;
                pf->path[i].y = current->y;
                i++;

                current = current->parent;
            }
            pf->pathSize = i;
            printf("%d ", i);
            break;
        }
    }
    printf("Pathfinding done\n");
}

void AddOpen(PathFinder* pf, int x, int y, Node* parent)
{
    if (x < 0 || x >= pf->map->width)
        return;
    if (y < 0 || y >= pf->map->height)
        return;

    // Ignore if the node isn't walkable
    if (GetTile(pf->map, x, y) == Wall)
        return;

    // Ignore if the node has already been checked before.
    if (pf->nodeMap[x][y].open == OnClosedList)
        return;

    if (pf->nodeMap[x][y].open == NoList)
    {
        pf->nodeMap[x][y].open = OnOpenList;
        QueueAdd(pf->openList, &pf->nodeMap[x][y]);

        pf->nodeMap[x][y].parent = parent;

        pf->nodeMap[x][y].g = ComputeG(pf, x, y, parent);
        pf->nodeMap[x][y].h = ComputeH(pf, x, y);
        pf->nodeMap[x][y].f = ComputeF(pf, x, y);
    }
    else if (pf->nodeMap[x][y].open == OnOpenList)
    {
        int g = ComputeG(pf, x, y, parent);

        // If this path we're currently searching is better, use it instead
        if (pf->nodeMap[x][y].g > g)
        {
            pf->nodeMap[x][y].parent = parent;
            pf->nodeMap[x][y].g = ComputeG(pf, x, y, parent);
            pf->nodeMap[x][y].f = ComputeF(pf, x, y);

            int index = QueueSearch(pf->openList, &pf->nodeMap[x][y]);
            Heapify(pf->openList, index);
        }
    }
}

int ComputeG(PathFinder* pf, int x, int y, Node* parent)
{
    if (!parent)
    {
        return 0;
    }
    else
    {
        int dx = abs(x - parent->x);
        int dy = abs(y - parent->y);

        if (dx + dy > 1)
            return parent->g + 14;
        else
            return parent->g + 10;
    }
}

int ComputeH(PathFinder* pf, int x, int y)
{
    return 10 * (abs(x - pf->endx) + abs(pf->endy - y)); // Manhatten method

    /*
    int xDistance = abs(x-pf->endx);
    int yDistance = abs(y-pf->endy);
    if (xDistance > yDistance)
        return 14*yDistance + 10*(xDistance-yDistance);
    else
        return 14*xDistance + 10*(yDistance-xDistance);
    */
}

int ComputeF(PathFinder* pf, int x, int y)
{
    return pf->nodeMap[x][y].g + pf->nodeMap[x][y].h;
}

void DrawPath(PathFinder* pf)
{
    // If FindPath was not successful, or hasn't been run yet.
    if (!pf->path)
    {
        printf("No Path\n");
        return;
    }

    for (int i = 0; i < pf->pathSize; ++i)
    {
        int x = pf->path[i].x;
        int y = pf->path[i].y;

        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        int x2 = x * pf->map->tilewidth;
        int y2 = y * pf->map->tileheight;
        glBegin(GL_QUADS);
            glVertex2f(x2, y2);
            glVertex2f(x2 + pf->map->tilewidth, y2);
            glVertex2f(x2 + pf->map->tilewidth, y2 + pf->map->tileheight);
            glVertex2f(x2, y2 + pf->map->tileheight);
        glEnd();
    }
}


