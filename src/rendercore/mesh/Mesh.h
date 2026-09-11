#ifndef MESH_H
#define MESH_H
#include <memory>
#include <vector>

#include <rendercore/mesh/ArrayBuffer.h>
#include <rendercore/mesh/ElementBuffer.h>
#include <glad/glad.h>

class Mesh : public std::enable_shared_from_this<Mesh> {

public:
    static std::shared_ptr<Mesh> create_quad(float x, float y, float width, float height);

    static std::shared_ptr<Mesh> create_line(float x1, float y1, float x2, float y2);

    static std::shared_ptr<Mesh> create();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;

    ~Mesh();

    std::shared_ptr<Mesh> add_vertex_buffer(const std::shared_ptr<ArrayBuffer>& buffer);

    std::shared_ptr<Mesh> add_instance_buffer(const std::shared_ptr<ArrayBuffer>& buffer);

    std::shared_ptr<Mesh> add_element_buffer(const std::shared_ptr<ElementBuffer> &buffer);
    void bind() const;
    void unbind() const;

    unsigned int id() const { return vao_id; }

    bool has_element_buffer() const { return has_ebo; }

    int default_index_count() const { return m_default_index_count; }
    int default_vertex_count() const { return m_default_vertex_count; }
    int default_instance_count() const { return m_default_instance_count; }

    [[nodiscard]] std::vector<std::shared_ptr<ArrayBuffer>> get_instance_vbos() { return instance_vbos; }

    [[nodiscard]] std::vector<std::shared_ptr<ArrayBuffer>> get_vertex_vbos() { return vertex_vbos; }

private:
    unsigned int vao_id = 0;
    std::vector<std::shared_ptr<ArrayBuffer>> vertex_vbos;
    std::vector<std::shared_ptr<ArrayBuffer>> instance_vbos;
    std::shared_ptr<ElementBuffer> ebo;
    bool has_ebo = false;
    int m_default_index_count = 0;
    int m_default_vertex_count = 0;
    int m_default_instance_count = 0;

    Mesh() {
        glGenVertexArrays(1, &vao_id);
    }
};


#endif //MESH_H
