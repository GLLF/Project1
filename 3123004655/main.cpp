#include "main.h"

int main(int argc, char* argv[]) {
    // ��������в�������
    if (argc != 4) {
        std::cerr << "����: ������������ȷ��" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    std::string originalFilePath = argv[1];
    std::string plagiarizedFilePath = argv[2];
    std::string outputFilePath = argv[3];

    try {
        // ��ȡ�ļ�����
        std::cout << "���ڶ�ȡԭ���ļ�..." << std::endl;
        std::string originalText = readFileContent(originalFilePath);
        std::cout << "ԭ���ļ���ȡ�ɹ�������: " << originalText.length() << " �ַ�" << std::endl;
        
        std::cout << "���ڶ�ȡ��Ϯ�������ļ�..." << std::endl;
        std::string plagiarizedText = readFileContent(plagiarizedFilePath);
        std::cout << "��Ϯ�������ļ���ȡ�ɹ�������: " << plagiarizedText.length() << " �ַ�" << std::endl;
        
        // ����LCS����
        std::cout << "���ڼ��������������..." << std::endl;
        int lcsLen = computeLCSLength(originalText, plagiarizedText);
        std::cout << "����������г���: " << lcsLen << " �ַ�" << std::endl;
        
        // �������ƶ�
        double similarity = calculateSimilarity(lcsLen, originalText.length(), plagiarizedText.length());
        
        // ���ɲ���ʾ��ϸ����
        std::string report = generateSimilarityReport(lcsLen, originalText.length(), 
                                                     plagiarizedText.length(), similarity);
        std::cout << std::endl << report << std::endl;
        
        // д������ֻд�봿���֣����ϲ���Ҫ��
        std::cout << "���ڽ����д����ļ�..." << std::endl;
        writeResultToFile(outputFilePath, similarity);
        std::cout << "�����д����ļ�: " << outputFilePath << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "����: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}