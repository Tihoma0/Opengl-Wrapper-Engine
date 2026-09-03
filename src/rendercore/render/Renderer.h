#ifndef RENDERER_H
#define RENDERER_H
#include <memory>

#include "rendercore/surface/Color.h"

class RenderTarget;
class Mesh;
class Material;
struct DrawOptions;


namespace Renderer {
    void clear(Color color);

    void set_target(const std::shared_ptr<RenderTarget> &target);

    void draw(const std::shared_ptr<Mesh>& mesh, Material& material, const DrawOptions& options);
};
#endif //RENDERER_H
