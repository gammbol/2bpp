#include <main.hpp>

int main(int argc, char *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2bpp test", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "The program couldn't create a window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Couldn't initialize GLAD!" << std::endl;
    return -1;
  }

  // vertices
  float vertices[] = {
    -.5f, -.5f, .0f,    .0f, 1.0f, .0f,   .0f, .0f,    // left bottom
    .5f, -.5f, .0f,     1.0f, .0f, .0f,   1.0f, .0f,     // right bottom
    .0f, 0.5f, .0f,     .0f, .0f, 1.0f,   .5f, 1.0f      // center top
  };
  unsigned int indices[] = {
    0, 1, 2,
  };

  // element buffer objects
  unsigned int EBO;
  glGenBuffers(1, &EBO);

  // binding ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // vao
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  // vbo
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // GL_ARRAY_BUFFER - vertex buffer object type
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // creating texture buffer (i think...)
  unsigned int texture;
  glGenTextures(1, &texture);

  // binding texture
  glBindTexture(GL_TEXTURE_2D, texture);

  // setting texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // textures loading
  int width, height, nrChannels;
  unsigned char* data = stbi_load("img/wall.jpg", &width, &height, &nrChannels, 0);

  if (data) {
    // generating texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cerr << "stbi_load: Failed to load texture!" << std::endl;
  }
  // freeing image byte vector
  stbi_image_free(data);

  // shaders
  shaders shd("shaders/vertex.vs", "shaders/fragment.fs");
  shd.compile_shaders();
  shd.link_program();

  // setting resize callback
  glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shd.use_program();

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void frambuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}