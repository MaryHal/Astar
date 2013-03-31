#include <GL/glfw.h>

#include "Application.h"
#include "Map.h"
#include "PathFinder.h"

int main(int argc, const char *argv[])
{
    Application app = { "A* Test", 640, 480, false, false };
    InitApp(&app);

    Map map;
    InitMap(&map, &app, 20, 20);

    PathFinder pathFinder;
    InitPathFinder(&pathFinder, &map, false);
    SetOrigin(&pathFinder, 0, 0);

    // Initial Path is to itself.
    SetDestination(&pathFinder, 0, 0);
    FindPath(&pathFinder);

    while (app.running)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        int x;
        int y;
        glfwGetMousePos(&x, &y);
        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
        {
            // Allow the user to create/delete walls.
            if (glfwGetKey(GLFW_KEY_LCTRL))
                SetTile(&map, x / map.tilewidth, y / map.tileheight, None);
            else
                SetTile(&map, x / map.tilewidth, y / map.tileheight, Wall);
        }
        if (x/map.tilewidth  != pathFinder.endx ||
            y/map.tileheight != pathFinder.endy)
        {
            SetDestination(&pathFinder, x / map.tilewidth, y / map.tileheight);
            FindPath(&pathFinder);
        }

        DrawMap(&map);
        DrawPath(&pathFinder);

        // Draw mouse position
        //glfwGetMousePos(&x, &y);
        x -= x % map.tilewidth;
        y -= y % map.tileheight;
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + map.tilewidth, y);
            glVertex2f(x + map.tilewidth, y + map.tileheight);
            glVertex2f(x, y + map.tileheight);
        glEnd();

        glfwSwapBuffers();

        app.running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
    }

    //DumpMap(&map);

    DeinitPathFinder(&pathFinder);
    DeinitMap(&map);
    DeinitApp();
    return 0;
}
