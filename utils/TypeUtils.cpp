#include "TypeUtils.h"

using namespace upload;
ImageFormat upload::getImageFormatFromFilePath(const std::string &filepath)
{
    size_t dotPos = filepath.find_last_of('.');
    if (dotPos == std::string::npos)
    {
        return ImageFormat::ImageFormat_Unknown;
    }
    std::string extension = filepath.substr(dotPos + 1);
    for (auto &c : extension)
    {
        c = tolower(c);
    }
    if (extension == "jpg" || extension == "jpeg")
    {
        return ImageFormat::ImageFormat_JPEG;
    }
    else if (extension == "png")
    {
        return ImageFormat::ImageFormat_PNG;
    }
    else if (extension == "bmp")
    {
        return ImageFormat::ImageFormat_BMP;
    }
    else if (extension == "gif")
    {
        return ImageFormat::ImageFormat_GIF;
    }
    else if (extension == "tiff" || extension == "tif")
    {
        return ImageFormat::ImageFormat_TIFF;
    }
    else
    {
        return ImageFormat::ImageFormat_Unknown;
    }
}
std::string upload::getImageStringFromImageFormat(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::ImageFormat_JPEG:
        return "JPEG";
    case ImageFormat::ImageFormat_PNG:
        return "PNG";
    case ImageFormat::ImageFormat_BMP:
        return "BMP";
    case ImageFormat::ImageFormat_GIF:
        return "GIF";
    case ImageFormat::ImageFormat_TIFF:
        return "TIFF";
    default:
        return "Unknown";
    }
    return "Unknown";
}