#include <stdio.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "PriorityQueue.h"
#include "Application.h"
#include "Map.h"
#include "PathFinder.h"

/*
int minCompare(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}
*/

int main(int argc, const char *argv[])
{
    /*
    PriorityQueue* q = CreateQueue(minCompare, 10);
    int array[9];
    for (int i = 9; i > 0; i--)
    {
        array[i] = i;
        QueueAdd(q, &array[i]);
    }
    printf("\n");
    while (GetQueueSize(q) > 0)
        printf("%d ", *(int*)QueueRemove(q));
    DeleteQueue(q);
    */

    Application app = { "A* Test", 640, 480, false, false };
    InitApp(&app);

    Map map;
    InitMap(&map, &app, 16, 16);

    PathFinder pathFinder;
    InitPathFinder(&pathFinder, &map);
    SetOrigin(&pathFinder, 0, 0);

    while (app.running)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        int x;
        int y;
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
        {
            glfwGetMousePos(&x, &y);
            SetTile(&map, x / map.tilewidth, y / map.tileheight, Wall);
        }

        glfwGetMousePos(&x, &y);
        SetDestination(&pathFinder, x / map.tilewidth, y / map.tileheight);
        FindPath(&pathFinder);

        DrawMap(&map);
        DrawPath(&pathFinder);

        glfwGetMousePos(&x, &y);
        x -= x % map.tilewidth;
        y -= y % map.tileheight;;
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + map.tilewidth, y);
            glVertex2f(x + map.tilewidth, y + map.tileheight);
            glVertex2f(x, y + map.tileheight);
        glEnd();

        glfwSwapBuffers();

        app.running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
        //glfwSleep(1.0 / 60.0 - fps.getFrameTime());
    }

    //DumpMap(&map);

    DeinitPathFinder(&pathFinder);
    DeinitMap(&map);
    DeinitApp();

    return 0;
}

