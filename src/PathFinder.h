#ifndef _PathFinder_h_
#define _PathFinder_h_

#include "Map.h"
#include "PriorityQueue.h"

static const int NoList = 0;
static const int OnOpenList = 1;
static const int OnClosedList = 2;

typedef struct PathNode
{
    int x;
    int y;
    struct PathNode* parent;
} PathNode;

typedef struct Node
{
    int x;
    int y;
    int f;
    int g;
    int h;
    int open;
    struct Node* parent;
} Node;

typedef struct
{
    int startx, starty;
    int endx, endy;

    Map* map;
    BinHeap* openList;
    Node** nodeMap;

    PathNode* path;
    //int pathSize;
} PathFinder;

void InitPathFinder(PathFinder* pathFinder, Map* map);
void DeinitPathFinder(PathFinder* pathFinder);

void SetOrigin(PathFinder* pf, int x, int y);
void SetDestination(PathFinder* pf, int x, int y);
void FindPath(PathFinder* pf);

void DeletePath(PathFinder* pf);
void SavePath(PathFinder* pf, Node* n);

void AddOpen(PathFinder* pf, int x, int y, Node* parent);
int ComputeG(PathFinder* pf, int x, int y, Node* parent);
int ComputeH(PathFinder* pf, int x, int y);
int ComputeF(PathFinder* pf, int x, int y);

void DrawPath(PathFinder* pf);

#endif

