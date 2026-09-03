#ifndef SHADER_H
#define SHADER_H
#include <memory>
#include <unordered_map>
#include <vector>

#include "glad/glad.h"
#include "rendercore/core/math/ivec/IVec2.h"
#include "rendercore/core/math/ivec/IVec3.h"
#include "rendercore/core/math/ivec/IVec4.h"
#include "rendercore/core/math/mat/Mat3.h"
#include "rendercore/core/math/mat/Mat4.h"


enum class ShaderStage {
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessControl,
    TessEval
};

const char* to_string(ShaderStage stage);
inline int hash(ShaderStage stage);


class Shader : public std::enable_shared_from_this<Shader> {
public:
    static std::shared_ptr<Shader> create();


    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;

    ~Shader() = default;

    std::shared_ptr<Shader> add_src(ShaderStage stage, const std::string& source);

    std::shared_ptr<Shader> add_file_src(ShaderStage stage, const std::string& filename);

    std::shared_ptr<Shader> compile();

    void use() const;

    GLuint id() const;

    GLint get_uniform_location(const std::string &uniform_name);

    // Integers
    void setUniform(const std::string& name, const int v0) {
        glUniform1i(get_uniform_location(name), v0);
    }
    void setUniform(const std::string& name, const int v0, const int v1) {
        glUniform2i(get_uniform_location(name), v0, v1);
    }
    void setUniform(const std::string& name, const int v0, const int v1, const int v2) {
        glUniform3i(get_uniform_location(name), v0, v1, v2);
    }
    void setUniform(const std::string& name, const int v0, const int v1, int v2, const int v3) {
        glUniform4i(get_uniform_location(name), v0, v1, v2, v3);
    }
    // GLuint
    void setUniform(const std::string& name, const GLuint id) {
        glUniform1ui(get_uniform_location(name), id);
    }
    // Floats
    void setUniform(const std::string& name, const float v0) {
        glUniform1f(get_uniform_location(name), v0);
    }
    void setUniform(const std::string& name, const float v0, const float v1) {
        glUniform2f(get_uniform_location(name), v0, v1);
    }
    void setUniform(const std::string& name, const float v0, const float v1, const float v2) {
        glUniform3f(get_uniform_location(name), v0, v1, v2);
    }
    void setUniform(const std::string& name, const float v0, const float v1, const float v2, const float v3) {
        glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
    }
    // Arrays
    void setUniform(const std::string& name, const std::vector<float>& arr) {
        glUniform1fv(get_uniform_location(name), arr.size(), arr.data());
    }
    void setUniform(const std::string& name, const std::vector<int>& arr) {
        glUniform1iv(get_uniform_location(name), arr.size(), arr.data());
    }
    void setUniform2fv(const std::string& name, const GLsizei count, const GLfloat* arr) {
        glUniform2fv(get_uniform_location(name), count, arr);
    }
    void setUniform3fv(const std::string& name, const GLsizei count, const GLfloat* arr) {
        glUniform3fv(get_uniform_location(name), count, arr);
    }
    void setUniform4fv(const std::string& name, const GLsizei count, const GLfloat* arr) {
        glUniform4fv(get_uniform_location(name), count, arr);
    }
    void setUniform2iv(const std::string& name, const GLsizei count, const GLint* arr) {
        glUniform2iv(get_uniform_location(name), count, arr);
    }
    void setUniform3iv(const std::string& name, const GLsizei count, const GLint* arr) {
        glUniform3iv(get_uniform_location(name), count, arr);
    }
    void setUniform4iv(const std::string& name, const GLsizei count, const GLint* arr) {
        glUniform4iv(get_uniform_location(name), count, arr);
    }

    // Matrices
    void setUniform(const std::string& name, const Mat3& m) {
        glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, m.m);
    }
    void setUniform(const std::string& name, const Mat4& m) {
        glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, m.m);
    }
    // Vectors
    void setUniform(const std::string& name, const Vec2& v) {
        glUniform2f(get_uniform_location(name), v.x, v.y);
    }
    void setUniform(const std::string& name, const Vec3& v) {
        glUniform3f(get_uniform_location(name), v.x, v.y, v.z);
    }
    void setUniform(const std::string& name, const Vec4& v) {
        glUniform4f(get_uniform_location(name), v.x, v.y, v.z, v.w);
    }

    void setUniform(const std::string& name, const IVec2& v) {
        glUniform2i(get_uniform_location(name), v.x, v.y);
    }
    void setUniform(const std::string& name, const IVec3& v) {
        glUniform3i(get_uniform_location(name), v.x, v.y, v.z);
    }
    void setUniform(const std::string& name, const IVec4& v) {
        glUniform4i(get_uniform_location(name), v.x, v.y, v.z, v.w);
    }

    void getfloatUniform(const std::string& name, float *out) {
        glGetUniformfv(m_id, get_uniform_location(name), out);
    }

    void getintUniform(const std::string& name, int *out) {
        glGetUniformiv(m_id, get_uniform_location(name), out);
    }

private:
    GLuint m_id = 0;
    bool is_compiled = false;

    Shader() = default;

    std::unordered_map<ShaderStage, std::string> m_sources;
    std::unordered_map<ShaderStage, std::string> m_file_sources;
    std::vector<GLuint> m_shader_ids;
    std::unordered_map<std::string, GLint> uniform_locations;

    static GLenum to_gl_enum(ShaderStage stage);

    static std::string loadShaderSource(const std::string& filepath);

    static bool check_success(unsigned int shader, ShaderStage stage);

    bool check_link() const;


};



#endif //SHADER_H
