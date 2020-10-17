#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>
#include <vector>

class App {
private:
  GLFWwindow* window;
  const std::uint64_t WINDOW_WIDTH = 800;
  const std::uint64_t WINDOW_HEIGHT = 600;
  unsigned int current_vao = 0;
  unsigned int current_vbo = 0;
  unsigned int current_ebo = 0;
  unsigned int current_program = 0;
  std::vector<unsigned int> vao_array = {};
  std::vector<unsigned int> vbo_array = {};
  std::vector<unsigned int> ebo_array = {};
  std::vector<unsigned int> program_array = {};

  unsigned int createAndBindVAO() {
    vao_array.emplace_back(0);
    glGenVertexArrays(1, &vao_array.back());
    glBindVertexArray(vao_array.back());
    return vao_array.back();
  }

  unsigned int createAndBindVBO(std::vector<float> vertices, GLenum usage) {
    vbo_array.emplace_back(0);
    glGenBuffers(1, &vbo_array.back());
    glBindBuffer(GL_ARRAY_BUFFER, vbo_array.back());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), usage);
    return vbo_array.back();
  }

  unsigned int createAndBindEBO(std::vector<unsigned int> indices, GLenum usage) {
    ebo_array.emplace_back(0);
    glGenBuffers(1, &ebo_array.back());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_array.back());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);
    return ebo_array.back();
  }

  unsigned int createAndCompileShader(GLenum shaderType, const GLchar **source) {
    int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, source, nullptr);
    glCompileShader(shader);
    return shader;
  }

  bool checkShaderCompileStatus(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      std::cout << "ERROR: vertex shader compilation failed" << std::endl << infoLog << std::endl;
      return false;
    }
    return true;
  }

  unsigned int setupProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    program_array.emplace_back(0);
    program_array.back() = glCreateProgram();
    glAttachShader(program_array.back(), vertexShader);
    glAttachShader(program_array.back(), fragmentShader);
    glLinkProgram(program_array.back());
    return program_array.back();
  }

  bool checkShaderProgramLinkStatus() {
    int success;
    char infoLog[512];
    glGetProgramiv(current_program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(current_program, 512, nullptr, nullptr);
      std::cout << "ERROR: program linkage failed" << std::endl << infoLog << std::endl;
      return false;
    }
    return true;
  }

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }

public:
  bool init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
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
    const char *vertexShaderSource = "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main() {\n"
      " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\n\0";

    unsigned int vertexShader = createAndCompileShader(GL_VERTEX_SHADER, &vertexShaderSource);
    checkShaderCompileStatus(vertexShader);

    const char *fragmentShaderSource = "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main() {\n"
      "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\n\0";

    unsigned int fragmentShader = createAndCompileShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);
    checkShaderCompileStatus(fragmentShader);

    current_program = setupProgram(vertexShader, fragmentShader);
    checkShaderProgramLinkStatus();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::vector<float> vertices = {
      0.5f, 0.5f, 0.0f, // top right
      0.5f, -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f, 0.0f // top left
    };

    std::vector<unsigned int> indices = {
      0, 1, 3,
      1, 2, 3
    };

    current_vao = createAndBindVAO();
    current_vbo = createAndBindVBO(vertices, GL_STATIC_DRAW);
    current_ebo = createAndBindEBO(indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE

    while (!glfwWindowShouldClose(window)) {
      processInput(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(current_program);
      glBindVertexArray(current_vao);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  void terminate() {
    for (auto &&e : program_array) { glad_glDeleteProgram(e); }
    for (auto &&e : vao_array) { glDeleteVertexArrays(1, &e); }
    for (auto &&e : vbo_array) { glad_glDeleteBuffers(1, &e); }
    for (auto &&e : ebo_array) { glad_glDeleteBuffers(1, &e); }
    glfwTerminate();
  }

};

int main() {
  auto app = App();

  if (app.init()) {
    app.run();
  }
  app.terminate();
}
