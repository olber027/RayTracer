//
// Created by olber on 8/27/2022.
//
#include <fstream>

#include "Image.h"
#include "PPMImageWriter.h"

namespace output
{
    void PPMImageWriter::write(const Image& image, const std::string& filepath)
    {
        std::ofstream out(filepath);
        out << "P3" << "\n";
        out << image.width() << " " << image.height() << "\n";
        out << image.getColorRange() << "\n";
        for(int j = 0; j < image.height(); j++) {
            for(int i = 0; i < image.width(); i++) {
                out << image.at(i, j).to_string() << " ";
            }
            out << "\n";
        }
    }
}