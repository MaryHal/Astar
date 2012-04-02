#include "Map.h"
#include "Application.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glfw.h>

void InitMap(Map* map, Application* app, int tilewidth, int tileheight)
{
    map->tilewidth = tilewidth;
    map->tileheight = tileheight;

    map->width  = app->width / map->tilewidth;
    map->height = app->height / map->tileheight;

    map->tiles = malloc(sizeof(Tile*) * map->width);
    for (int i = 0; i < map->width; ++i)
    {
        map->tiles[i] = malloc(sizeof(Tile) * map->height);
        memset(map->tiles[i], 0, sizeof(Tile) * map->height);
    }
}

void DeinitMap(Map* map)
{
    for (int i = 0; i < map->width; ++i)
        free(map->tiles[i]);
    free(map->tiles);
}

bool IsValidTile(Map* map, int x, int y)
{
    return !(x < 0 || x >= map->width ||
             y < 0 || y >= map->height);
}

void SetTile(Map* map, int x, int y, Tile value)
{
    if (!IsValidTile(map, x, y))
        return;
    map->tiles[x][y] = value;
}

Tile GetTile(Map* map, int x, int y)
{
    if (!IsValidTile(map, x, y))
        return Invalid;
    return map->tiles[x][y];
}

void DrawMap(Map* map)
{
    for (int x = 0; x < map->width; ++x)
    {
        for (int y = 0; y < map->height; ++y)
        {
            if (map->tiles[x][y] == 1)
            {
                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                int x2 = x * map->tilewidth;
                int y2 = y * map->tileheight;
                glBegin(GL_QUADS);
                    glVertex2f(x2, y2);
                    glVertex2f(x2 + map->tilewidth, y2);
                    glVertex2f(x2 + map->tilewidth, y2 + map->tileheight);
                    glVertex2f(x2, y2 + map->tileheight);
                glEnd();
            }
        }
    }
};

void DumpMap(Map* map)
{
    for (int x = 0; x < map->width; ++x)
    {
        for (int y = 0; y < map->height; ++y)
        {
            if (map->tiles[x][y])
            {
                printf("(%d, %d) = %d\n", x, y, map->tiles[x][y]);
            }
        }
    }
}

