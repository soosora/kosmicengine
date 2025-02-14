#include "Kosmic/Renderer/Framebuffer.hpp"
#include <GL/glew.h>
#include "Kosmic/Core/Logging.hpp"

namespace Kosmic::Renderer {

class OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height) { Invalidate(); }
    
    ~OpenGLFramebuffer() override {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteRenderbuffers(1, &m_DepthAttachment);
    }
    
    void Bind() const override {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }
    
    void Unbind() const override {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void Resize(uint32_t width, uint32_t height) override {
        m_Width = width;
        m_Height = height;
        Invalidate();
    }
    
private:
    void Invalidate() {
        if(m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteRenderbuffers(1, &m_DepthAttachment);
        }
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        // Create color attachment texture
        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        // Create renderbuffer for depth & stencil attachment
        glGenRenderbuffers(1, &m_DepthAttachment);
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            KOSMIC_ERROR("Framebuffer is incomplete!");
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    uint32_t m_RendererID = 0;
    uint32_t m_ColorAttachment = 0;
    uint32_t m_DepthAttachment = 0;
    uint32_t m_Width, m_Height;
};

std::shared_ptr<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height) {
    return std::make_shared<OpenGLFramebuffer>(width, height);
}

} // namespace Kosmic::Renderer
