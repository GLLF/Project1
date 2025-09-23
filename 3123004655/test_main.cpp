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

// 测试用例结构体
struct TestCase {
    std::string name;
    std::string original;
    std::string plagiarized;
    double expected_min;
    double expected_max;
    std::string description;
    bool should_pass; // 新增：测试是否应该通过
    std::vector<std::string> command_args; // 新增：命令行参数
};

// 测试结果结构体
struct TestResult {
    std::string name;
    bool passed;
    double expected_min;
    double expected_max;
    double actual;
    std::string message;
    int exit_code; // 新增：退出码
};

// 创建测试文件
void createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
}

// 运行命令行测试
TestResult runCommandLineTest(const TestCase& testCase) {
    TestResult result;
    result.name = testCase.name;
    result.expected_min = testCase.expected_min;
    result.expected_max = testCase.expected_max;
    
    try {
        // 构建命令
        std::string command = "main.exe";
        for (const auto& arg : testCase.command_args) {
            command += " " + arg;
        }
        
        // 执行命令并获取退出码
        int exit_code = std::system(command.c_str());
        result.exit_code = exit_code;
        
        // 检查退出码
        if (testCase.should_pass) {
            // 应该通过的测试：退出码为0
            result.passed = (exit_code == 0);
            result.message = "退出码: " + std::to_string(exit_code);
        } else {
            // 应该失败的测试：退出码非0
            result.passed = (exit_code != 0);
            result.message = "退出码: " + std::to_string(exit_code) + " (期望非零)";
        }
        
        result.actual = -1.0; // 命令行测试不计算相似度
        
    } catch (const std::exception& e) {
        result.passed = false;
        result.actual = -1.0;
        result.message = "异常 - " + std::string(e.what());
        result.exit_code = -1;
    }
    
    return result;
}

// 运行正常的功能测试
TestResult runFunctionalTest(const TestCase& testCase) {
    TestResult result;
    result.name = testCase.name;
    result.expected_min = testCase.expected_min;
    result.expected_max = testCase.expected_max;
    
    try {
        // 创建测试文件
        createTestFile("test_original.txt", testCase.original);
        createTestFile("test_plagiarized.txt", testCase.plagiarized);
        
        // 读取文件内容
        std::string originalText = readFileContent("test_original.txt");
        std::string plagiarizedText = readFileContent("test_plagiarized.txt");
        
        // 计算LCS长度
        int lcsLen = computeLCSLength(originalText, plagiarizedText);
        
        // 计算相似度
        result.actual = calculateSimilarity(lcsLen, originalText.length(), plagiarizedText.length());
        
        // 检查结果是否在预期范围内
        if (result.actual >= testCase.expected_min && result.actual <= testCase.expected_max) {
            result.passed = true;
            result.message = " 实际值: " + std::to_string(result.actual);
        } else {
            result.passed = false;
            result.message = " 期望范围: [" + 
                           std::to_string(testCase.expected_min) + ", " + 
                           std::to_string(testCase.expected_max) + "], 实际值: " + 
                           std::to_string(result.actual);
        }
        
        result.exit_code = 0;
        
        // 清理测试文件
        remove("test_original.txt");
        remove("test_plagiarized.txt");
        remove("test_result.txt");
        
    } catch (const std::exception& e) {
        result.passed = false;
        result.actual = -1.0;
        result.message = "异常 - " + std::string(e.what());
        result.exit_code = -1;
    }
    
    return result;
}

// 打印测试结果
void printTestResult(const TestResult& result, int index) {
    std::cout << "测试 " << index << ": " << result.name << " - ";
    if (result.passed) {
        std::cout << "\033[32m 通过 \033[0m";
    } else {
        std::cout << "\033[33m 失败 \033[0m";
    }
    std::cout << " - " << result.message;
    
    if (result.exit_code >= 0) {
        std::cout << " [退出码: " << result.exit_code << "]";
    }
    
    std::cout << std::endl;
}

