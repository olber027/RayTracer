//
// Created by olber on 9/4/2022.
//

#pragma once

#include <string>
#include <fstream>
#include "Image.h"
#include "ImageWriter_I.h"

namespace output {
    class PNGImageWriter : public ImageWriter_I
    {
    public:
        PNGImageWriter() = default;

        void write(const Image& image, const std::string& filepath) override;
    };
}