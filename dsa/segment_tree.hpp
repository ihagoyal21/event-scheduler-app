// segment_tree.hpp
#pragma once
#include <vector>

class SegmentTreeScheduler {
public:
    SegmentTreeScheduler(const std::vector<int>& arr);
    void update(int index, int value);
    int range_query(int left, int right);
    std::vector<int> tree;
private:
    int n;
    void build(const std::vector<int>& arr);
};
