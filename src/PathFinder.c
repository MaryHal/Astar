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

    //pathFinder->path = malloc(sizeof(PathNode) * map->height * map->width);
    //pathFinder->pathSize = 0;
    pathFinder->path = NULL;
}

void DeinitPathFinder(PathFinder* pathFinder)
{
    DeleteQueue(pathFinder->openList);
    for (int i = 0; i < pathFinder->map->width; ++i)
        free(pathFinder->nodeMap[i]);
    free(pathFinder->nodeMap);

    DeletePath(pathFinder);
}

void SetOrigin(PathFinder* pf, int x, int y)
{
    if (!IsValidTile(pf->map, x, y))
        return;
    pf->startx = x;
    pf->starty = y;
}

void SetDestination(PathFinder* pf, int x, int y)
{
    if (!IsValidTile(pf->map, x, y))
        return;
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
        //printf("%d ", GetQueueSize(pf->openList));
        // a) Look for the lowest F cost square on the open list.
        Node* current = (Node*)QueueRemove(pf->openList);

        x = current->x;
        y = current->y;

        // b) Switch it to the closed list. 
        current->open = OnClosedList;

        // c) For each of the 8 squares adjacent to this current square, attempt to add it
        //    to the open list.
        // AddOpen checks for adjacent-validity and whether the node is a wall or not.
        AddOpen(pf, x - 0, y - 1, current);
        AddOpen(pf, x - 0, y + 1, current);
        AddOpen(pf, x - 1, y - 0, current);
        AddOpen(pf, x + 1, y - 0, current);

        /*
        AddOpen(pf, x - 1, y - 1, current);
        AddOpen(pf, x + 1, y - 1, current);
        AddOpen(pf, x - 1, y + 1, current);
        AddOpen(pf, x + 1, y + 1, current);
        */

        // Are we done?
        if (x == pf->endx && y == pf->endy &&
            pf->nodeMap[pf->endx][pf->endy].open == OnClosedList)
        {
            // Step 3: Save Path
            DeletePath(pf);
            SavePath(pf, current);

            /*
            PathNode* p = pf->path;
            if (p == NULL)
                printf(" WHAT?!?!?!?!?!");
            while (p)
            {
                printf("(%p) %d, %d", p, p->x, p->y);
                p = p->parent;
            }
            */
            break;
        }
    }
    printf("Pathfinding done\n");
}

void DeletePath(PathFinder* pf)
{
    while (pf->path)
    {
        free(pf->path);
        pf->path = pf->path->parent;
    }
    pf->path = NULL;
}

void SavePath(PathFinder* pf, Node* n)
{
    pf->path = malloc(sizeof(PathNode));
    pf->path->x = n->x;
    pf->path->y = n->y;
    pf->path->parent = NULL;

    PathNode* p = pf->path;
    n = n->parent;
    while (n)
    {
        p->parent = malloc(sizeof(PathNode));
        p = p->parent;
        p->x = n->x;
        p->y = n->y;
        p->parent = NULL;

        // printf("(%p) %d, %d\n", p, p->x, p->y);

        n = n->parent;
    }
    /* Bad Debugging
    printf("====================\n");
    p = pf->path;
    while (p)
    {
        printf("(%p) %d, %d (%p)\n", p, p->x, p->y, p->parent);
        p = p->parent;
    }
    */
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
        pf->nodeMap[x][y].parent = parent;

        pf->nodeMap[x][y].g = ComputeG(pf, x, y, parent);
        pf->nodeMap[x][y].h = ComputeH(pf, x, y);
        pf->nodeMap[x][y].f = ComputeF(pf, x, y);

        pf->nodeMap[x][y].open = OnOpenList;
        QueueAdd(pf->openList, &pf->nodeMap[x][y]);
    }
    else if (pf->nodeMap[x][y].open == OnOpenList)
    {
        int g = ComputeG(pf, x, y, parent);

        // If this path we're currently searching is better, use it instead
        if (pf->nodeMap[x][y].g > g)
        {
            pf->nodeMap[x][y].parent = parent;

            pf->nodeMap[x][y].g = ComputeG(pf, x, y, parent);
            pf->nodeMap[x][y].h = ComputeH(pf, x, y);
            pf->nodeMap[x][y].f = ComputeF(pf, x, y);

            int index = QueueSearch(pf->openList, &pf->nodeMap[x][y]);
            PercolateUp(pf->openList, index, &pf->nodeMap[x][y]);
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
        int dx = abs(parent->x - x);
        int dy = abs(parent->y - y);

        if (dx + dy > 1)
            return parent->g + 14;
        else
            return parent->g + 10;
    }
}

int ComputeH(PathFinder* pf, int x, int y)
{
    //return 10 * (abs(pf->endx - x) + abs(pf->endy - y)); // Manhatten method

    int xDistance = abs(x-pf->endx);
    int yDistance = abs(y-pf->endy);
    if (xDistance > yDistance)
        return 14*yDistance + 10*(xDistance-yDistance);
    else
        return 14*xDistance + 10*(yDistance-xDistance);
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

    PathNode* current = pf->path;
    while (current)
    {
        int x = current->x * pf->map->tilewidth;
        int y = current->y * pf->map->tileheight;

        glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + pf->map->tilewidth, y);
            glVertex2f(x + pf->map->tilewidth, y + pf->map->tileheight);
            glVertex2f(x, y + pf->map->tileheight);
        glEnd();

        current = current->parent;
    }
}


