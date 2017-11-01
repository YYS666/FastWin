#include <vector>
#include <unordered_map>
#include <string>
#include <thread>
#include <iostream>
#include <stdlib.h>
#include <chrono>

#include "partition.hpp"
#include "order.hpp"
#include "WindowFunctionSum.hpp"
#include "WindowFunctionAvg.hpp"
#include "WindowFunctionMin.hpp"
#include "WindowFunctionMax.hpp"
#include "WindowFunctionRank.hpp"

namespace FastWin{

  void WindowFunctionHandle(
                          std::string fun_name,
                          std::string fun_attribute,
                          std::string rangeOrRow,
                          std::string preceding,
                          std::string following,
                          std::unordered_map<int, std::vector<std::vector<int>>>& combined_dataset
  )
  {
    std::vector<std::thread> workers;
    std::vector<std::vector<std::vector<int>>> perthread_levels;
    std::vector<bound_rec> perthread_rec;
    for(int i = 0; i < combined_dataset.size(); i++){
      perthread_levels.push_back(std::vector<std::vector<int>>());
      perthread_rec.push_back(bound_rec());
    }
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < combined_dataset.size(); i++){
      if(combined_dataset[i].size() == 0)
         continue;
      switch(std::stoi(fun_name)){
        case 1:
          workers.push_back(std::thread(FastWin::WindowFunctionSum,
                                        fun_attribute, 
                                        rangeOrRow, 
                                        preceding, 
                                        following, 
                                        std::ref(combined_dataset[i]),
                                        std::ref(perthread_levels.at(i)),
                                        std::ref(perthread_rec.at(i))));
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          break;
        case 5:
          break;
        default:
          std::cout << "Function not supported." << std::endl;
          return;
      }
    }
    for(int i = 0; i < workers.size(); i++){
      workers.at(i).join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout<<"aggregation (ms):         " << (double)elapsed.count() / 1000000 << std::endl;
  }
}

