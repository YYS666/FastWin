#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <thread>
#include "WindowFunctionSum.hpp"
#include "partition.hpp"

namespace FastWin{

void buildLevels(int fanout, 
                 int funAttribute, 
                 std::vector<std::vector<int>>& dataset,
                 std::vector<std::vector<int>>& sum_levels
){
	//build first level attribute
        if(fanout == 1) {
        	std::cout << "ERROR: fanout has to be larger than 1" << std::endl;
                return;
        }
	std::vector<int> first_level;
	int toPut = 0, count = 0;
	for(int i = 0; i < dataset.size(); i++){
		toPut = dataset.at(i).at(funAttribute);
		first_level.push_back(toPut);
	}
	
	sum_levels.push_back(first_level);
	
	while(sum_levels.back().size() > 1){
		std::vector<int> toAdd;
		int sumAggregate = 0, count = 0;
		for(int i = 0; i < sum_levels.back().size(); i++){
			sumAggregate += sum_levels.back().at(i);
			count++;
			if(count == fanout || i == sum_levels.back().size() - 1){
				toAdd.push_back(sumAggregate);
				sumAggregate = 0;
				count = 0;
			}
		}
		sum_levels.push_back(toAdd);
	}                                                                                                                                               
	return;
}


int traverseTree(int begin, 
                 int end, 
                 int fanout,
                 std::vector<std::vector<int>>& sum_levels
) {
	int ans = 0;
	for(int i = 0; i < sum_levels.size(); i++) {
        	int parentBegin = begin / fanout;
		int parentEnd = end / fanout;
		//std::cout<<begin<<"	"<<end<<std::endl;
		if(parentBegin == parentEnd){
			for(int j = begin; j <= end; j++){
				//std::cout<<i<<"	"<<j<<std::endl;
				ans += sum_levels.at(i).at(j);
			}
			return ans;
 		}
		int groupBegin = parentBegin * fanout;
		if(begin != groupBegin){
			int limit = groupBegin + fanout;
			for(int pos = begin; pos < limit; pos++){
				ans += sum_levels.at(i).at(pos);
			}
			parentBegin++;
		}
		int groupEnd = (parentEnd + 1) * fanout - 1;
		if(end != groupEnd){
			for(int pos = groupEnd + 1 - fanout; pos <= end; pos++){
				ans += sum_levels.at(i).at(pos);
			}
			parentEnd--;
		}
		begin = parentBegin;
		end = parentEnd;
        }
}

void removableCumulativeSum(int attributeToCompute, 
                            std::string preceding, 
                            std::string following, 
                            std::vector<std::vector<int>>& dataset,
                            std::unordered_map<int, std::vector<int>>& sum_ref){
	//calculate aggregate sum based on different conditions.
	if(preceding == "ALL" && following == "ALL"){
		for(int i = 0; i < dataset.size(); ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
		        int sum = 0;
                        for(int j = start; j <= end; j++){
			  sum += dataset[j].at(attributeToCompute);
                        }
                        for(int j = start; j <= end; j++){
			  dataset[j].push_back(sum);
                        }
                        i = end + 1;
		}
	}
	else if(preceding == "ALL" && following == "NO"){
		for(int i = 0; i < dataset.size(); ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
		        int sum = 0;
                        for(int j = start; j <=end; j++){
			  sum += dataset.at(j).at(attributeToCompute);
			  dataset.at(j).push_back(sum);
                        }
                        i = end + 1;
		}
	}
	else if(preceding == "NO" && following == "ALL"){
		for(int i = dataset.size() - 1; i >= 0; ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
		        int sum = 0;
                        for(int j = end; j >= start; j--){
			  sum += dataset.at(j).at(attributeToCompute);
			  dataset.at(j).push_back(sum);
                        }
                        i = start - 1;
		}
	}
	else if(preceding == "RAND" && following == "RAND"){
		for(int i = 0; i < dataset.size(); i++){
			int pend = dataset.size() - 1;
			int fbegin = rand() % (i + 1);
			int fend = rand() % (dataset.size()) + i;
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
			fbegin = std::max(start, fbegin);
			fend = std::min(end, fend);
			int sum = 0;
			for(int j = fbegin; j <= fend; j++){
				sum += dataset.at(j).at(attributeToCompute);
			}
			dataset.at(i).push_back(sum);
		}
	}
	else{
		int numPreceding = std::stoi(preceding);
		int numFollowing = std::stoi(following);
		for(int i = 0; i < dataset.size(); ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
			int sum = 0;
			int fbegin = 0, fend = numFollowing;
			for(int j = i; j <= i + numFollowing && j <= end; j++){
				sum += dataset.at(j).at(attributeToCompute);
			}
			dataset.at(i).push_back(sum);
			//propagate the window frame from second tuple
			for(int j = i + 1; j <= end; j++){
				fbegin = j - numPreceding;
				fend = j + numFollowing;
				if(fbegin <= start && fend <= end){
					sum += dataset.at(fend).at(attributeToCompute);
					dataset.at(j).push_back(sum);
				
				}else if(fbegin > start && fend <= end){
					sum -= dataset.at(fbegin - 1).at(attributeToCompute);
					sum += dataset.at(fend).at(attributeToCompute);
					dataset.at(j).push_back(sum);
				}else if(fbegin > start && fend > end){
					sum -= dataset.at(fbegin - 1).at(attributeToCompute);
					dataset.at(j).push_back(sum);
				}else if(fbegin < start && fend > end){
					dataset.at(j).push_back(sum);
				}
			}
                        i = end + 1;
		}	
	}
	return;
}

void segmentTreeSum(int attributeToCompute, 
                    int fanout, 
                    std::string preceding, 
                    std::string following, 
                    std::vector<std::vector<int>>& dataset,
                    std::vector<std::vector<int>>& sum_levels,
                    std::unordered_map<int, std::vector<int>>& sum_ref){
	//calculate aggregate sum based on different conditions.
	if(preceding == "ALL" && following == "ALL"){
                for(int i = 0; i < dataset.size(); ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
		  	int sum = traverseTree(start, end, fanout, sum_levels);
		  	for(int j = start; j <= end; j++){
				dataset.at(j).push_back(sum);
			}
                        i = end + 1;
                }
	}
	else if(preceding == "ALL" && following == "NO"){
		for(int i = 0; i < dataset.size(); ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
		  	for(int j = start; j <= end; j++){
		  	        int sum = traverseTree(start, j, fanout, sum_levels);
				dataset.at(j).push_back(sum);
			}
                        i = end + 1;
		}
	}
	else if(preceding == "NO" && following == "ALL"){
		for(int i = dataset.size() - 1; i >= 0; ){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
                        //std::cout<<start<<"	"<<end<<"	"<<dataset.size()<<std::endl;
		  	for(int j = end; j >= start; j--){
		  		int sum = traverseTree(j, end, fanout, sum_levels);
				dataset.at(j).push_back(sum);
			}
                        i = start - 1;
		}
	}
	else if(preceding == "RAND" && following == "RAND"){
		for(int i = 0; i < dataset.size(); i++){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
			int fbegin = rand() % (i + 1);
			int fend = rand() % (dataset.size()) + i;
			fbegin = std::max(start, fbegin);
			fend = std::min(end, fend);
			int sum = 0;
			sum = traverseTree(fbegin, fend, fanout, sum_levels);
			dataset.at(i).push_back(sum);
		}
	}
	else{
		int numPreceding = std::stoi(preceding);
		int numFollowing = std::stoi(following);
		int fbegin = 0, fend = numFollowing;
		// for each tuple, traverse the tree to calculate the value.
		for(int i = 0; i < dataset.size(); i++){
                        int start = sum_ref[dataset.at(i).at(attribute_to_partition)].at(0);
                        int end = sum_ref[dataset.at(i).at(attribute_to_partition)].at(1);
			fbegin = i - numPreceding;
			fend = i + numFollowing;
			if(fbegin < start) {
				fbegin = start;
    			}
			if(fend > end) {
				fend = end;
 			}
			int sum = 0;
			sum = traverseTree(fbegin, fend, fanout, sum_levels);
			dataset[i].push_back(sum);
                        if(dataset[i].size() == 3){
			  dataset[i].push_back(sum);
                          //std::cout<<dataset[i].size()<<std::endl;
                        }
		}	
	}
	return;
}



void WindowFunctionSum(
                        std::string funAttribute,
                        std::string rangeOrRow,
                        std::string preceding,
                        std::string following,
                        std::vector<std::vector<int>>& dataset,
                        std::vector<std::vector<int>>& sum_level,
                        std::unordered_map<int, std::vector<int>>& sum_ref

){
	//std::cout << "Processing Sum..." << std::endl;
	
	//divide the current dataset into several if needed.
	//used when intra-partition is applied
	
	//build levels for further use.
	int fanout = 2;
	
	int attributeToCompute = std::stoi(funAttribute) - 1;
	//std::cout <<"Preceding: "<< preceding << "	" <<"Following: "<< following << std::endl;
        
        int partition = dataset.at(0).at(attribute_to_partition);
        int start = 0; 
        int end = 0;
        //std::cout<<"start processing ref"<<std::endl;
        for(int i = 0; i <= dataset.size(); i++){
          if(i == dataset.size()){
            std::vector<int> toAdd;
            toAdd.push_back(start);
            toAdd.push_back(end - 1);
            sum_ref.insert(std::make_pair(partition, toAdd));
          }else{
            if(dataset.at(i).at(attribute_to_partition) != dataset.at(start).at(attribute_to_partition)){
              std::vector<int> toAdd;
              toAdd.push_back(start);
              toAdd.push_back(end - 1);
              sum_ref.insert(std::make_pair(partition, toAdd));
              partition = dataset.at(i).at(attribute_to_partition);
              start = i;
              end++;
            }else{
              end++;
            }
          }
        }
        //std::cout<<"end processing ref"<<std::endl;
        
	//auto start_time = std::chrono::high_resolution_clock::now();
	buildLevels(fanout, attributeToCompute, std::ref(dataset), std::ref(sum_level));
	//auto end_time = std::chrono::high_resolution_clock::now();
	//auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	//std::cout << "Building levels (ms):        "<< (double)elapsed.count() / 1000000 << std::endl;
	//start_time = std::chrono::high_resolution_clock::now();
	removableCumulativeSum(attributeToCompute, preceding, following, std::ref(dataset), std::ref(sum_ref));
	//end_time = std::chrono::high_resolution_clock::now();
	//elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	//std::cout << "Removable Cumulative Sum (ms):        "<< (double)elapsed.count() / 1000000 << std::endl;


	//start_time = std::chrono::high_resolution_clock::now();
	segmentTreeSum(attributeToCompute, fanout, preceding, following,std::ref( dataset), std::ref(sum_level), std::ref(sum_ref));
	//end_time = std::chrono::high_resolution_clock::now();
	//elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	//std::cout << "Segment Tree Sum (ms):        "<< (double)elapsed.count() / 1000000 << std::endl;
	return;
}
}
