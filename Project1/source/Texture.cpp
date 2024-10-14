#include "Texture.h"
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace nzgdc_demo
{
    Texture::Texture(const std::string_view path) :
        m_id{0}
    {
        LoadTexture(path);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_id);
    }

    void Texture::Bind() const
    {
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::SetTexturePath(const std::string_view path)
    {
        LoadTexture(path);
    }

    unsigned Texture::GetTextureId() const
    {
        return m_id;
    }

    void Texture::LoadTexture(const std::string_view path)
    {
        if (path.empty())
        {
            return;
        }
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);  
        int width{0}, height{0}, numChannels{0};
        unsigned char* data{stbi_load(path.data(), &width, &height, &numChannels, 0)};
        if (!data)
        {
            std::puts("ERROR: Failed to read image resource!");
            return;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
}
