#include "BloomEffect.hpp"

#include <cmath>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "ShaderSources.hpp"

using namespace cubedemo;

void BloomEffect::setupFBO()
{
    GLenum drawBuffers[] = { gl::COLOR_ATTACHMENT0 };

    // Set up HDR framebuffer
    gl::GenFramebuffers(1, &m_hdrFBO);
    gl::BindFramebuffer(gl::FRAMEBUFFER, m_hdrFBO);
    {
        // Create and setup texture object
        gl::GenTextures(1, &m_hdrTex);
        gl::ActiveTexture(gl::TEXTURE0);
        gl::BindTexture(gl::TEXTURE_2D, m_hdrTex);
        gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGB32F, m_fbWidth, m_fbHeight, 0, gl::RGB, gl::FLOAT, nullptr);

        // Bind texture to fbo
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, m_hdrTex, 0);

        // Create and setup depth buffer
        GLuint depthBuf;
        gl::GenRenderbuffers(1, &depthBuf);
        gl::BindRenderbuffer(gl::RENDERBUFFER, depthBuf);
        gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT, m_fbWidth, m_fbHeight);

        // Bind depth buffer to hdr fbo
        gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, depthBuf);

        // Set fragment output targets
        gl::DrawBuffers(1, drawBuffers);
    }
    gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

    // Set up blur and bright-pass filter fbo
    gl::GenFramebuffers(1, &m_blurFBO);
    gl::BindFramebuffer(gl::FRAMEBUFFER, m_blurFBO);
    {
        // Create two textures for the bright-pass filter and the two-pass blur
        gl::GenTextures(1, &m_tex1);
        gl::ActiveTexture(gl::TEXTURE1);
        gl::BindTexture(gl::TEXTURE_2D, m_tex1);
        gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGB32F, m_bloomBufWidth, m_bloomBufHeight, 0, gl::RGB, gl::FLOAT, nullptr);

        gl::GenTextures(1, &m_tex2);
        gl::ActiveTexture(gl::TEXTURE2);
        gl::BindTexture(gl::TEXTURE_2D, m_tex2);
        gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGB32F, m_bloomBufWidth, m_bloomBufHeight, 0, gl::RGB, gl::FLOAT, nullptr);

        // Bind texture 1 to the fbo
        gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, m_tex1, 0);

        gl::DrawBuffers(1, drawBuffers);
    }
    gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void BloomEffect::setupShader()
{
    m_shader.attachShaderFromSource(gl::VERTEX_SHADER, shaderSourceHDRBloomVert());
    m_shader.attachShaderFromSource(gl::FRAGMENT_SHADER, shaderSourceHDRBloomFrag());
    m_shader.link();
    m_shader.addAttributes({});
    m_shader.addUniforms({});
}

void BloomEffect::setupFullscreenQuad()
{

}

float BloomEffect::gauss(float, float)
{

}

void BloomEffect::computeLogAveLuminance()
{
    // Read texture content into a pixel buffer
    GLfloat *texData = new GLfloat[m_fbWidth * m_fbHeight * 3];
    gl::ActiveTexture(gl::TEXTURE0);
    gl::BindTexture(gl::TEXTURE_2D, m_hdrTex);
    gl::GetTexImage(gl::TEXTURE_2D, 0, gl::RGB, gl::FLOAT, texData);

    float sum = 0.0f;
    for (size_t i = 0; i < m_fbWidth * m_fbHeight; i++)
    {
        float lum = glm::dot(
            glm::vec3(texData[i * 3 + 0], texData[i * 3 + 1], texData[i * 3 + 2]),
            glm::vec3(0.2126f, 0.7152f, 0.0722f));
        sum += logf(lum + 0.00001f);
    }

    m_aveLum = expf(sum / (m_fbWidth * m_fbHeight));

    delete[] texData;
}

BloomEffect::BloomEffect()
    : m_fbWidth{ 0 }, m_fbHeight{ 0 }, m_bloomBufWidth{ 0 }, m_bloomBufHeight{ 0 }
{

}

void BloomEffect::resize(size_t width, size_t height)
{
    m_fbWidth = width;
    m_fbHeight = height;
    m_bloomBufWidth = width / 8;
    m_bloomBufHeight = height / 8;
}

void BloomEffect::renderPass1()
{

}

void BloomEffect::renderPass2()
{

}

void BloomEffect::renderPass3()
{

}

void BloomEffect::renderPass4()
{

}

void BloomEffect::renderPass5()
{

}