// 主测试函数
int main() {
    std::cout << "开始论文查重系统单元测试（包含参数错误测试）..." << std::endl;
    std::cout << "==========================================" << std::endl;
    
    // 创建一些基础测试文件
    createTestFile("original.txt", "这是原文内容。");
    createTestFile("plagiarized.txt", "这是抄袭版内容。");
    
    // 定义测试用例
    std::vector<TestCase> testCases = {
        {
            "普通长文本（中文）",
            "自然语言处理是人工智能领域中的重要分支，它致力于研究计算机与人类自然语言之间的交互。通过机器学习算法，计算机可以理解、解释和生成人类语言，这在智能客服、机器翻译和情感分析等领域有广泛应用。深度学习技术的发展进一步推动了自然语言处理的进步。",
            "自然语言处理作为人工智能的关键分支，专注于研究计算机与人类自然语言之间的互动。借助机器学习方法，计算机能够理解、解析和创造人类语言，这项技术在智能客服、机器翻译和情感分析等场景中应用广泛。深度学习的发展极大地促进了自然语言处理的提升。",
            0.70, 0.80,
            "测试中文长文本的相似度计算"
        },
        
        // 样例2：普通长文本（英文）
        {
            "普通长文本（英文）",
            "Machine learning is a subset of artificial intelligence that focuses on the development of algorithms and statistical models that enable computers to improve their performance on a specific task through experience. It is widely used in various applications such as image recognition, natural language processing, and predictive analytics. Deep learning, a specialized form of machine learning, has achieved remarkable success in recent years.",
            "Machine learning, as a branch of artificial intelligence, concentrates on creating algorithms and statistical models that allow computers to enhance their capabilities on particular tasks via experience. This technology finds extensive use in areas like image recognition, NLP, and predictive analysis. Deep learning, an advanced type of machine learning, has shown significant achievements in the past few years.",
            0.60, 0.70,
            "测试英文长文本的相似度计算"
        },
        
        // 样例3：短文本
        {
            "短文本",
            "今天天气真好。",
            "今天天气很不错。",
            0.70, 0.80,
            "测试短文本的相似度计算"
        },
        
        // 样例4：空文本
        {
            "空文本",
            "",
            "",
            1.00, 1.00,
            "测试两个空文本的相似度计算"
        },
        
        // 样例5：完全相同的文本
        {
            "完全相同的文本",
            "区块链技术通过去中心化的方式确保数据的安全性和不可篡改性。",
            "区块链技术通过去中心化的方式确保数据的安全性和不可篡改性。",
            1.00, 1.00,
            "测试完全相同的文本的相似度计算"
        },
        
        // 样例6：完全不同的文本
        {
            "完全不同的文本",
            "春天来了，花园里的花儿都开放了，蝴蝶在花丛中飞舞。",
            "量子计算机利用量子比特进行并行计算，具有超越传统计算机的潜力。",
            0.00, 0.10,
            "测试完全不同的文本的相似度计算"
        },
        
        // 样例7：仅标点符号不同
        {
            "仅标点符号不同",
            "人工智能技术正在快速发展；它改变了我们的生活方式，工作方式，甚至思维方式！",
            "人工智能技术正在快速发展，它改变了我们的生活方式、工作方式、甚至思维方式。",
            0.90, 0.95,
            "测试仅标点符号不同的文本的相似度计算"
        },
        
        // 样例8：词序调换
        {
            "词序调换",
            "深度学习是机器学习的一个分支，它基于人工神经网络。",
            "基于人工神经网络，深度学习是机器学习的一个分支。",
            0.70, 0.80,
            "测试词序调换的文本的相似度计算"
        },
        
        // 样例9：部分内容抄袭
        {
            "部分内容抄袭",
            "云计算提供了按需访问共享计算资源的能力，包括服务器、存储和应用程序。这种模式提高了资源利用率，降低了成本，并提供了更大的灵活性。企业可以根据业务需求快速扩展或缩减资源。",
            "边缘计算将计算资源部署在数据源附近，减少了网络延迟。这种模式提高了响应速度，降低了带宽消耗，并提供了更好的数据隐私。企业可以根据业务需求快速部署计算节点。云计算提供了按需访问共享计算资源的能力，包括服务器、存储和应用程序。",
            0.40, 0.50,
            "测试部分内容抄袭的文本的相似度计算"
        },
        
        // 样例10：多语言混合
        {
            "多语言混合",
            "Python是一种高级编程语言，由Guido van Rossum于1991年创建。它的设计哲学强调代码的可读性，语法允许程序员用更少的代码行表达概念。Python supports multiple programming paradigms, including object-oriented, imperative and functional programming.",
            "Python是一种高级编程语言，Guido van Rossum在1991年创造了它。该语言的设计注重代码可读性，其语法让开发者能用更简洁的代码实现功能。Python支持多种编程范式，如面向对象、命令式和函数式编程。",
            0.55, 0.65,
            "测试多语言混合文本的相似度计算"
        },
        
        // 样例11：特殊字符和格式
        {
            "特殊字符和格式",
            "数据库管理系统（DBMS）是用于管理数据库的软件系统。常见的DBMS包括MySQL、Oracle和SQL Server。SQL（Structured Query Language）是用于与数据库通信的标准语言。SELECT * FROM users WHERE age > 18;",
            "数据库管理系统(DBMS)是用来管理数据库的软件。常用的DBMS有MySQL、Oracle和SQL Server。SQL(结构化查询语言)是与数据库交互的标准语言。SELECT * FROM users WHERE age > 18;",
            0.75, 0.85,
            "测试包含特殊字符和格式的文本的相似度计算"
        },
        
        // 样例12：超长文本（压力测试）
        {
            "超长文本（压力测试）",
            "人工智能的发展历程可以追溯到20世纪50年代。当时，科学家们开始探索能否让机器模拟人类的智能行为。早期的AI研究主要集中在问题解决和符号处理方法上。1956年，约翰·麦卡锡在达特茅斯会议上首次提出了\"人工智能\"这一术语，标志着AI作为一个独立研究领域的诞生。在随后的几十年里，AI经历了多次繁荣与低谷。专家系统在80年代取得了商业成功，但受限于计算能力和数据量，AI的发展在90年代进入低谷期。直到21世纪初，随着计算能力的提升和大数据的出现，机器学习特别是深度学习技术取得了突破性进展。2012年，AlexNet在ImageNet竞赛中的卓越表现引起了广泛关注，开启了深度学习的新时代。如今，AI技术已广泛应用于图像识别、自然语言处理、自动驾驶等多个领域，对社会生产和生活方式产生了深远影响。未来，随着算法的不断优化和计算资源的进一步增长，人工智能有望在更多领域发挥重要作用。",
            "人工智能的演进历史可以回溯到20世纪中期。那时，研究人员开始尝试让机器模仿人类的智能活动。初期的AI研究焦点在于问题求解和符号处理技术。1956年，约翰·麦卡锡在达特茅斯会议上第一次提出\"人工智能\"这个概念，这标志着AI成为一个独立学科。在接下来的几十年中，AI发展经历了多次高潮和低谷。专家系统在1980年代实现了商业上的成功，但由于计算能力和数据规模的限制，AI在1990年代进入了发展缓慢期。直到21世纪初期，随着计算能力的增强和海量数据的可获得性，机器学习尤其是深度学习技术实现了重大突破。2012年，AlexNet在ImageNet比赛中的出色表现吸引了全球目光，开启了深度学习的黄金时代。当前，AI技术已经被广泛运用于图像识别、自然语言理解、自动驾驶等众多领域，深刻改变了社会生产和日常生活模式。展望未来，随着算法的持续改进和计算资源的进一步丰富，人工智能有望在更多行业展现其价值。",
            0.70, 0.80,
            "测试超长文本的相似度计算和性能"
        },
        
        // 新增的参数错误测试用例（13-18）
        {
            "参数过少测试",
            "", "", 0, 0, 
            "测试命令行参数数量不足的情况",
            false, // 应该失败
            {"original.txt"} // 只有一个参数
        },
        {
            "参数过多测试", 
            "", "", 0, 0,
            "测试命令行参数数量过多的情况",
            false, // 应该失败
            {"original.txt", "plagiarized.txt", "result.txt", "extra_param.txt"}
        },
        {
            "原文文件不存在测试",
            "", "", 0, 0,
            "测试原文文件路径不存在的情况", 
            false, // 应该失败
            {"non_existent_original.txt", "plagiarized.txt", "result.txt"}
        },
        {
            "抄袭版文件不存在测试",
            "", "", 0, 0, 
            "测试抄袭版文件路径不存在的情况",
            false, // 应该失败
            {"original.txt", "non_existent_plagiarized.txt", "result.txt"}
        },
        {
            "带空格文件路径测试",
            "内容A", "内容B", 0.4, 0.6,
            "测试文件路径包含空格的情况（鲁棒性测试）",
            true, // 可能通过，取决于实现
            {"original with space.txt", "plagiarized with space.txt", "result with space.txt"}
        }
    };
    
    // 为带空格文件路径测试创建文件
    createTestFile("original with space.txt", "这是带空格路径的原文。");
    createTestFile("plagiarized with space.txt", "这是带空格路径的抄袭版。");
    
    // 运行所有测试用例
    int passed = 0;
    int total = testCases.size();
    
    for (size_t i = 0; i < testCases.size(); ++i) {
        TestResult result;
        
        if (testCases[i].command_args.empty()) {
            // 功能测试
            result = runFunctionalTest(testCases[i]);
        } else {
            // 命令行测试
            result = runCommandLineTest(testCases[i]);
        }
        
        printTestResult(result, i + 1);
        
        if (result.passed) {
            passed++;
        }
        
        // 添加间隔，使输出更清晰

        std::cout << std::endl;
        
    }
    
    // 清理测试文件
    remove("original.txt");
    remove("plagiarized.txt"); 
    remove("original with space.txt");
    remove("plagiarized with space.txt");
    remove("result with space.txt");
    
    // 输出测试总结
    std::cout << "==========================================" << std::endl;
    std::cout << "测试总结: " << passed << "/" << total << " 通过" << std::endl;
    
    if (passed == total) {
        std::cout << " 所有测试用例通过！" << std::endl;
    } else {
        std::cout << " 部分测试用例失败！" << std::endl;
        
    }
    
    return (passed == total) ? 0 : 1;
}