#include <ctime>
#include <filesystem>
#include <string>

#include "Image.h"
#include "windows.h"
#include <shellapi.h>
#include <thread>

#include "ImageProcessor.h"
#include "ImageViewer.h"



void ImageViewer::showImage(const Image& image, const std::string &name) {
    init();
    const auto timestamp =
        std::chrono::high_resolution_clock::now()
            .time_since_epoch()
            .count();
    std::string img_path = path.string();
    if (!name.empty())
        img_path += name;
    else
        img_path += std::to_string(timestamp);
    img_path += ".png";
    const Image shown_image = ImageProcessor::convert(image, PixelStorageFormat::RGBA8);
    shown_image.save_png(img_path);
    open_file(img_path);
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
}

void ImageViewer::init() {
    if (initialized)
        return;
    path = std::filesystem::absolute(path.filesystem_path());
    std::filesystem::create_directories(path.filesystem_path());
    for (const auto& entry : std::filesystem::directory_iterator(path.filesystem_path()))
        std::filesystem::remove_all(entry.path());
    initialized = true;
}

void ImageViewer::open_file(const Path &path)
{
#ifdef _WIN32
    ShellExecuteA(nullptr, "open", path.c_str(), nullptr, nullptr, SW_SHOW);
#elif __APPLE__
    std::string cmd = "open \"" + path + "\"";
    std::system(cmd.c_str());
#else
    std::string cmd = "xdg-open \"" + path + "\"";
    std::system(cmd.c_str());
#endif
}
