// segment_tree.cpp
#include "segment_tree.hpp"

SegmentTreeScheduler::SegmentTreeScheduler(const std::vector<int>& arr) : n(arr.size()), tree(2 * n) {
    build(arr);
}

void SegmentTreeScheduler::build(const std::vector<int>& arr) {
    for (int i = 0; i < n; ++i) tree[n + i] = arr[i];
    for (int i = n - 1; i > 0; --i) tree[i] = tree[2*i] + tree[2*i+1];
}

void SegmentTreeScheduler::update(int index, int value) {
    int pos = index + n;
    tree[pos] = value;
    while (pos > 1) {
        pos /= 2;
        tree[pos] = tree[2*pos] + tree[2*pos+1];
    }
}

int SegmentTreeScheduler::range_query(int left, int right) {
    left += n; right += n;
    int sum = 0;
    while (left < right) {
        if (left % 2) sum += tree[left++];
        if (right % 2) sum += tree[--right];
        left /= 2; right /= 2;
    }
    return sum;
}
