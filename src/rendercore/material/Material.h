#ifndef MATERIAL_H
#define MATERIAL_H
#include <memory>
#include <unordered_map>
#include <variant>

#include  "rendercore/core/math/vec/Vec2.h"
#include  "rendercore/core/math/vec/Vec3.h"
#include  "rendercore/core/math/vec/Vec4.h"
#include  "rendercore/core/math/mat/Mat3.h"
#include  "rendercore/core/math/mat/Mat4.h"
#include  "rendercore/core/math/ivec/IVec2.h"
#include  "rendercore/core/math/ivec/IVec3.h"
#include  "rendercore/core/math/ivec/IVec4.h"

class Shader;
class Texture2D;
class Sampler;

using UniformValue = std::variant<
    int,
    float,
    Vec2,
    Vec3,
    Vec4,
    Mat3,
    Mat4,
    IVec2,
    IVec3,
    IVec4
>;

struct UniformEntry {
    UniformValue value;
    bool dirty = true;
};

struct TextureBinding { // TODO: add texture atlas support - also on glsl side
    std::shared_ptr<Texture2D> texture;
    std::shared_ptr<Sampler> sampler;
    bool is_atlas = false;
    Vec4 uv;
};


class Material {
public:
    std::shared_ptr<Shader> m_shader;
    std::unordered_map<std::string, UniformEntry> m_uniforms;
    std::unordered_map<std::string, TextureBinding> m_texture_bindings;

    explicit Material(std::shared_ptr<Shader> shader);

    void set_uniform(const std::string& name, const UniformValue& value);

    void set_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture);

    void set_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture, const std::shared_ptr<Sampler>& sampler);

    void upload();
};
#endif //MATERIAL_H
