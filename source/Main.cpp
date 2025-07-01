#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <string>

#define TRACE(msg, ...) spdlog::trace(msg, __VA_ARGS__)
#define DEBUG(msg, ...) spdlog::debug(msg, __VA_ARGS__)
#define INFO(msg, ...) spdlog::info(msg, __VA_ARGS__)
#define WARN(msg, ...) spdlog::warn(msg, __VA_ARGS__)
#define ERROR(msg, ...) spdlog::error(msg, __VA_ARGS__)
#define CRITICAL(msg, ...) spdlog::critical(msg, __VA_ARGS__)

void glfw_errorCallback(int error_code, const char* description);
void glfw_framebufferSizeCallback(GLFWwindow* window, int width, int height);

void compileShader(GLuint shader);
void linkShaders(GLuint program, GLuint vertex_shader, GLuint fragment_shader);

int main()
{
  spdlog::set_level(spdlog::level::trace);
  spdlog::set_pattern("[ %T ]--[%^ %L %$]: %v");

  glfwSetErrorCallback(glfw_errorCallback);
  if (!glfwInit())
  {
    CRITICAL("Failed to initialize GLFW!");
    return -1;
  }

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Ursa Engine!", NULL, NULL);
  if (window == NULL)
  {
    CRITICAL("Failed to create window!");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, glfw_framebufferSizeCallback);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
  {
    CRITICAL("Failed to load OpenGL!");
    glfwTerminate();
    return -1;
  }
  INFO("OpenGL Vendor   | {}", (const char*)glGetString(GL_VENDOR));
  INFO("OpenGL Renderer | {}", (const char*)glGetString(GL_RENDERER));
  INFO("OpenGL Version  | {}", (const char*)glGetString(GL_VERSION));

  const GLchar* vshader_source =
    "#version 330\n"
    "layout (location = 0) in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);\n"
    "}\0"
  ;

  const GLchar* fshader_source =
    "#version 330\n"
    "out vec4 col;\n"
    "void main()\n"
    "{\n"
    "col = vec4(1.0f, 0.5f, 1.0f, 1.0f);\n"
    "}\0"
  ;

  GLuint vshader = 0;
  GLuint fshader = 0;
  vshader = glCreateShader(GL_VERTEX_SHADER);
  fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vshader, 1, &vshader_source, NULL);
  glShaderSource(fshader, 1, &fshader_source, NULL);
  compileShader(vshader);
  compileShader(fshader);

  GLuint program = 0;
  program = glCreateProgram();
  linkShaders(program, vshader, fshader);

  glDeleteShader(vshader);
  glDeleteShader(fshader);

  GLfloat vertices[] = {
    -0.5f, -0.5f,  0.0f, // 0 bottom left
    -0.5f,  0.5f,  0.0f, // 1 top left
     0.5f, -0.5f,  0.0f, // 2 bottom right
     0.5f,  0.5f,  0.0f, // 3 top right
  };

  GLuint indices[] = {
    0, 1, 2,
    3, 2, 1
  };

  GLuint vao = 0;
  GLuint vbo = 0;
  GLuint ebo = 0;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glClearDepth(1.0f);

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(program);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glUseProgram(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwWaitEventsTimeout(1.0f / 60.0f);
  }

  glfwTerminate();
  return 0;
}

void glfw_errorCallback(int error_code, const char* description)
{
  ERROR("(GLFW)[{}] {}", error_code, description);
}

void glfw_framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void compileShader(GLuint shader)
{
  glCompileShader(shader);

  int compile_status = 0;
  char info_log[512] = "";
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE)
  {
    glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
    std::string log = info_log;
    log.erase(0, log.find_first_not_of(" \t\n\r"));
    log.erase(log.find_last_not_of(" \t\n\r") + 1);
    ERROR("(OpenGL)[Shader Compilation] {}", log);
  }
}

void linkShaders(GLuint program, GLuint vertex_shader, GLuint fragment_shader)
{
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  int link_status = 0;
  char info_log[512] = "";
  glGetProgramiv(program, GL_LINK_STATUS, &link_status);
  if (link_status == GL_FALSE)
  {
    glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
    std::string log = info_log;
    log.erase(0, log.find_first_not_of(" \t\n\r"));
    log.erase(log.find_last_not_of(" \t\n\r") + 1);
    ERROR("(OpenGL)[Shader Linking] {}", log);
  }

  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);
}
