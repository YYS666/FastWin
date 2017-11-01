#include <vector>
#include <unordered_map>

namespace FastWin{
    extern int num_of_partitions;
    extern int attribute_to_partition;
    void partitionBy(std::unordered_map<int, std::vector<std::vector<int>>>& combined_dataset,
                     int attributeToPartitionBy, 
                     std::vector<std::vector<int>>& dataset);
}
