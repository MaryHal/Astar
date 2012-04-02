#ifndef _Map_h_
#define _Map_h_

#include "Application.h"

typedef enum
{
    None,
    Wall,
    Invalid
} Tile;

typedef struct
{
    int tilewidth;
    int tileheight;

    int width;
    int height;

    Tile** tiles;
} Map;

void InitMap(Map* map, Application* app, int tilewidth, int tileheight);
void DeinitMap(Map* map);

bool IsValidTile(Map* map, int x, int y);
void SetTile(Map* map, int x, int y, Tile value);
Tile GetTile(Map* map, int x, int y);
void DrawMap(Map* map);

void DumpMap(Map* map);

#endif

