#include "Log.hpp"
#include "Shader.hpp"

#include <sstream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

Shader::Shader(const std::filesystem::path& vertex_shader_file_path, const std::filesystem::path& fragment_shader_file_path)
  : m_id(0)
{
  std::string vertex_shader_source;
  std::string fragment_shader_source;
  std::ifstream vertex_shader_file;
  std::ifstream fragment_shader_file;

  vertex_shader_file.exceptions(std::fstream::failbit | std::fstream::badbit);
  fragment_shader_file.exceptions(std::fstream::failbit | std::fstream::badbit);
  try
  {
    vertex_shader_file.open(vertex_shader_file_path);
    fragment_shader_file.open(fragment_shader_file_path);

    std::stringstream vertex_shader_stream;
    std::stringstream fragment_shader_stream;
    vertex_shader_stream << vertex_shader_file.rdbuf();
    fragment_shader_stream << fragment_shader_file.rdbuf();

    vertex_shader_file.close();
    fragment_shader_file.close();

    vertex_shader_source = vertex_shader_stream.str();
    fragment_shader_source = fragment_shader_stream.str();
  }
  catch (std::ifstream::failure error)
  {
    LOG_ERROR("Shader not successfully read!");
  }

  const char* vertex_shader_source_cstr = vertex_shader_source.c_str();
  const char* fragment_shader_source_cstr = fragment_shader_source.c_str();

  GLuint vertex_shader_id = 0;
  GLuint fragment_shader_id = 0;
  vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vertex_shader_id, 1, &vertex_shader_source_cstr, NULL);
  glShaderSource(fragment_shader_id, 1, &fragment_shader_source_cstr, NULL);
  compileShader(vertex_shader_id, vertex_shader_file_path);
  compileShader(fragment_shader_id, fragment_shader_file_path);

  m_id = glCreateProgram();
  linkShaders(m_id, vertex_shader_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
}

Shader::~Shader()
{}

void Shader::compileShader(GLuint shader_id, const std::filesystem::path& shader_file_path)
{
  glCompileShader(shader_id);

  int compile_status = 0;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
  if (compile_status == GL_FALSE)
  {
    GLint error_log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &error_log_length);

    std::vector<GLchar> error_log(error_log_length);
    std::string shader_file_path_str = std::filesystem::absolute(shader_file_path).generic_string();
    glGetShaderInfoLog(shader_id, error_log_length, &error_log_length, &error_log[0]);
    LOG_ERROR("[OpenGL] Failed to compile shader '{}'\n" 
              "-- -------------^------------------------------------- -- \n\n\n"
              "{}                                                        \n"
              "-- --------------------------------------------------- -- ",
              shader_file_path_str, error_log.data());
  }
}

void Shader::linkShaders(GLuint program_id, GLuint vertex_shader_id, GLuint fragment_shader_id)
{
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  int link_status = 0;
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
  if (link_status == GL_FALSE)
  {
    GLint error_log_length = 0;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &error_log_length);

    std::vector<GLchar> error_log(error_log_length);
    glGetProgramInfoLog(program_id, error_log_length, &error_log_length, &error_log[0]);
    LOG_ERROR("[OpenGL] Failed to link shader!\n" 
              "-- -------------^------------------------------------- -- \n\n\n"
              "{}                                                        \n\n"
              "-- --------------------------------------------------- -- ",
              error_log.data());
  }

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);
}

void Shader::use() const
{
  glUseProgram(m_id);
}

void Shader::unuse() const
{
  glUseProgram(0);
}

void Shader::setBool(const std::string& uniform_name, bool value) const
{
  glUniform1i(glGetUniformLocation(m_id, uniform_name.c_str()), (int)value);
}

void Shader::setInt(const std::string& uniform_name, int value) const
{
  glUniform1i(glGetUniformLocation(m_id, uniform_name.c_str()), value);
}

void Shader::setFloat(const std::string& uniform_name, float value) const
{
  glUniform1f(glGetUniformLocation(m_id, uniform_name.c_str()), value);
}
