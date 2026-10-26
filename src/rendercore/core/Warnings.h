#ifndef WARNINGS_H
#define WARNINGS_H



// #define DEBUG_MODE

#include <glad/glad.h>
#include <iostream>

#define THROW_RUNTIME(msg) \
throw std::runtime_error(std::string(msg) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")");


inline void APIENTRY debugCallback(GLenum source, GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam)
{
    fprintf(stderr, "OpenGL Debug: %s\n", message);
}

namespace Warnings {

    enum class Type : uint32_t
    {
        General               = 1 << 0,
        PolygonVertexMismatch = 1 << 1,
        MeshVertexMismatch    = 1 << 2,
        MeshInstanceMismatch  = 1 << 3
    };

    inline uint32_t enabled = 0xffffffff;

    inline void enable_all()
    {
        enabled = 0xffffffff;
    }

    inline void disable_all()
    {
        enabled = 0;
    }

    inline void disable(Type type) {
        enabled &= ~static_cast<uint32_t>(type);
    }

    inline void enable(Type type) {
        enabled |= static_cast<uint32_t>(type);
    }

    inline void enableGLDebugMessages() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
    }

    inline void disableGLDebugMessages() {
        glDisable(GL_DEBUG_OUTPUT);
        glDisable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    inline void warn(const std::string& message, Type type)
    {
        if (enabled & static_cast<uint32_t>(type))
            std::cerr << message << std::endl;
    }

}




#ifdef DEBUG_MODE
#define DEBUG_CACHE
#endif


#endif //WARNINGS_H
