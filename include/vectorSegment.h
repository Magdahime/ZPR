#pragma once

#include <vector>

template <typename type>
struct VectorSegment
{
    typename std::vector<type>::iterator segmentFirst{};
    typename std::vector<type>::iterator segmentLast{};
};