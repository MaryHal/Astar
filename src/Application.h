#ifndef _Application_h_
#define _Application_h_

#include <stdbool.h>

typedef struct
{
    const char* title;
    unsigned int width;
    unsigned int height;

    bool fullscreen;
    bool running;
} Application;

bool InitApp(Application* app);
void DeinitApp();

void InitOpenGL(Application* app);

#endif

