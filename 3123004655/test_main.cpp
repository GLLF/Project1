#include "main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

// ���������ṹ��
struct TestCase {
    std::string name;
    std::string original;
    std::string plagiarized;
    double expected_min;
    double expected_max;
    std::string description;
    bool should_pass; // �����������Ƿ�Ӧ��ͨ��
    std::vector<std::string> command_args; // �����������в���
};

// ���Խ���ṹ��
struct TestResult {
    std::string name;
    bool passed;
    double expected_min;
    double expected_max;
    double actual;
    std::string message;
    int exit_code; // �������˳���
};

// ���������ļ�
void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
}

// ���������в���
TestResult runCommandLineTest(const TestCase& testCase) {
    TestResult result;
    result.name = testCase.name;
    result.expected_min = testCase.expected_min;
    result.expected_max = testCase.expected_max;
    
    try {
        // ��������
        std::string command = "main.exe";
        for (const auto& arg : testCase.command_args) {
            command += " " + arg;
        }
        
        // ִ�������ȡ�˳���
        int exit_code = std::system(command.c_str());
        result.exit_code = exit_code;
        
        // ����˳���
        if (testCase.should_pass) {
            // Ӧ��ͨ���Ĳ��ԣ��˳���Ϊ0
            result.passed = (exit_code == 0);
            result.message = "�˳���: " + std::to_string(exit_code);
        } else {
            // Ӧ��ʧ�ܵĲ��ԣ��˳����0
            result.passed = (exit_code != 0);
            result.message = "�˳���: " + std::to_string(exit_code) + " (��������)";
        }
        
        result.actual = -1.0; // �����в��Բ��������ƶ�
        
    } catch (const std::exception& e) {
        result.passed = false;
        result.actual = -1.0;
        result.message = "�쳣 - " + std::string(e.what());
        result.exit_code = -1;
    }
    
    return result;
}

// ���������Ĺ��ܲ���
TestResult runFunctionalTest(const TestCase& testCase) {
    TestResult result;
    result.name = testCase.name;
    result.expected_min = testCase.expected_min;
    result.expected_max = testCase.expected_max;
    
    try {
        // ���������ļ�
        createTestFile("test_original.txt", testCase.original);
        createTestFile("test_plagiarized.txt", testCase.plagiarized);
        
        // ��ȡ�ļ�����
        std::string originalText = readFileContent("test_original.txt");
        std::string plagiarizedText = readFileContent("test_plagiarized.txt");
        
        // ����LCS����
        int lcsLen = computeLCSLength(originalText, plagiarizedText);
        
        // �������ƶ�
        result.actual = calculateSimilarity(lcsLen, originalText.length(), plagiarizedText.length());
        
        // ������Ƿ���Ԥ�ڷ�Χ��
        if (result.actual >= testCase.expected_min && result.actual <= testCase.expected_max) {
            result.passed = true;
            result.message = " ʵ��ֵ: " + std::to_string(result.actual);
        } else {
            result.passed = false;
            result.message = " ������Χ: [" + 
                           std::to_string(testCase.expected_min) + ", " + 
                           std::to_string(testCase.expected_max) + "], ʵ��ֵ: " + 
                           std::to_string(result.actual);
        }
        
        result.exit_code = 0;
        
        // ��������ļ�
        remove("test_original.txt");
        remove("test_plagiarized.txt");
        remove("test_result.txt");
        
    } catch (const std::exception& e) {
        result.passed = false;
        result.actual = -1.0;
        result.message = "�쳣 - " + std::string(e.what());
        result.exit_code = -1;
    }
    
    return result;
}

// ��ӡ���Խ��
void printTestResult(const TestResult& result, int index) {
    std::cout << "���� " << index << ": " << result.name << " - ";
    if (result.passed) {
        std::cout << "\033[32m ͨ�� \033[0m";
    } else {
        std::cout << "\033[33m ʧ�� \033[0m";
    }
    std::cout << " - " << result.message;
    
    if (result.exit_code >= 0) {
        std::cout << " [�˳���: " << result.exit_code << "]";
    }
    
    std::cout << std::endl;
}

