#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H
#include <memory>
#include <vector>

#include "glad/glad.h"
#include "rendercore/core/enums/textures/DrawMode.h"

class ElementBuffer {
public:
    static std::shared_ptr<ElementBuffer> create(const std::vector<int> &data, DrawMode draw_mode = DrawMode::STATIC_DRAW);
    void build(GLuint vao);
    ElementBuffer(const ElementBuffer&) = delete;
    ElementBuffer& operator=(const ElementBuffer&) = delete;

    ElementBuffer(ElementBuffer&&) noexcept = default;
    ElementBuffer& operator=(ElementBuffer&&) noexcept = default;

    [[nodiscard]] int size() const { return data.size(); }


private:
    explicit ElementBuffer(std::vector<int> data, DrawMode draw_mode = DrawMode::STATIC_DRAW);

    unsigned int m_id = 0;
    std::vector<int> data;
    DrawMode usage;
    bool is_built = false;
};
#endif //ELEMENTBUFFER_H
