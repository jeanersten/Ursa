#include "Log.hpp"
#include "Shader.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>

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
  GLFWwindow* window = glfwCreateWindow(640, 640, "Ursa Engine!", NULL, NULL);
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
    // positions          // colors            // tex coords
    -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  2.0f,    // 0 top left
    -0.6f, -0.6f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,    // 1 bottom left
     0.6f,  0.6f,  0.0f,  1.0f,  1.0f,  0.0f,  2.0f,  2.0f,    // 2 top right
     0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  1.0f,  2.0f,  0.0f     // 3 bottom right
  };

  GLuint indices[] = {
    0, 1, 3,
    3, 2, 0
  };

  stbi_set_flip_vertically_on_load(1);
  GLint width[2] = {0, 0};
  GLint height[2] = {0, 0};
  GLint channels[2] = {0, 0};
  GLubyte* data[2] = {stbi_load("resource/images/Container.jpg", &width[0], &height[0], &channels[0], 3),
                      stbi_load("resource/images/Tag.png", &width[1], &height[1], &channels[1], 4)};
  if (data[0] == NULL || data[1] == NULL)
  {
    LOG_ERROR("Failed to load texutre from image!");
  }

  GLuint vao = 0;
  GLuint vbo = 0;
  GLuint ebo = 0;
  GLuint textures[2] = {0, 0};

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenTextures(2, textures);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width[0], height[0], 0, GL_RGB, GL_UNSIGNED_BYTE, data[0]);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width[1], height[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, data[1]);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data[0]);
  stbi_image_free(data[1]);

  Shader shader_sample("resource/shaders/VertexSample.vert", "resource/shaders/FragmentSample.frag");
  shader_sample.use();
  shader_sample.setInt("tex0", 0);
  shader_sample.setInt("tex1", 1);
  shader_sample.unuse();

  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glClearDepth(1.0f);

  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_sample.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
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
