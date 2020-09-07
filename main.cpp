#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>

class App {
private:
  GLFWwindow* window;
  const std::uint64_t WINDOW_WIDTH = 800;
  const std::uint64_t WINDOW_HEIGHT = 600;
public:
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glad_glViewport(0, 0, width, height);
  }

  bool init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return false;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return true;
  }

  void run() {
    while (!glfwWindowShouldClose(window)) {
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  void terminate() {
    glfwTerminate();
  }

};

int main() {
  auto app = App();

  app.init();
  app.run();
  app.terminate();
}
