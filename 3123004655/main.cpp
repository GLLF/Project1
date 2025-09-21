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
        std::cout << "正在读取原文文件..." << std::endl;
        std::string originalText = readFileContent(originalFilePath);
        std::cout << "原文文件读取成功，长度: " << originalText.length() << " 字符" << std::endl;
        
        std::cout << "正在读取抄袭版论文文件..." << std::endl;
        std::string plagiarizedText = readFileContent(plagiarizedFilePath);
        std::cout << "抄袭版论文文件读取成功，长度: " << plagiarizedText.length() << " 字符" << std::endl;
        
        // 计算LCS长度
        std::cout << "正在计算最长公共子序列..." << std::endl;
        int lcsLen = computeLCSLength(originalText, plagiarizedText);
        std::cout << "最长公共子序列长度: " << lcsLen << " 字符" << std::endl;
        
        // 计算相似度
        double similarity = calculateSimilarity(lcsLen, originalText.length(), plagiarizedText.length());
        
        // 生成并显示详细报告
        std::string report = generateSimilarityReport(lcsLen, originalText.length(), 
                                                     plagiarizedText.length(), similarity);
        std::cout << std::endl << report << std::endl;
        
        // 写入结果（只写入纯数字，符合测试要求）
        std::cout << "正在将结果写入答案文件..." << std::endl;
        writeResultToFile(outputFilePath, similarity);
        std::cout << "结果已写入答案文件: " << outputFilePath << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}