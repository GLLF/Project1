#include "main.h"

int main(int argc, char* argv[]) {
    // 检查命令行参数数量
    if (argc != 4) {
        std::cerr << "错误: 参数数量不正确。" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::string originalFilePath = argv[1];
    std::string plagiarizedFilePath = argv[2];
    std::string outputFilePath = argv[3];

    try {
        // 读取文件内容
        std::string originalText = readFileContent(originalFilePath);
        std::string plagiarizedText = readFileContent(plagiarizedFilePath);
        
        // 计算LCS长度
        int lcsLen = computeLCSLength(originalText, plagiarizedText);
        
        // 计算相似度
        double similarity = calculateSimilarity(lcsLen, originalText.length(), plagiarizedText.length());
        
        // 写入结果
        writeResultToFile(outputFilePath, similarity);
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}