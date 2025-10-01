#include "utils/string_utils.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <cstdarg>

namespace GameEngine2D {

std::string StringUtils::trim(const std::string& str) {
    return trimRight(trimLeft(str));
}

std::string StringUtils::trimLeft(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string StringUtils::trimRight(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string StringUtils::toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringUtils::toUpperCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }
    
    return result;
}

std::vector<std::string> StringUtils::split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    result.push_back(str.substr(start));
    return result;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& delimiter) {
    if (strings.empty()) {
        return "";
    }
    
    std::string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        result += delimiter + strings[i];
    }
    
    return result;
}

bool StringUtils::startsWith(const std::string& str, const std::string& prefix) {
    return str.length() >= prefix.length() && 
           str.substr(0, prefix.length()) == prefix;
}

bool StringUtils::endsWith(const std::string& str, const std::string& suffix) {
    return str.length() >= suffix.length() && 
           str.substr(str.length() - suffix.length()) == suffix;
}

bool StringUtils::contains(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

bool StringUtils::isEmpty(const std::string& str) {
    return str.empty();
}

bool StringUtils::isWhitespace(const std::string& str) {
    return std::all_of(str.begin(), str.end(), ::isspace);
}

std::string StringUtils::replace(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = result.find(from);
    if (pos != std::string::npos) {
        result.replace(pos, from.length(), to);
    }
    return result;
}

std::string StringUtils::replaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return result;
}

std::string StringUtils::format(const std::string& format, ...) {
    va_list args;
    va_start(args, format);
    
    // Get the size needed for the formatted string
    va_list argsCopy;
    va_copy(argsCopy, args);
    int size = vsnprintf(nullptr, 0, format.c_str(), argsCopy);
    va_end(argsCopy);
    
    if (size < 0) {
        va_end(args);
        return "";
    }
    
    // Create buffer and format the string
    std::string result(size + 1, '\0');
    vsnprintf(&result[0], size + 1, format.c_str(), args);
    va_end(args);
    
    // Remove null terminator
    result.resize(size);
    return result;
}

std::string StringUtils::padLeft(const std::string& str, size_t width, char padChar) {
    if (str.length() >= width) {
        return str;
    }
    return std::string(width - str.length(), padChar) + str;
}

std::string StringUtils::padRight(const std::string& str, size_t width, char padChar) {
    if (str.length() >= width) {
        return str;
    }
    return str + std::string(width - str.length(), padChar);
}

std::string StringUtils::toString(int value) {
    return std::to_string(value);
}

std::string StringUtils::toString(float value) {
    return std::to_string(value);
}

std::string StringUtils::toString(double value) {
    return std::to_string(value);
}

std::string StringUtils::toString(bool value) {
    return value ? "true" : "false";
}

bool StringUtils::isDigit(char c) {
    return std::isdigit(c);
}

bool StringUtils::isAlpha(char c) {
    return std::isalpha(c);
}

bool StringUtils::isAlphaNumeric(char c) {
    return std::isalnum(c);
}

bool StringUtils::isWhitespace(char c) {
    return std::isspace(c);
}

bool StringUtils::isPrintable(char c) {
    return std::isprint(c);
}

std::string StringUtils::escape(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            case '\"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string StringUtils::unescape(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case 'n': result += '\n'; ++i; break;
                case 'r': result += '\r'; ++i; break;
                case 't': result += '\t'; ++i; break;
                case '"': result += '"'; ++i; break;
                case '\\': result += '\\'; ++i; break;
                default: result += str[i]; break;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

} // namespace GameEngine2D
