#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

// 读取文件内容
std::string readFileContent(const std::string& filePath);

// 计算两个字符串的最长公共子序列长度
int computeLCSLength(const std::string& text1, const std::string& text2);

// 计算相似度
double calculateSimilarity(int lcsLength, int len1, int len2);

// 将结果写入文件
void writeResultToFile(const std::string& outputPath, double similarity);

// 打印程序使用说明
void printUsage(const char* programName);

#endif 