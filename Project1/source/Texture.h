#pragma once

#include <string_view>

namespace nzgdc_demo
{
    class Texture
    {
    public:
        Texture(const std::string_view path);
        ~Texture();

        void Bind() const;

    private:
        void LoadTexture(const std::string_view path);

    private:
        unsigned int m_id;
    };
}
