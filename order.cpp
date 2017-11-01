#include <vector>
#include "order.hpp"
#include <parallel/algorithm>
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <unordered_map>
#include "partition.hpp"

namespace FastWin{

int attributeToOrder = 0;

bool myOrderFunction(std::vector<int> a, std::vector<int> b){
        if(a.at(attribute_to_partition) == b.at(attribute_to_partition))
	  return a.at(attributeToOrder) < b.at(attributeToOrder);
        else
          return a.at(attribute_to_partition) < b.at(attribute_to_partition);
}

void orderBy(int attributeToOrderBy, std::unordered_map<int, std::vector<std::vector<int>>>& combined_dataset){
        attributeToOrder = attributeToOrderBy;
	//std::cout<<dataset.at(0).size()<<std::endl;
        std::cout<<"start processing sort......"<<std::endl;
	auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < num_of_partitions; i++){
          //std::cout << combined_dataset[i].size() <<std::endl;
          __gnu_parallel::sort(combined_dataset[i].begin(), combined_dataset[i].end(), myOrderFunction);
        }
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	std::cout << "parallel sort (ms):        "<< (double)elapsed.count() / 1000000 << std::endl;

        return;
}
}
