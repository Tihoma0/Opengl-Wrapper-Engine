#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "../opengl/GLTrack.h"
#include "ShaderCache.h"


const char* to_string(ShaderStage stage) {
    switch (stage) {
        case ShaderStage::Vertex:     return "Vertex";
        case ShaderStage::Fragment:   return "Fragment";
        case ShaderStage::Geometry:   return "Geometry";
        case ShaderStage::Compute:    return "Compute";
        case ShaderStage::TessControl: return "TessControl";
        case ShaderStage::TessEval:   return "TessEval";
        default:                      return "Unknown";
    }
}
int hash(ShaderStage stage) {
    return static_cast<int>(stage);
}


std::shared_ptr<Shader> Shader::create() {
    struct EnableMakeShared : public Shader {
        EnableMakeShared() : Shader() {}
    };
    return std::make_shared<EnableMakeShared>();
}


std::shared_ptr<Shader> Shader::add_src(ShaderStage stage, const std::string& source) {
    m_sources[stage] = source;
    if (m_file_sources.contains(stage))
        m_file_sources.erase(stage);
    return shared_from_this();
}

 std::shared_ptr<Shader> Shader::add_file_src(ShaderStage stage, const std::string& filename) {
    m_file_sources[stage] = filename;
    return shared_from_this();
}

std::shared_ptr<Shader> Shader::compile() {
    if (is_compiled || m_id) {
        std::cerr << "Shader is already compiled" << std::endl;
        return shared_from_this();
    }
    for (const auto& [stage, filename] : m_file_sources) {
        m_sources[stage] = loadShaderSource(filename);
    }
    uint64_t key = 0;

    for (int i = 0; i < m_sources.size(); i++) {
        auto it = m_sources.find(static_cast<ShaderStage>(i));
        if (it == m_sources.end()) continue;

        uint64_t h = std::hash<std::string>{}(it->second);

        key ^= h + 0x9e3779b9 + (key << 6) + (key >> 2);
    }


    CacheLookupResult result = GLContext::current_context->shader_cache.create_or_get_program(key);
    m_id = result.program;
    if (!result.newly_created) {
        is_compiled = true;
        return shared_from_this();
    }
    for (const auto& [stage, src] : m_sources) {
        GLuint shader = glCreateShader(to_gl_enum(stage));
        const char* c = src.c_str();

        glShaderSource(shader, 1, &c, nullptr);
        glCompileShader(shader);

        if (!check_success(shader, stage)) {
            glDeleteShader(shader);
            std::cerr << "Failed to compile shader";
            return shared_from_this();
        }
        glAttachShader(m_id, shader);
        m_shader_ids.push_back(shader);
    }

    glLinkProgram(m_id);

    if (!check_link()) {
        std::cerr << "Failed to link shader";
        return shared_from_this();
    }

    for (GLuint s : m_shader_ids)
        glDeleteShader(s);
    m_shader_ids.clear();
    is_compiled = true;
    return shared_from_this();
}

void Shader::use() const {
    if (!is_compiled) {
        std::cout << "Warning: shader not compiled!";
    }
    use_program(m_id);
}

GLuint Shader::id() const {
    return m_id;
}

GLint Shader::get_uniform_location(const std::string &uniform_name) {
    auto location = uniform_locations.find(uniform_name);
    if (location != uniform_locations.end())
        return location->second;
    GLint loc = glGetUniformLocation(m_id, uniform_name.c_str());
    uniform_locations[uniform_name] = loc;
    return loc;
}

GLenum Shader::to_gl_enum(ShaderStage stage) {
    switch (stage) {
        case ShaderStage::Vertex:       return GL_VERTEX_SHADER;
        case ShaderStage::Fragment:     return GL_FRAGMENT_SHADER;
        case ShaderStage::Geometry:     return GL_GEOMETRY_SHADER;
        case ShaderStage::Compute:      return GL_COMPUTE_SHADER;
        case ShaderStage::TessControl:  return GL_TESS_CONTROL_SHADER;
        case ShaderStage::TessEval:     return GL_TESS_EVALUATION_SHADER;
    }
    return 0;
}

std::string Shader::loadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool Shader::check_success(unsigned int shader, ShaderStage stage) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compile Error (" << to_string(stage) << ")\n" << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::check_link() const {
    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(m_id, 1024, nullptr, log);
        std::cerr << "Shader link error:\n" << log<< std::endl << " endlog" << std::endl;
        return false;
    }
    return true;
}
