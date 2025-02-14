#pragma once

#include <vector>
#include <memory>

namespace Kosmic::Renderer {

class RenderPass {
public:
    virtual ~RenderPass() = default;
    // Execute the pass
    virtual void Execute() = 0;
};

class RenderGraph {
public:
    RenderGraph() = default;
    ~RenderGraph() = default;

    // Add a render pass to the graph
    void AddPass(std::shared_ptr<RenderPass> pass) { m_Passes.push_back(pass); }

    // Execute all added passes sequentially
    void Execute() {
        for (auto& pass : m_Passes)
            pass->Execute();
    }

private:
    std::vector<std::shared_ptr<RenderPass>> m_Passes;
};

} // namespace Kosmic::Renderer