// �����Ժ���
int main() {
    std::cout << "��ʼ���Ĳ���ϵͳ��Ԫ���ԣ���������������ԣ�..." << std::endl;
    std::cout << "==========================================" << std::endl;
    
    // ����һЩ���������ļ�
    createTestFile("original.txt", "����ԭ�����ݡ�");
    createTestFile("plagiarized.txt", "���ǳ�Ϯ�����ݡ�");
    
    // �����������
    std::vector<TestCase> testCases = {
        {
            "��ͨ���ı������ģ�",
            "��Ȼ���Դ������˹����������е���Ҫ��֧�����������о��������������Ȼ����֮��Ľ�����ͨ������ѧϰ�㷨�������������⡢���ͺ������������ԣ��������ܿͷ��������������з����������й㷺Ӧ�á����ѧϰ�����ķ�չ��һ���ƶ�����Ȼ���Դ���Ľ�����",
            "��Ȼ���Դ�����Ϊ�˹����ܵĹؼ���֧��רע���о��������������Ȼ����֮��Ļ�������������ѧϰ������������ܹ���⡢�����ʹ����������ԣ�����������ܿͷ��������������з����ȳ�����Ӧ�ù㷺�����ѧϰ�ķ�չ����شٽ�����Ȼ���Դ����������",
            0.70, 0.80,
            "�������ĳ��ı������ƶȼ���"
        },
        
        // ����2����ͨ���ı���Ӣ�ģ�
        {
            "��ͨ���ı���Ӣ�ģ�",
            "Machine learning is a subset of artificial intelligence that focuses on the development of algorithms and statistical models that enable computers to improve their performance on a specific task through experience. It is widely used in various applications such as image recognition, natural language processing, and predictive analytics. Deep learning, a specialized form of machine learning, has achieved remarkable success in recent years.",
            "Machine learning, as a branch of artificial intelligence, concentrates on creating algorithms and statistical models that allow computers to enhance their capabilities on particular tasks via experience. This technology finds extensive use in areas like image recognition, NLP, and predictive analysis. Deep learning, an advanced type of machine learning, has shown significant achievements in the past few years.",
            0.60, 0.70,
            "����Ӣ�ĳ��ı������ƶȼ���"
        },
        
        // ����3�����ı�
        {
            "���ı�",
            "����������á�",
            "���������ܲ���",
            0.70, 0.80,
            "���Զ��ı������ƶȼ���"
        },
        
        // ����4�����ı�
        {
            "���ı�",
            "",
            "",
            1.00, 1.00,
            "�����������ı������ƶȼ���"
        },
        
        // ����5����ȫ��ͬ���ı�
        {
            "��ȫ��ͬ���ı�",
            "����������ͨ��ȥ���Ļ��ķ�ʽȷ�����ݵİ�ȫ�ԺͲ��ɴ۸��ԡ�",
            "����������ͨ��ȥ���Ļ��ķ�ʽȷ�����ݵİ�ȫ�ԺͲ��ɴ۸��ԡ�",
            1.00, 1.00,
            "������ȫ��ͬ���ı������ƶȼ���"
        },
        
        // ����6����ȫ��ͬ���ı�
        {
            "��ȫ��ͬ���ı�",
            "�������ˣ���԰��Ļ����������ˣ������ڻ����з��衣",
            "���Ӽ�����������ӱ��ؽ��в��м��㣬���г�Խ��ͳ�������Ǳ����",
            0.00, 0.10,
            "������ȫ��ͬ���ı������ƶȼ���"
        },
        
        // ����7���������Ų�ͬ
        {
            "�������Ų�ͬ",
            "�˹����ܼ������ڿ��ٷ�չ�����ı������ǵ����ʽ��������ʽ������˼ά��ʽ��",
            "�˹����ܼ������ڿ��ٷ�չ�����ı������ǵ����ʽ��������ʽ������˼ά��ʽ��",
            0.90, 0.95,
            "���Խ������Ų�ͬ���ı������ƶȼ���"
        },
        
        // ����8���������
        {
            "�������",
            "���ѧϰ�ǻ���ѧϰ��һ����֧���������˹������硣",
            "�����˹������磬���ѧϰ�ǻ���ѧϰ��һ����֧��",
            0.70, 0.80,
            "���Դ���������ı������ƶȼ���"
        },
        
        // ����9���������ݳ�Ϯ
        {
            "�������ݳ�Ϯ",
            "�Ƽ����ṩ�˰�����ʹ��������Դ���������������������洢��Ӧ�ó�������ģʽ�������Դ�����ʣ������˳ɱ������ṩ�˸��������ԡ���ҵ���Ը���ҵ�����������չ��������Դ��",
            "��Ե���㽫������Դ����������Դ�����������������ӳ١�����ģʽ�������Ӧ�ٶȣ������˴������ģ����ṩ�˸��õ�������˽����ҵ���Ը���ҵ��������ٲ������ڵ㡣�Ƽ����ṩ�˰�����ʹ��������Դ���������������������洢��Ӧ�ó���",
            0.40, 0.50,
            "���Բ������ݳ�Ϯ���ı������ƶȼ���"
        },
        
        // ����10�������Ի��
        {
            "�����Ի��",
            "Python��һ�ָ߼�������ԣ���Guido van Rossum��1991�괴�������������ѧǿ������Ŀɶ��ԣ��﷨�������Ա�ø��ٵĴ����б����Python supports multiple programming paradigms, including object-oriented, imperative and functional programming.",
            "Python��һ�ָ߼�������ԣ�Guido van Rossum��1991�괴�������������Ե����ע�ش���ɶ��ԣ����﷨�ÿ��������ø����Ĵ���ʵ�ֹ��ܡ�Python֧�ֶ��ֱ�̷�ʽ���������������ʽ�ͺ���ʽ��̡�",
            0.55, 0.65,
            "���Զ����Ի���ı������ƶȼ���"
        },
        
        // ����11�������ַ��͸�ʽ
        {
            "�����ַ��͸�ʽ",
            "���ݿ����ϵͳ��DBMS�������ڹ������ݿ�����ϵͳ��������DBMS����MySQL��Oracle��SQL Server��SQL��Structured Query Language�������������ݿ�ͨ�ŵı�׼���ԡ�SELECT * FROM users WHERE age > 18;",
            "���ݿ����ϵͳ(DBMS)�������������ݿ����������õ�DBMS��MySQL��Oracle��SQL Server��SQL(�ṹ����ѯ����)�������ݿ⽻���ı�׼���ԡ�SELECT * FROM users WHERE age > 18;",
            0.75, 0.85,
            "���԰��������ַ��͸�ʽ���ı������ƶȼ���"
        },
        
        // ����12�������ı���ѹ�����ԣ�
        {
            "�����ı���ѹ�����ԣ�",
            "�˹����ܵķ�չ���̿���׷�ݵ�20����50�������ʱ����ѧ���ǿ�ʼ̽���ܷ��û���ģ�������������Ϊ�����ڵ�AI�о���Ҫ�������������ͷ��Ŵ������ϡ�1956�꣬Լ���������ڴ���é˹�������״������\"�˹�����\"��һ�����־��AI��Ϊһ�������о�����ĵ����������ļ�ʮ���AI�����˶�η�����͹ȡ�ר��ϵͳ��80���ȡ������ҵ�ɹ����������ڼ�����������������AI�ķ�չ��90�������͹��ڡ�ֱ��21���ͳ������ż��������������ʹ����ݵĳ��֣�����ѧϰ�ر������ѧϰ����ȡ����ͻ���Խ�չ��2012�꣬AlexNet��ImageNet�����е�׿Խ���������˹㷺��ע�����������ѧϰ����ʱ�������AI�����ѹ㷺Ӧ����ͼ��ʶ����Ȼ���Դ����Զ���ʻ�ȶ�����򣬶�������������ʽ��������ԶӰ�졣δ���������㷨�Ĳ����Ż��ͼ�����Դ�Ľ�һ���������˹����������ڸ������򷢻���Ҫ���á�",
            "�˹����ܵ��ݽ���ʷ���Ի��ݵ�20�������ڡ���ʱ���о���Ա��ʼ�����û���ģ����������ܻ�����ڵ�AI�о����������������ͷ��Ŵ�������1956�꣬Լ���������ڴ���é˹�����ϵ�һ�����\"�˹�����\"���������־��AI��Ϊһ������ѧ�ơ��ڽ������ļ�ʮ���У�AI��չ�����˶�θ߳��͵͹ȡ�ר��ϵͳ��1980���ʵ������ҵ�ϵĳɹ��������ڼ������������ݹ�ģ�����ƣ�AI��1990��������˷�չ�����ڡ�ֱ��21���ͳ��ڣ����ż�����������ǿ�ͺ������ݵĿɻ���ԣ�����ѧϰ���������ѧϰ����ʵ�����ش�ͻ�ơ�2012�꣬AlexNet��ImageNet�����еĳ�ɫ����������ȫ��Ŀ�⣬���������ѧϰ�Ļƽ�ʱ������ǰ��AI�����Ѿ����㷺������ͼ��ʶ����Ȼ������⡢�Զ���ʻ���ڶ�������̸ı�������������ճ�����ģʽ��չ��δ���������㷨�ĳ����Ľ��ͼ�����Դ�Ľ�һ���ḻ���˹����������ڸ�����ҵչ�����ֵ��",
            0.70, 0.80,
            "���Գ����ı������ƶȼ��������"
        },
        
        // �����Ĳ����������������13-18��
        {
            "�������ٲ���",
            "", "", 0, 0, 
            "���������в���������������",
            false, // Ӧ��ʧ��
            {"original.txt"} // ֻ��һ������
        },
        {
            "�����������", 
            "", "", 0, 0,
            "���������в���������������",
            false, // Ӧ��ʧ��
            {"original.txt", "plagiarized.txt", "result.txt", "extra_param.txt"}
        },
        {
            "ԭ���ļ������ڲ���",
            "", "", 0, 0,
            "����ԭ���ļ�·�������ڵ����", 
            false, // Ӧ��ʧ��
            {"non_existent_original.txt", "plagiarized.txt", "result.txt"}
        },
        {
            "��Ϯ���ļ������ڲ���",
            "", "", 0, 0, 
            "���Գ�Ϯ���ļ�·�������ڵ����",
            false, // Ӧ��ʧ��
            {"original.txt", "non_existent_plagiarized.txt", "result.txt"}
        },
        {
            "���ո��ļ�·������",
            "����A", "����B", 0.4, 0.6,
            "�����ļ�·�������ո�������³���Բ��ԣ�",
            true, // ����ͨ����ȡ����ʵ��
            {"original with space.txt", "plagiarized with space.txt", "result with space.txt"}
        }
    };
    
    // Ϊ���ո��ļ�·�����Դ����ļ�
    createTestFile("original with space.txt", "���Ǵ��ո�·����ԭ�ġ�");
    createTestFile("plagiarized with space.txt", "���Ǵ��ո�·���ĳ�Ϯ�档");
    
    // �������в�������
    int passed = 0;
    int total = testCases.size();
    
    for (size_t i = 0; i < testCases.size(); ++i) {
        TestResult result;
        
        if (testCases[i].command_args.empty()) {
            // ���ܲ���
            result = runFunctionalTest(testCases[i]);
        } else {
            // �����в���
            result = runCommandLineTest(testCases[i]);
        }
        
        printTestResult(result, i + 1);
        
        if (result.passed) {
            passed++;
        }
        
        // ��Ӽ����ʹ���������

        std::cout << std::endl;
        
    }
    
    // ��������ļ�
    remove("original.txt");
    remove("plagiarized.txt"); 
    remove("original with space.txt");
    remove("plagiarized with space.txt");
    remove("result with space.txt");
    
    // ��������ܽ�
    std::cout << "==========================================" << std::endl;
    std::cout << "�����ܽ�: " << passed << "/" << total << " ͨ��" << std::endl;
    
    if (passed == total) {
        std::cout << " ���в�������ͨ����" << std::endl;
    } else {
        std::cout << " ���ֲ�������ʧ�ܣ�" << std::endl;
        
    }
    
    return (passed == total) ? 0 : 1;
}