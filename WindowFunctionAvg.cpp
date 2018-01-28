#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include "WindowFunctionAvg.hpp"
#include "WindowFunctionSum.hpp"

namespace FastWin{
void removableCumulativeAvg(int attributeToCompute, std::string preceding, std::string following, std::vector<std::vector<int>>& dataset){

        //determine the frame bound and calculate

        //calculate aggregate sum based on different conditions.
        if(preceding == "ALL" && following == "ALL"){
                int sum = 0;
		int count = 0;
                for(int i = 0; i < dataset.size(); i++){
                        sum += dataset[i].at(attributeToCompute);
			count++;
                }
                sum /= sum / count;
                for(int i = 0; i < dataset.size(); i++){
                        dataset.at(i).push_back(sum);
                }
        }
        else if(preceding == "ALL" && following == "NO"){
                int sum = 0;
		int count = 0;
		int toPut;
                for(int i = 0; i < dataset.size(); i++){
                        sum += dataset.at(i).at(attributeToCompute);
			count++;
			toPut = sum / count;
                        dataset.at(i).push_back(toPut);
                }
        }
        else if(preceding == "NO" && following == "ALL"){
                int sum = 0;
		int count = 0;
		int toPut;
                for(int i = dataset.size() - 1; i >= 0; i--){
                        sum += dataset.at(i).at(attributeToCompute);
			count++;
			toPut = sum / count;
                        dataset.at(i).push_back(toPut);
                }
        }
        else if(preceding == "RAND" && following == "RAND"){
                for(int i = 0; i < dataset.size(); i++){
                        int pend = dataset.size() - 1;
                        int fbegin = rand() % (i + 1);
                        int fend = rand() % (dataset.size()) + i;
                        fbegin = std::max(0, fbegin);
                        fend = std::min(pend, fend);
                        int sum = 0;
			int count = 0;
                        for(int j = fbegin; j <= fend; j++){
                                sum += dataset.at(j).at(attributeToCompute);
				count++;
                        }
                        dataset.at(i).push_back(sum / count);
                }
        }
        else{
                int numPreceding = std::stoi(preceding);
                int numFollowing = std::stoi(following);
                int sum = 0;
                int fbegin = 0, fend = numFollowing;

                //calculate initial sum
		int count = 0;
                for(int i = 0; i <= numFollowing && i < dataset.size(); i++){
                        sum += dataset.at(i).at(attributeToCompute);
			count++;
                }

                dataset.at(0).push_back(sum / count);

                //propagate the window frame from second tuple
                for(int i = 1; i < dataset.size(); i++){
                        fbegin = i - numPreceding;
                        fend = i + numFollowing;
                        if(fbegin <= 0 && fend < dataset.size()){
                                sum += dataset.at(fend).at(attributeToCompute);
				count++;
                                dataset.at(i).push_back(sum / count);

                        }else if(fbegin > 0 && fend < dataset.size()){
                                sum -= dataset.at(fbegin - 1).at(attributeToCompute);
                                sum += dataset.at(fend).at(attributeToCompute);
                                dataset.at(i).push_back(sum / count);

                        }else if(fbegin > 0 && fend >= dataset.size()){
                                sum -= dataset.at(fbegin - 1).at(attributeToCompute);
				count--;
                                dataset.at(i).push_back(sum / count);

                        }else if(fbegin < 0 && fend >= dataset.size()){
                                dataset.at(i).push_back(sum / count);
                        }
                }
        }
        return;
}


void segmentTreeAvg(int attributeToCompute, int fanout, std::string preceding, std::string following, std::vector<std::vector<int>>& dataset){
        //calculate aggregate sum based on different conditions.
        int sum = 0;
        if(preceding == "ALL" && following == "ALL"){
                //int sum = traverseTree(0, dataset.size() - 1, fanout);
		int count = dataset.size();
                for(int i = 0; i < dataset.size(); i++){
                        dataset.at(i).push_back(sum / count);
                }
        }
        else if(preceding == "ALL" && following == "NO"){
                int sum = 0;
		int count = 0;
                for(int i = 0; i < dataset.size(); i++){
                  //      sum = traverseTree(0, i, fanout);
			count = i + 1;
                        dataset.at(i).push_back(sum / count);
                }
        }
        else if(preceding == "NO" && following == "ALL"){
                int sum = 0;
		int count = 0;
                for(int i = dataset.size() - 1; i >= 0; i--){
                    //    sum = traverseTree(i, dataset.size() - 1, fanout);
			count = dataset.size() - i;
                        dataset.at(i).push_back(sum);
                }
        }
        else if(preceding == "RAND" && following == "RAND"){
                for(int i = 0; i < dataset.size(); i++){
                        int pend = dataset.size() - 1;
                        int fbegin = rand() % (i + 1);
                        int fend = rand() % (dataset.size()) + i;
                        fbegin = std::max(0, fbegin);
                        fend = std::min(pend, fend);
                        int sum = 0;
                      //  sum = traverseTree(fbegin, fend, fanout);
			int count = fend - fbegin + 1;
                        dataset.at(i).push_back(sum / count);
                }
        }
        else{
                int numPreceding = std::stoi(preceding);
                int numFollowing = std::stoi(following);
                int fbegin = 0, fend = numFollowing;
                // for each tuple, traverse the tree to calculate the value.
                for(int i = 0; i < dataset.size(); i++){
                        fbegin = i - numPreceding;
                        fend = i + numFollowing;
                        if(fbegin < 0) {
                                fbegin = 0;
                        }
                        if(fend >= dataset.size()) {
                                fend = dataset.size() - 1;
                        }
                        int sum = 0;
			int count = 0;
                        //std::cout<<fbegin << "        " << fend<< std::endl;
                        //sum = traverseTree(fbegin, fend, fanout);
			count = fend - fbegin + 1;
                        dataset.at(i).push_back(sum / count);
                }
        }
        return;
}                                  

void WindowFunctionAvg(
                        std::string funAttribute,
                        std::string partitionAttribute,
			std::string orderAttribute,
                        std::string rangeOrRow,
                        std::string preceding,
                        std::string following,
                        std::vector<std::vector<int>>& dataset
		)
{
	std::cout << "Processing Avg..." << std::endl;
        
        int orderByAttribute;
        //divide the current dataset into several if needed.
        //used when intra-partition is applied
        //if(partitionAttribute != "NO"){
          //      divideDataset(partitionAttribute, dataset);
        //}       
        if(orderAttribute != "NO"){
                int orderByAttribute = std::stoi(orderAttribute);
        }       
        
        //build levels for further use.
        int fanout = 4;
        
        int attributeToCompute = std::stoi(funAttribute) - 1;
	if(sum_levels.size() == 0){
        	buildLevels(fanout, attributeToCompute, dataset);
        }
        std::cout << preceding << "     " << following << std::endl;
        removableCumulativeAvg(attributeToCompute, preceding, following, dataset);
        
        /*
        for (int i = 0; i < levels.size(); i++) {
          std::cout << "level: " << i << std::endl;
          for (int j = 0; j < levels.at(i).size(); j++) {
            std::cout << levels.at(i).at(j) << "        ";
          }
          std::cout << std::endl;
        }*/

        segmentTreeAvg(attributeToCompute, fanout, preceding, following, dataset);
        
        return;
	
}
}
