//
// Created by olber on 8/27/2022.
//

#pragma once
#include <string>

#include "Image.h"

namespace output {
    class ImageWriter_I {
    public:
        virtual ~ImageWriter_I() = default;

        virtual void write(const Image& image, const std::string& filepath) = 0;
    };
}