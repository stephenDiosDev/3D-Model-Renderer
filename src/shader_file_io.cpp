#include "shader.hpp"

#include <fstream>
#include <iostream>

namespace opengl {

std::string loadShaderStringFromFile(const std::string &filePath) {
  std::string shaderCode;
  std::ifstream fileStream(filePath);
  if (fileStream.good()) {
    std::string line;
    while (std::getline(fileStream, line)) {
      shaderCode += "\n" + line;
    }
    fileStream.close();

  } else {
    std::cerr << "[Error] could not open shader file " << filePath << "\n";
    return {""};
  }
  return shaderCode;
}

} // namespace openGL
