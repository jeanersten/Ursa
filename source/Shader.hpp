#pragma once

#include <glad/gl.h>

#include <filesystem>
#include <string>

class Shader
{
private:
  GLuint m_id;

  void compileShader(GLuint shader_id, const std::filesystem::path& shader_file_path);
  void linkShaders(GLuint program_id, GLuint vertex_shader_id, GLuint fragment_shader_id);

public:
  Shader(const std::filesystem::path& vertex_shader_file_path, const std::filesystem::path& fragment_shader_file_path);
  ~Shader();

  void use() const;
  void unuse() const;
  void setBool(const std::string& uniform_name, bool value) const;
  void setInt(const std::string& uniform_name, int value) const;
  void setFloat(const std::string& uniform_name, float value) const;
};

