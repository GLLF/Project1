#include "main.h"

double calculateSimilarity(int lcsLength, int len1, int len2) {
    if (len1 == 0 && len2 == 0) return 1.0;
    if (len1 == 0 || len2 == 0) return 0.0;
    
    // 使用公式: 相似度 = (2 * LCS长度) / (原文长度 + 抄袭文长度)
    return (2.0 * lcsLength) / (len1 + len2);
}

void printUsage(const char* programName) {
    std::cerr << "使用方法: " << programName << " [原文文件] [抄袭版论文文件] [答案文件]" << std::endl;
    std::cerr << "示例: " << programName << " C:\\original.txt C:\\plagiarized.txt C:\\result.txt" << std::endl;
}