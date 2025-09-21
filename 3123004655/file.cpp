#include "main.h"

std::string readFileContent(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("�޷����ļ�: " + filePath);
    }
    
    // ��ȡ�ļ���С
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // ��ȡ�ļ�����
    std::string content(size, '\0');
    if (!file.read(&content[0], size)) {
        throw std::runtime_error("��ȡ�ļ�ʧ��: " + filePath);
    }
    
    file.close();
    return content;
}

void writeResultToFile(const std::string& outputPath, double similarity) {
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        throw std::runtime_error("�޷���������ļ�: " + outputPath);
    }
    
    // ���������λС�������ƶ�
    outFile << std::fixed << std::setprecision(2) << similarity;
    outFile.close();
}