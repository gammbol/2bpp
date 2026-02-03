#include <main.hpp>

int main(int argc, char *argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // stbi image vertical flip
  stbi_set_flip_vertically_on_load(true);

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
    // vertex           // texture
    -.5f, -.5f, .0f,    .0f, .0f,     // bottom left
    -.5f, .5f, .0f,     .0f, 1.0f,    // top left
    .5f, .5f, .0f,      1.0f, 1.0f,   // top right
    .5f, -.5f, .0f,     1.0f, .0f,    // bottom right
  };
  unsigned int indices[] = {
    0, 1, 2,    // top triangle
    0, 2, 3     // bottom triangle
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // creating texture buffer (i think...)
  unsigned int texture[2];
  glGenTextures(2, texture);

  // binding texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);

  // setting texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // textures loading
  int width, height, nrChannels;
  unsigned char* data = stbi_load("img/container.jpg", &width, &height, &nrChannels, 0);

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

  // creating second texture
  data = stbi_load("img/awesomeface.png", &width, &height, &nrChannels, 0);

  if (data) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    // using GL_RGBA because the resource in png format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cerr << "stbi_load: Failed to load texture!" << std::endl;
  }
  stbi_image_free(data);

  // shaders
  shaders shd("shaders/vertex.vs", "shaders/fragment.fs");
  shd.compile_shaders();
  shd.link_program();

  // using the shader program in order to bind textures
  // because it doesnt want to bind it at all if you add another one
  shd.use_program();
  shd.setInt("tex0", 0);
  shd.setInt("tex1", 1);

  // setting resize callback
  glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shd.use_program();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
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