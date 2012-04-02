#include "Application.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GL/glfw.h>

bool InitApp(Application* app)
{
    glfwInit();

    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    int ok = glfwOpenWindow(app->width, app->height,
            8, 8, 8, 8,
            0, 0,
            app->fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);
    if (!ok)
    {
        app->running = false;
        return false;
    }

    glfwSetWindowTitle(app->title);
    glfwSwapInterval(1);

    /*
    glfwSetKeyCallback(keyInput);
    glfwSetMousePosCallback(mouseMoveInput);
    glfwSetMouseButtonCallback(mouseInput);
    */

    glewInit();
    InitOpenGL(app);

    app->running = true;
    return true;
}

void DeinitApp()
{
    glfwTerminate();
}

void InitOpenGL(Application* app)
{
    // Set Default ClearColor
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, app->width, app->height, 0, -1, 1);

    // Initialize modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set alpha blending and disable lighting
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_LINE_SMOOTH);

    // If there was any errors
    if (glGetError() != GL_NO_ERROR)
        printf("Could not initialize OpenGL.");
}

