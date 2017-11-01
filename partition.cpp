#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <iostream>
#include <chrono>


#include "partition.hpp"



namespace FastWin{
  int num_of_partitions = 16;
  int attribute_to_partition;
  std::mutex combine_mutex;

  void divide(std::unordered_map<int, std::vector<std::vector<int>>>& my_map,
              std::unordered_map<int, std::vector<std::vector<int>>>& combined_map, 
              std::vector<std::vector<int>>& my_data) {
    for(std::vector<int> v : my_data){
      //std::cout<<v.at(attribute_to_partition) % num_of_partitions << std::endl;
      my_map[v.at(attribute_to_partition) % num_of_partitions].push_back(v);
    }
    //std::cout<<my_map[1].size()<<std::endl;
    
    combine_mutex.lock();
    for(int i = 0; i < num_of_partitions; i++){
      combined_map[i].insert(combined_map[i].end(), my_map[i].begin(), my_map[i].end());
    }
    combine_mutex.unlock();
    //std::cout<<combined_map[1].size()<<std::endl;
    return;
  }

  void partitionBy(std::unordered_map<int, std::vector<std::vector<int>>>& combined_map, 
                   int attributeToPartitionBy, 
                   std::vector<std::vector<int>>& dataset){
    attribute_to_partition = attributeToPartitionBy;
    std::cout<<"start processing partition......"<<std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < num_of_partitions; i++){
      combined_map.insert(std::make_pair(i, std::vector<std::vector<int>>()));
    }
    std::vector<std::unordered_map<int, std::vector<std::vector<int>>>> perthread_map;
    for(int i = 0; i < 4; i++){
      perthread_map.push_back(std::unordered_map<int, std::vector<std::vector<int>>>());
      for(int j = 0; j < num_of_partitions; j++){
          perthread_map.at(i).insert(std::make_pair(j, std::vector<std::vector<int>>()));
      }
    }
    if(dataset.size() < 1024){
      divide(perthread_map.at(0), combined_map, dataset);
      //std::cout<<"haha"<<combined_map[1].size()<<std::endl;
    } else {
      std::vector<std::thread> workers;
      std::vector<std::vector<std::vector<int>>> divided_set;
      int block = dataset.size() / 4;
      for(int i = 0; i < 4; i++){
        if(i == 3){
          divided_set.push_back(std::vector<std::vector<int>>(dataset.begin() + i * block, dataset.end()));
        }else{
          divided_set.push_back(std::vector<std::vector<int>>(dataset.begin() + i * block, dataset.begin() + (i + 1) * block - 1));
        }
      }
      for(int i = 0; i < 4; i++){
        workers.push_back(std::thread(divide, 
                                      std::ref(perthread_map.at(i)),
                                      std::ref(combined_map), 
                                      std::ref(divided_set.at(i))));
      }
      for(int i = 0; i < 4; i++){
        workers.at(i).join();
      }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout<<"partition (ms): 	" << (double)elapsed.count() / 1000000 << std::endl;
    return;
  } 
}
