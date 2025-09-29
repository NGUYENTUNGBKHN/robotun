#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <model.h>
#include <ctrl.h>
#include <ode.h>
/* OPENGL header */
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
double pi = 3.14159265358979323846; // Simplified pi value

// Function to initialize GLFW and the window
GLFWwindow* startGLFW()
{
    if (!glfwInit())
    {
        fprintf(stderr, "GLFW failed to initialize glfw, panic\n");
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "demo gui 02",
        nullptr,
        nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // IMPORTANT: Set the current window as the OpenGL rendering target (Context)
    glfwMakeContextCurrent(window);
    glOrtho(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT, -1.0, 1.0);
    return window;
}


void drawCircle(float x, float y, float r)
{
    int res = 100; // Resolution: number of triangles used

    glBegin(GL_TRIANGLE_FAN);

    // Center point of the circle
    glVertex2d(x, y);

    // Loop to calculate and draw vertices around the center
    for (int i = 0; i <= res; i++)
    {
        double angle = 2.0 * pi * (static_cast<double>(i) / res);

        float _x = x + static_cast<float>(std::cos(angle) * r);
        float _y = y + static_cast<float>(std::sin(angle) * r);

        glVertex2d(_x, _y);
    }

    // IMPORTANT: End the drawing command
    glEnd();
}


int main(int, char**)
{
    GLFWwindow* window = startGLFW();

    std::vector<float> position = { 400.0f, 300.0f };


    while (!glfwWindowShouldClose(window))
    {
        // 1. Clear the screen
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color
        glClear(GL_COLOR_BUFFER_BIT);


        // 2. Draw the circle
        drawCircle(position[0], position[1], 50.0f);
        


        // 3. Display the result
        glfwSwapBuffers(window);
        // 4. Process events
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}