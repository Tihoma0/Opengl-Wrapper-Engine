#ifndef PATH_H
#define PATH_H

#include <filesystem>
#include <string>

class Path {
public:
    Path() = default;

    Path(const char* path)
        : m_path(path) {}

    Path(const std::string& path)
        : m_path(path) {}

    Path(const std::filesystem::path& path)
        : m_path(path) {}

    [[nodiscard]]
    std::string string() const {
        return m_path.string();
    }

    [[nodiscard]]
    const char* c_str() const {
        m_string = m_path.string();
        return m_string.c_str();
    }

    [[nodiscard]]
    const std::filesystem::path& filesystem_path() const {
        return m_path;
    }

    [[nodiscard]]
    bool operator==(const Path& other) const {
        return m_path == other.m_path;
    }

    [[nodiscard]]
    bool operator!=(const Path& other) const {
        return !(*this == other);
    }

private:
    std::filesystem::path m_path;
    mutable std::string m_string;
};

#endif // PATH_H
