#ifndef IZN_PARSER_HPP
#define IZN_PARSER_HPP

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <variant>
#include <algorithm>

namespace Izn {

class Parser {
private:
    using ValueType = std::variant<std::string, int, double, bool>;
    std::map<std::string, std::map<std::string, ValueType>> data;
    std::string currentSection;

    static bool isInteger(const std::string& str) {
        if(str.empty()) return false;
        return str.find_first_not_of("0123456789-") == std::string::npos;
    }

    static bool isDouble(const std::string& str) {
        if(str.empty()) return false;
        char* end = nullptr;
        std::strtod(str.c_str(), &end);
        return end != str.c_str() && *end == '\0' && str.find('.') != std::string::npos;
    }

    static bool isBool(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower == "true" || lower == "false";
    }

    static ValueType parseValue(const std::string& str) {
        std::string value = str;
        if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }

        if (isBool(value)) {
            std::string lower = value;
            std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
            return lower == "true";
        }

        if (isInteger(value)) {
            return std::stoi(value);
        }

        if (isDouble(value)) {
            return std::stod(value);
        }

        return value;
    }

public:
    Parser() {}

    bool parse(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            if (line[0] == '@') {
                currentSection = line.substr(1);
                currentSection.erase(0, currentSection.find_first_not_of(" \t"));
                currentSection.erase(currentSection.find_last_not_of(":") + 1);
                continue;
            }

            size_t delimPos = line.find(':');
            if (delimPos != std::string::npos && !currentSection.empty()) {
                std::string key = line.substr(0, delimPos);
                std::string value = line.substr(delimPos + 1);

                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t?") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                data[currentSection][key] = parseValue(value);
            }
        }

        file.close();
        return true;
    }

    template<typename T>
    T get(const std::string& section, const std::string& key) const {
        auto secIt = data.find(section);
        if (secIt != data.end()) {
            auto keyIt = secIt->second.find(key);
            if (keyIt != secIt->second.end()) {
                try {
                    return std::get<T>(keyIt->second);
                } catch (const std::bad_variant_access&) {
                    std::cerr << "Type mismatch for " << section << "." << key << std::endl;
                }
            }
        }
        return T{};
    }

    auto getValue(const std::string& section, const std::string& key) const {
        auto secIt = data.find(section);
        if (secIt != data.end()) {
            auto keyIt = secIt->second.find(key);
            if (keyIt != secIt->second.end()) {
                return keyIt->second;
            }
        }
        return ValueType{std::string{}};
    }

    void print() const {
        for (const auto& section : data) {
            std::cout << "@" << section.first << "::" << std::endl;
            for (const auto& pair : section.second) {
                std::cout << "    " << pair.first << ": ";
                std::visit([](const auto& val) { std::cout << val; }, pair.second);
                std::cout << std::endl;
                std::cout << std::endl;
            }
        }
    }
};

} // namespace Ion

#endif // IZN_PARSER_HPP