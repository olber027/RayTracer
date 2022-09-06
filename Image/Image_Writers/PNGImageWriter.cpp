//
// Created by olber on 9/4/2022.
//

#include <fstream>

#include "Image.h"
#include "Color.h"
#include "PNGImageWriter.h"
#include "Endian.h"
//#include <Magick++.h>

namespace output
{
    void PNGImageWriter::write(const Image& image, const std::string& filepath)
    {
//        auto toMagickColor = [](const color_core::Color& color) {
//            return Magick::Color(color.R(), color.G(), color.B(), color.Alpha());
//        };
//        Magick::Image magickImage = Magick::Image();
//        magickImage.size(Magick::Geometry(image.width(), image.height()));
//        magickImage.colorSpace(Magick::ColorspaceType::sRGBColorspace);
//        magickImage.type(Magick::PaletteAlphaType);
//        magickImage.quantizeColors(256);
//        for(int i = 0; i < image.width(); i++) {
//            for(int j = 0; j < image.height(); j++) {
//                magickImage.pixelColor(i, j, toMagickColor(image.at(i, j)));
//            }
//        }
//        magickImage.write(filepath);
    }
}