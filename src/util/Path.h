#ifndef GLWRAPPER_II_PATH_H
#define GLWRAPPER_II_PATH_H
#include <filesystem>



class Path {
public:
    Path(const std::string &path) {
        this->m_path = path;
        m_string = path;
    }

    Path(const char* path) {
        this->m_path = path;
        m_string = path;
    }

    Path(const std::filesystem::path& path) {
        this->m_path = path;
        m_string = path.string();
    }

    Path() = default;

    [[nodiscard]] const char* c_str() const {
        return m_string.c_str();
    }

    [[nodiscard]] std::string string() const {
        return m_path.string();
    }

    [[nodiscard]] std::filesystem::path filesystem_path() const {
        return m_path;
    }
private:
    std::filesystem::path m_path;
    std::string m_string;
};


#endif //GLWRAPPER_II_PATH_H
