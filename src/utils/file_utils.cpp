#include "utils/file_utils.h"
#include "utils/logger.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace GameEngine2D {

std::string FileUtils::readTextFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR_FMT("Failed to open file: {}", filepath);
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

std::vector<unsigned char> FileUtils::readBinaryFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        LOG_ERROR_FMT("Failed to open binary file: {}", filepath);
        return {};
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<unsigned char> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    file.close();
    
    return data;
}

bool FileUtils::writeTextFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR_FMT("Failed to create file: {}", filepath);
        return false;
    }
    
    file << content;
    file.close();
    
    return true;
}

bool FileUtils::writeBinaryFile(const std::string& filepath, const std::vector<unsigned char>& data) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        LOG_ERROR_FMT("Failed to create binary file: {}", filepath);
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();
    
    return true;
}

bool FileUtils::fileExists(const std::string& filepath) {
    return std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath);
}

bool FileUtils::directoryExists(const std::string& dirpath) {
    return std::filesystem::exists(dirpath) && std::filesystem::is_directory(dirpath);
}

bool FileUtils::createDirectory(const std::string& dirpath) {
    try {
        return std::filesystem::create_directories(dirpath);
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to create directory {}: {}", dirpath, e.what());
        return false;
    }
}

bool FileUtils::deleteFile(const std::string& filepath) {
    try {
        return std::filesystem::remove(filepath);
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to delete file {}: {}", filepath, e.what());
        return false;
    }
}

bool FileUtils::deleteDirectory(const std::string& dirpath) {
    try {
        return std::filesystem::remove_all(dirpath) > 0;
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to delete directory {}: {}", dirpath, e.what());
        return false;
    }
}

std::string FileUtils::getDirectory(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.parent_path().string();
}

std::string FileUtils::getFilename(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.filename().string();
}

std::string FileUtils::getExtension(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.extension().string();
}

std::string FileUtils::getFilenameWithoutExtension(const std::string& filepath) {
    std::filesystem::path path(filepath);
    return path.stem().string();
}

std::string FileUtils::combinePath(const std::string& path1, const std::string& path2) {
    std::filesystem::path p1(path1);
    std::filesystem::path p2(path2);
    return (p1 / p2).string();
}

std::vector<std::string> FileUtils::listFiles(const std::string& dirpath) {
    std::vector<std::string> files;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to list files in {}: {}", dirpath, e.what());
    }
    
    return files;
}

std::vector<std::string> FileUtils::listDirectories(const std::string& dirpath) {
    std::vector<std::string> directories;
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
            if (entry.is_directory()) {
                directories.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to list directories in {}: {}", dirpath, e.what());
    }
    
    return directories;
}

std::vector<std::string> FileUtils::listFilesRecursive(const std::string& dirpath) {
    std::vector<std::string> files;
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dirpath)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to list files recursively in {}: {}", dirpath, e.what());
    }
    
    return files;
}

size_t FileUtils::getFileSize(const std::string& filepath) {
    try {
        return std::filesystem::file_size(filepath);
    } catch (const std::exception& e) {
        LOG_ERROR_FMT("Failed to get file size for {}: {}", filepath, e.what());
        return 0;
    }
}

std::string FileUtils::getFileHash(const std::string& filepath) {
    // Simple hash implementation - in a real application, you'd use a proper hash function
    auto data = readBinaryFile(filepath);
    if (data.empty()) {
        return "";
    }
    
    size_t hash = 0;
    for (unsigned char byte : data) {
        hash = hash * 31 + byte;
    }
    
    std::ostringstream oss;
    oss << std::hex << hash;
    return oss.str();
}

bool FileUtils::isFile(const std::string& path) {
    return std::filesystem::is_regular_file(path);
}

bool FileUtils::isDirectory(const std::string& path) {
    return std::filesystem::is_directory(path);
}

} // namespace GameEngine2D
