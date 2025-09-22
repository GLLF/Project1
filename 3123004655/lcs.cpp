#include "main.h"

int computeLCSLength(const std::string& text1, const std::string& text2) {
    int m = text1.length();
    int n = text2.length();
    
    // 使用二维向量存储DP表
    std::vector<std::vector<int>> dp(2, std::vector<int>(n + 1, 0));
    
    // 填充DP表
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i%2][j] = dp[(i - 1)%2][j - 1] + 1;
            } else {
                dp[i%2][j] = std::max(dp[(i - 1)%2][j], dp[i%2][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}