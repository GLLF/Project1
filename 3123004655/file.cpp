#include "main.h"

std::string readFileContent(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }
    
    // 获取文件大小
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // 读取文件内容
    std::string content(size, '\0');
    if (!file.read(&content[0], size)) {
        throw std::runtime_error("读取文件失败: " + filePath);
    }
    
    file.close();
    return content;
}

void writeResultToFile(const std::string& outputPath, double similarity) {
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        throw std::runtime_error("无法创建输出文件: " + outputPath);
    }
    
    // 输出保留两位小数的相似度
    outFile << std::fixed << std::setprecision(2) << similarity;
    outFile.close();
}