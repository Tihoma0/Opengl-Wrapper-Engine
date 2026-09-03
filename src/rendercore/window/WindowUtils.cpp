#ifdef _WIN32


#include <windows.h>
#include "WindowUtils.h"
#include <glad/glad.h>








[[nodiscard]] int getMonitorWidth(){return GetSystemMetrics(SM_CXSCREEN);}
[[nodiscard]] int getMonitorHeight(){return GetSystemMetrics(SM_CYSCREEN);}

void* getHInstance() {
    static HINSTANCE hInst = nullptr;
    if (!hInst)
        hInst = GetModuleHandle(nullptr);
    return hInst;
}

GLADloadproc getProcAddr() {
    return reinterpret_cast<GLADloadproc>(wglGetProcAddress);
}

#endif //_WIN32
