#pragma once

#include <string>
#include <vector>

namespace GameEngine2D {

class FileUtils {
public:
    // File reading
    static std::string readTextFile(const std::string& filepath);
    static std::vector<unsigned char> readBinaryFile(const std::string& filepath);
    
    // File writing
    static bool writeTextFile(const std::string& filepath, const std::string& content);
    static bool writeBinaryFile(const std::string& filepath, const std::vector<unsigned char>& data);
    
    // File operations
    static bool fileExists(const std::string& filepath);
    static bool directoryExists(const std::string& dirpath);
    static bool createDirectory(const std::string& dirpath);
    static bool deleteFile(const std::string& filepath);
    static bool deleteDirectory(const std::string& dirpath);
    
    // Path utilities
    static std::string getDirectory(const std::string& filepath);
    static std::string getFilename(const std::string& filepath);
    static std::string getExtension(const std::string& filepath);
    static std::string getFilenameWithoutExtension(const std::string& filepath);
    static std::string combinePath(const std::string& path1, const std::string& path2);
    
    // File listing
    static std::vector<std::string> listFiles(const std::string& dirpath);
    static std::vector<std::string> listDirectories(const std::string& dirpath);
    static std::vector<std::string> listFilesRecursive(const std::string& dirpath);
    
    // File information
    static size_t getFileSize(const std::string& filepath);
    static std::string getFileHash(const std::string& filepath);
    static bool isFile(const std::string& path);
    static bool isDirectory(const std::string& path);
};

} // namespace GameEngine2D
