
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <filesystem>
#include <string>

#include "Image.h"



class ImageViewer {
public:
    void showImage(const Image& image, std::string name = "");

private:
    std::filesystem::path path = "current_shown/";
    bool initialized = false;
    void init();

    static void open_file(std::filesystem::path path);
};
#endif //IMAGEVIEWER_H
