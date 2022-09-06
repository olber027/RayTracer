//
// Created by olber on 9/6/2022.
//
#pragma once

#include <string>
#include <memory>
#include <map>

#include "ImageWriter_I.h"
#include "PNGImageWriter.h"
#include "PPMImageWriter.h"

namespace output {
    class ImageWriterBuilder {
    private:
        static std::unique_ptr<ImageWriter_I> createPPMWriter();
        static std::unique_ptr<ImageWriter_I> createPNGWriter();

        static std::map<std::string, std::string> suffix_map;

    public:
        static std::unique_ptr<ImageWriter_I> createWriter(const std::string& file_suffix);
    };
}