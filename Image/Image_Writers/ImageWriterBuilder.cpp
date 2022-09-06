//
// Created by olber on 9/6/2022.
//

#include <memory>
#include <string>

#include "ImageWriterBuilder.h"
#include "ImageWriter_I.h"
#include "PNGImageWriter.h"
#include "PPMImageWriter.h"

namespace output
{
    std::map<std::string, std::string> ImageWriterBuilder::suffix_map =  {
        {"ppm", "ppm"}, {"PPM", "ppm"},
        {"png", "png"}, {"PNG", "png"}
    };

    std::unique_ptr<ImageWriter_I> ImageWriterBuilder::createPPMWriter()
    {
        return std::make_unique<PPMImageWriter>();
    }

    std::unique_ptr<ImageWriter_I> ImageWriterBuilder::createPNGWriter()
    {
        return std::make_unique<PNGImageWriter>();
    }

    std::unique_ptr<ImageWriter_I> ImageWriterBuilder::createWriter(const std::string& file_suffix)
    {
        if(suffix_map[file_suffix] == "ppm") {
            return createPPMWriter();
        } else if(suffix_map[file_suffix] == "png") {
            return createPNGWriter();
        }
        return nullptr;
    }

}