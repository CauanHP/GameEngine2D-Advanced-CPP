#pragma once

#include <string>
#include <vector>

namespace GameEngine2D {

class StringUtils {
public:
    // String manipulation
    static std::string trim(const std::string& str);
    static std::string trimLeft(const std::string& str);
    static std::string trimRight(const std::string& str);
    static std::string toLowerCase(const std::string& str);
    static std::string toUpperCase(const std::string& str);
    
    // String splitting and joining
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::vector<std::string> split(const std::string& str, const std::string& delimiter);
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    
    // String checking
    static bool startsWith(const std::string& str, const std::string& prefix);
    static bool endsWith(const std::string& str, const std::string& suffix);
    static bool contains(const std::string& str, const std::string& substring);
    static bool isEmpty(const std::string& str);
    static bool isWhitespace(const std::string& str);
    
    // String replacement
    static std::string replace(const std::string& str, const std::string& from, const std::string& to);
    static std::string replaceAll(const std::string& str, const std::string& from, const std::string& to);
    
    // String formatting
    static std::string format(const std::string& format, ...);
    static std::string padLeft(const std::string& str, size_t width, char padChar = ' ');
    static std::string padRight(const std::string& str, size_t width, char padChar = ' ');
    
    // String conversion
    static std::string toString(int value);
    static std::string toString(float value);
    static std::string toString(double value);
    static std::string toString(bool value);
    
    // Character utilities
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);
    static bool isWhitespace(char c);
    static bool isPrintable(char c);
    
    // String encoding
    static std::string escape(const std::string& str);
    static std::string unescape(const std::string& str);
};

} // namespace GameEngine2D
