//
// Created by olber on 8/27/2022.
//

#pragma once
#include <string>
#include <fstream>
#include "Image.h"
#include "ImageWriter_I.h"

namespace output {
    class PPMImageWriter : public ImageWriter_I
    {
    public:
        PPMImageWriter() = default;

        void write(const Image& image, const std::string& filepath) override;
    };
}