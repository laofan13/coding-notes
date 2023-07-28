// https://leetcode.cn/problems/number-of-digit-one/description/

#include <vector>
#include <cstdio>
#include <cmath>

using namespace std;

class Solution {
public:
    int digit_len = 11;

    int countDigitOne(int n) {
        vector<int64_t> dp(digit_len);
        dp[0] = 0;
        int64_t d = 1;
        for(int i = 1; i < digit_len; i++) {
            dp[i] = 10 * dp[i-1] + d;
            d = d * 10;
        }
        fprintf(stdout, "%d %d\n", dp[0], dp[1]);

        int tmp = n;
        vector<int> digits;
        while(n) {
            digits.push_back(n % 10);
            n = n / 10;
        }
    
        vector<int64_t> count(10, 0);
        for(int i = digits.size() - 1; i >= 0; i--) {
            for(int j = 0;j < 10; j++)
                count[j] += dp[i] * digits[i];
            int d = pow(10, i);
            for(int j = 0; j < digits[i]; j++)
                count[j] += d;
            tmp -= (d * digits[i]);
            count[digits[i]] += tmp + 1;
            count[0] -= d; 
        }
        fprintf(stdout, "%d %d\n", count[0], dp[1]);
        
        return count[1];
    }
};

int main() {
    Solution solution;
    fprintf(stdout, "%d \n", solution.countDigitOne(13));
    return 0;
}