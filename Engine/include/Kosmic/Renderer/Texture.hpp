#pragma once
#include <string>
#include <cstdint>

namespace Kosmic::Renderer {

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    inline uint32_t GetID() const { return m_RendererID; }

private:
    std::string m_Path;
    uint32_t m_RendererID;
    int m_Width, m_Height, m_Channels;
};

} // namespace Kosmic::Renderer
