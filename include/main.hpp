#ifndef MAIN_2BPP_H
#define MAIN_2BPP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <shaders.hpp>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void frambuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

#endif