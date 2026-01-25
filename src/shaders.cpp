#include <shaders.hpp>

shaders::shaders(const char *vs_path, const char *fs_path) {
  state = NOS;

  std::ifstream vfs;
  std::ifstream ffs;

  vfs.exceptions (std::ifstream::failbit | std::ifstream::badbit);
  ffs.exceptions (std::ifstream::failbit | std::ifstream::badbit);

  try {
    vfs.open(vs_path);
    ffs.open(fs_path);

    std::stringstream vsStream, fsStream;

    vsStream << vfs.rdbuf();
    fsStream << ffs.rdbuf();

    vfs.close();
    ffs.close();

    vs = vsStream.str();
    fs = fsStream.str();
  } catch (std::istream::failure e) {
    std::cerr << "Couldn't process the shaders!" << std::endl;
    std::cerr << e.what() << std::endl;
    return;
  }

  state = LOADED;
}