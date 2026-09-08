
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <filesystem>
#include <string>
#include "Image.h"



class ImageViewer {
public:
    static void showImage(const Image& image, const std::string &name = "");

private:
    inline static Path path = "current_shown/";
    inline static bool initialized = false;
    static void init();

    static void open_file(const Path &path);
};
#endif //IMAGEVIEWER_H
