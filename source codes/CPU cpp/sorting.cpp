#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int main (void) {
    vector<int> nums = {3, 6, 5, 9, 2};

    for (int i=0; i<nums.size(); i++) {
        for (int j = i + 1; j < nums.size(); j++) {
            if (nums[j] > nums[i]) {
                int temp = nums[i];
                nums[i] = nums[j];
                nums[j] = temp;
            }
        }
    }

    for (int &i : nums) {
        cout << i << " ";
    }

    return 0;
}