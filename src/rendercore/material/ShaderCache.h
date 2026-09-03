#ifndef SHADERCACHE_H
#define SHADERCACHE_H
#include <unordered_map>

#include "glad/glad.h"

struct CacheLookupResult
{
    GLuint program;
    bool newly_created;
};


class ShaderCache {
public:
    ShaderCache() = default;
    ~ShaderCache();

    CacheLookupResult create_or_get_program(uint64_t key);

    void enable();
    void disable();

    void delete_all_programs();

private:
    std::unordered_map<uint64_t, GLuint> m_cache;
    bool m_enabled = true;
};
#endif //SHADERCACHE_H
