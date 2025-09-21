#include "main.h"
#include <iostream>
#include <sstream>

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

// 新增函数：生成详细的相似度报告
std::string generateSimilarityReport(int lcsLength, int len1, int len2, double similarity) {
    std::stringstream report;
    
    report << "论文查重分析报告" << std::endl;
    report << "====================" << std::endl;
    report << "原文长度: " << len1 << " 字符" << std::endl;
    report << "待检测文本长度: " << len2 << " 字符" << std::endl;
    report << "最长公共子序列长度: " << lcsLength << " 字符" << std::endl;
    report << "-------------------" << std::endl;
    report << "相似度计算结果: " << std::fixed << std::setprecision(2) << similarity << std::endl;
    report << "-------------------" << std::endl;
    
    // 根据相似度值提供不同的解释
    if (similarity >= 0.9) {
        report << "分析: 文本高度相似，可能存在抄袭行为" << std::endl;
    } else if (similarity >= 0.7) {
        report << "分析: 文本相似度较高，建议进一步检查" << std::endl;
    } else if (similarity >= 0.5) {
        report << "分析: 文本有部分相似内容" << std::endl;
    } else if (similarity >= 0.3) {
        report << "分析: 文本相似度较低" << std::endl;
    } else {
        report << "分析: 文本基本不相似" << std::endl;
    }
    
    report << "====================" << std::endl;
    report << "注: 相似度基于最长公共子序列(LCS)算法计算" << std::endl;
    
    return report.str();
}