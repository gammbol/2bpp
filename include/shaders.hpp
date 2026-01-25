#ifndef SHADERS_2BPP_H
#define SHADERS_2BPP_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

enum SHADERS_STATES {
  NOS,
  LOADED,
  COMPILED,
  LINKED
};

class shaders {
  std::string vs, fs;

  SHADERS_STATES state;

public:
  shaders(const char *vs_path, const char *fs_path);

  void compile_shaders();
  void link_program();
};

#endif