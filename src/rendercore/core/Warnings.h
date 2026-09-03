#ifndef WARNINGS_H
#define WARNINGS_H

#include <string>
#include <stdexcept>

#define THROW_RUNTIME(msg) \
throw std::runtime_error(std::string(msg) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")")

class GLContext;

struct WarningConfig {
    bool mesh_vertex_mismatch = true;
    bool mesh_instance_mismatch = true;
    bool gl_debug_messages = true;
};

inline void APIENTRY debugCallback(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam)
{
    fprintf(stderr, "OpenGL Debug: %s\n", message);
}

inline WarningConfig warnings;

namespace Warnings {

    inline void disableMeshWarnings() {
        warnings.mesh_vertex_mismatch = false;
        warnings.mesh_instance_mismatch = false;
    }

    inline void enableMeshWarnings() {
        warnings.mesh_vertex_mismatch = true;
        warnings.mesh_instance_mismatch = true;
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

    inline void disableWarnings() {
        disableMeshWarnings();
        disableGLDebugMessages();
    }


    inline void enableWarnings() {
        enableMeshWarnings();
        enableGLDebugMessages();
    }

}




#ifdef DEBUG_MODE
#define DEBUG_CACHE
#endif


#endif //WARNINGS_H
