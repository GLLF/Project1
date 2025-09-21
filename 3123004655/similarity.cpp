#include "main.h"
#include <iostream>
#include <sstream>

double calculateSimilarity(int lcsLength, int len1, int len2) {
    if (len1 == 0 && len2 == 0) return 1.0;
    if (len1 == 0 || len2 == 0) return 0.0;
    
    // ʹ�ù�ʽ: ���ƶ� = (2 * LCS����) / (ԭ�ĳ��� + ��Ϯ�ĳ���)
    return (2.0 * lcsLength) / (len1 + len2);
}

void printUsage(const char* programName) {
    std::cerr << "ʹ�÷���: " << programName << " [ԭ���ļ�] [��Ϯ�������ļ�] [���ļ�]" << std::endl;
    std::cerr << "ʾ��: " << programName << " C:\\original.txt C:\\plagiarized.txt C:\\result.txt" << std::endl;
}

// ����������������ϸ�����ƶȱ���
std::string generateSimilarityReport(int lcsLength, int len1, int len2, double similarity) {
    std::stringstream report;
    
    report << "���Ĳ��ط�������" << std::endl;
    report << "====================" << std::endl;
    report << "ԭ�ĳ���: " << len1 << " �ַ�" << std::endl;
    report << "������ı�����: " << len2 << " �ַ�" << std::endl;
    report << "����������г���: " << lcsLength << " �ַ�" << std::endl;
    report << "-------------------" << std::endl;
    report << "���ƶȼ�����: " << std::fixed << std::setprecision(2) << similarity << std::endl;
    report << "-------------------" << std::endl;
    
    // �������ƶ�ֵ�ṩ��ͬ�Ľ���
    if (similarity >= 0.9) {
        report << "����: �ı��߶����ƣ����ܴ��ڳ�Ϯ��Ϊ" << std::endl;
    } else if (similarity >= 0.7) {
        report << "����: �ı����ƶȽϸߣ������һ�����" << std::endl;
    } else if (similarity >= 0.5) {
        report << "����: �ı��в�����������" << std::endl;
    } else if (similarity >= 0.3) {
        report << "����: �ı����ƶȽϵ�" << std::endl;
    } else {
        report << "����: �ı�����������" << std::endl;
    }
    
    report << "====================" << std::endl;
    report << "ע: ���ƶȻ��������������(LCS)�㷨����" << std::endl;
    
    return report.str();
}