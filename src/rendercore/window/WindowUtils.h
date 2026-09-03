#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H
#ifdef _WIN32


#include <limits>
#include <glad/glad.h>

enum class WindowZLayers {
    ALWAYS_ON_TOP,
    TOP,
    NORMAL,
    BOTTOM
};

enum class WindowBackend {
    LayeredCPU,
    OpenGL
};


namespace WindowPositions {
    inline constexpr int CENTER_X = std::numeric_limits<int>::min();
    inline constexpr int CENTER_Y = std::numeric_limits<int>::min() + 1;
    inline constexpr int TOP      = std::numeric_limits<int>::min() + 2;
    inline constexpr int LEFT     = std::numeric_limits<int>::min() + 3;
    inline constexpr int BOTTOM   = std::numeric_limits<int>::min() + 4;
    inline constexpr int RIGHT    = std::numeric_limits<int>::min() + 5;
};



struct WindowColor {
    uint8_t a, r, g, b;

    constexpr WindowColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
        : a(alpha), r(red), g(green), b(blue) {}

    [[nodiscard]] constexpr uint32_t toPixel() const {
        return (static_cast<uint32_t>(a) << 24) |
               (static_cast<uint32_t>(r) << 16) |
               (static_cast<uint32_t>(g) << 8) |
               static_cast<uint32_t>(b);
    }
};

struct PixelBuf {
    int width, height;
    uint32_t *data;
    bool ownsMemory = false;

    PixelBuf(uint32_t *buf, int width, int height, bool owns=false)
        : width(width), height(height), data(buf), ownsMemory(owns) {}

    ~PixelBuf() {
        if (ownsMemory) {
            delete[] data;
        }
    }

    uint32_t &operator()(int x, int y) {
        return data[y * width + x];
    }

    const uint32_t &operator()(int x, int y) const {
        return data[y * width + x];
    }
};



[[nodiscard]] int getMonitorWidth();
[[nodiscard]] int getMonitorHeight();

void* getHInstance();

GLADloadproc getProcAddr();


#endif //_WIN32
#endif //WINDOWUTILS_H
