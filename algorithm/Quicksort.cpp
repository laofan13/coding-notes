#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int quit_sort2(vector<int> &nums, int s, int t) {
    if(s >= t)
        return s;
    int i = s;
    int x = nums[t];
    for(int j = s; j < t; j++) {
        if(nums[j] < x) {
            swap(nums[i++], nums[j]);
        }
    }
    swap(nums[i], nums[t]);
    return i;
}

void quit_sort(vector<int> &nums, int l, int r) {
    if(l >= r)
        return;
    int mid = quit_sort2(nums, l, r);
    // std::cout << mid << std::endl;
    // for(auto &x : nums)
    //     cout << x << " ";
    quit_sort(nums, l, mid - 1);
    quit_sort(nums, mid + 1, r);
}


int main() {
    // vector<int> data = {3,4,6,1,2};
    // vector<int> data = {1,2,3,4};
    // vector<int> data = {4,3,2,1};
    // vector<int> data = {3,2,1,1};
    vector<int> data = {3,2,1,1};

    quit_sort(data, 0, data.size() - 1);

    for(auto &x : data)
        cout << x << " ";
    return 0;
}

1. 100T = 100*1024G/16G = 6400份
2. 10台机器，每台机器处理640份, 对其进行外排序，最后生成十份排序好的文件在共享存储上
3. 最后一台机器负责归并排序这十份文件排序好的文件。

t每份16G文件读取时间或者写入时间
9 * 1280t*10 + 6400t + 


1. hash 100TB数据到 十份。
2. 每一台机器负责一份，进行外部排序
3. 最后一台机器负责归并排序这十份文件排序好的文件。
