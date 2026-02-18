#ifndef MAIN_2BPP_H
#define MAIN_2BPP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include <shaders.hpp>

// image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void frambuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

#endif