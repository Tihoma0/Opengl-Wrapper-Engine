
#include "rendercore/material/Material.h"

#include <iostream>
#include <memory>
#include <utility>
#include <variant>

#include "Shader.h"
#include "rendercore/surface/textures/Sampler.h"
#include "rendercore/surface/textures/Texture.h"



Material::Material(std::shared_ptr<Shader> shader) {
    m_shader = std::move(shader);
}

void Material::set_uniform(const std::string& name, const UniformValue& value) {
    auto& entry = m_uniforms[name];
    entry.value = value;
    entry.dirty = true;
}

void Material::set_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture) {
    m_texture_bindings[name] = { texture };
}

void Material::set_texture(const std::string& name, const std::shared_ptr<Texture2D>& texture, const std::shared_ptr<Sampler>& sampler) {
    m_texture_bindings[name] = { texture, sampler };
}

void Material::upload() {
    m_shader->use();
    for (auto& [name, entry] : m_uniforms) {
        if (!entry.dirty)
            continue;
        std::visit([&](auto&& v) {
            m_shader->setUniform(name, v);
        }, entry.value);
        entry.dirty = false;
    }
    int slot = 0;
    for (const auto& [name, value] : m_texture_bindings) {
        const auto [texture, sampler] = value;
        texture->bind(slot);
        // std::cout << texture->get_default_sampler() << std::endl;
        // std::cout << sampler << std::endl;
        if (sampler)
            sampler->bind(slot);
        else {
            texture->get_default_sampler()->bind(slot);
            // texture->get_default_sampler()->print_filters();
            // GLint filter;
            // glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &filter);
            //
            // std::cout << "min filter: 0x"
            //           << std::hex << filter << std::dec << '\n';
        }
        m_shader->setUniform(name, slot);
        slot++;
    }

#ifdef DEBUG_CACHE
    GLContext::current_context->validate_state();
#endif
}
