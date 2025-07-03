#include "Log.hpp"
#include "Shader.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <string>

void glfw_errorCallback(int error_code, const char* description);
void glfw_framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main()
{
  Log::setup(LOG_LEVEL_TRACE);

  glfwSetErrorCallback(glfw_errorCallback);
  if (!glfwInit())
  {
    LOG_CRITICAL("Failed to initialize GLFW!");
    return -1;
  }

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Ursa Engine!", NULL, NULL);
  if (window == NULL)
  {
    LOG_CRITICAL("Failed to create window!");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, glfw_framebufferSizeCallback);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
  {
    LOG_CRITICAL("Failed to load OpenGL!");
    glfwTerminate();
    return -1;
  }
  LOG_INFO("OpenGL Version  | {}", (const char*)glGetString(GL_VERSION));

  GLfloat vertices[] = {
     0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, // 0 top mid
    -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f, // 1 bottom left
     0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f  // 2 bottom right
  };

  GLuint indices[] = {
    0, 1, 2,
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glClearDepth(1.0f);

  Shader shader_sample("resource/VertexSample.vert", "resource/FragmentSample.frag");

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = glfwGetTime();
    GLfloat red_val = (sin(time) / 2.0f) + 0.5f;

    shader_sample.use();
    shader_sample.setFloat("red", red_val);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    shader_sample.unuse();

    glfwSwapBuffers(window);
    glfwWaitEventsTimeout(1.0f / 60.0f);
  }

  glfwTerminate();
  return 0;
}

void glfw_errorCallback(int error_code, const char* description)
{
  LOG_ERROR("[GLFW-{}] {}", error_code, description);
}

void glfw_framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}
