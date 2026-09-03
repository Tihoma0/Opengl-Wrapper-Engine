
#include "rendercore/material/ShaderCache.h"

#include <ranges>
#include <unordered_map>

#include "Shader.h"


ShaderCache::~ShaderCache() {
    delete_all_programs();
}

CacheLookupResult ShaderCache::create_or_get_program(const uint64_t key) {
    if (!m_cache.contains(key) || !m_enabled) {
        const GLuint program = glCreateProgram();
        if (m_enabled)
            m_cache[key] = program;
        return { program, true };
    }
    return { m_cache[key], false };
}

void ShaderCache::enable() { m_enabled = true; }
void ShaderCache::disable() { m_enabled = false; }

void ShaderCache::delete_all_programs() {
    for (const auto &program: m_cache | std::views::values) {
        glDeleteProgram(program);
    }
}


