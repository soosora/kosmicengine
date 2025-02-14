#pragma once

#include <GL/glew.h>

namespace Kosmic::Renderer {

class StateManager {
public:
    // Set the cull face state if needed
    static void SetCullFace(bool enable) {
        if (enable) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);
    }
    
    // Set the depth test state if needed
    static void SetDepthTest(bool enable) {
        if (enable) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }
};

} // namespace Kosmic::Renderer
