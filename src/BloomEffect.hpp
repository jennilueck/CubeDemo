#pragma once

#include "GLShader.hpp"

namespace cubedemo
{
    // Implements a bloom effect
    // For this, there are 5 render passes:
    // 1 - Render the scene to a HDR texture
    // 2 - Run a bright-pass filter over the HDR texture from step 1 and downsample
    // 3 + 4 - Apply a gaussian blur to the downsampled texture
    // 5 - Apply tone mapping to the original HDR texture, and add the blurred bright-pass filter results
    class BloomEffect
    {
    private:
        GLShader m_shader;
        size_t m_fbWidth, m_fbHeight; // Window framebuffer size
        size_t m_bloomBufWidth, m_bloomBufHeight; // Size for bloom buffer
        GLuint m_fullscreenQuadVAO;
        GLuint m_pass1Index, m_pass2Index, m_pass3Index, m_pass4Index, m_pass5Index; // Render pass subroutine indices
        GLuint m_hdrFBO, m_blurFBO; // FBO objects
        GLuint m_hdrTex, m_tex1, m_tex2; // Texture objects
        GLuint m_linearSampler, m_nearestSampler; // Sampler objects

        float m_aveLum;

        void setupFBO();
        void setupShader();
        void setupFullscreenQuad();

        float gauss(float, float);
        void computeLogAveLuminance();

    public:
        BloomEffect();

        void resize(size_t width, size_t height);

        void renderPass1();
        void renderPass2();
        void renderPass3();
        void renderPass4();
        void renderPass5();
    };
}
