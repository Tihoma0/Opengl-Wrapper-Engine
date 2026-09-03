#ifndef ARRAYBUFFER_H
#define ARRAYBUFFER_H
#include <memory>
#include <vector>

#include "glad/glad.h"
#include "rendercore/core/enums/textures/DrawMode.h"


struct ArrayBufferAttribute {
    int size;
    int layout;
    int offset = 0;
    int divisor = 0;

    ArrayBufferAttribute(int size, int layout) {
        this->size = size;
        this->layout = layout;
    }

    ArrayBufferAttribute& set_layout(int layout) {
        this->layout = layout;
        return *this;
    }

    ArrayBufferAttribute& set_offset(int offset) {
        this->offset = offset;
        return *this;
    }

    ArrayBufferAttribute& set_divisor(int divisor) {
        this->divisor = divisor;
        return *this;
    }
};

class ArrayBuffer : public std::enable_shared_from_this<ArrayBuffer> {
public:

    std::vector<float> data;
    unsigned int usage = GL_STATIC_DRAW;
    std::vector<ArrayBufferAttribute> attributes;
    bool is_built = false;

    static std::shared_ptr<ArrayBuffer> create(const std::vector<float> &data);

    ArrayBuffer(const ArrayBuffer&) = delete;
    ArrayBuffer& operator=(const ArrayBuffer&) = delete;

    ArrayBuffer(ArrayBuffer&&) noexcept = default;
    ArrayBuffer& operator=(ArrayBuffer&&) noexcept = default;

    ~ArrayBuffer();

    void bind() const;

    void unbind() const;


    std::shared_ptr<ArrayBuffer> set_data(const std::vector<float>& new_data, const DrawMode usage = DrawMode::DYNAMIC_DRAW);

    void update_data(const std::vector<float>& new_data, int start = 0);
    std::shared_ptr<ArrayBuffer> add_attribute(ArrayBufferAttribute attribute);
    std::shared_ptr<ArrayBuffer> set_usage(unsigned int usage);

    std::shared_ptr<ArrayBuffer> build(const GLuint vao);

    [[nodiscard]] unsigned int id() const { return m_id; }

    [[nodiscard]] int size() const { return data.size(); }

    [[nodiscard]] int group_count() const { return m_group_count; }

    [[nodiscard]] int stride() const { return m_stride; }

private:
    unsigned int m_id = 0;
    int m_group_count = 0;
    int m_stride = 0;

    explicit ArrayBuffer(std::vector<float> data);
};



#endif //ARRAYBUFFER_H
