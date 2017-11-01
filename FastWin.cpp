#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <unordered_map>

#include "partition.hpp"
#include "order.hpp"
#include "WindowFunctionHandle.hpp"
#include "WindowFunctionSum.hpp"
#include "WindowFunctionAvg.hpp"
#include "WindowFunctionMin.hpp"
#include "WindowFunctionMax.hpp"
#include "WindowFunctionRank.hpp"


int main(int argc, char *argv[]){

	std::string fileName;
	std::string funName;
	std::string funAttribute;
	std::string partitionAttribute;
	std::string orderAttribute;
	std::string rangeOrRow;
	std::string numPreceding;
	std::string numFollowing;

	std::vector<std::vector<int>> dataset;
        std::unordered_map<int, std::vector<std::vector<int>>> combined_dataset;

	if(argc != 14){
		std::cout << "Please enter right number of argument!" << std::endl;
		return 0;
	}else{
		fileName = argv[1];
		funName = argv[2];
		funAttribute = argv[3];
		if((std::string)argv[4] != "partition"){
			std::cout << "Syntax Error: Please specify partition clause." << std::endl;
			return 0;
		}else{
			partitionAttribute = argv[5];
		}
		if((std::string)argv[6] != "order"){
			std::cout << "Syntax Error: Please specify order clause." << std::endl;
			return 0;
		}else{
			orderAttribute = argv[7];
		}
		if((std::string)argv[8] != "window"){
			std::cout << "Syntax Error: Please specify window condition." << std::endl;
			return 0;
		}else{
			rangeOrRow = argv[9];
		}
		if((std::string)argv[10] != "preceding"){
			std::cout << "Syntax Error: Please specify preceding condition." << std::endl;
			return 0;
		}else{
			numPreceding = argv[11];
		}
		if((std::string)argv[12] != "following"){
			std::cout << "Syntax Error: Please specify following condition." << std::endl;
			return 0;
		}else{
			numFollowing = argv[13];
		}
		
	}

	//read the data to dataset
	std::ifstream infile;
	infile.open(fileName);
	if(infile.is_open()){
		std::string schema;
		std::getline(infile, schema);
		std::cout << schema << std::endl;
		std::string line;
		while(std::getline(infile, line)){
			std::vector<int> toAdd;
			std::istringstream ss(line);
			std::string token;
			while(std::getline(ss, token, ',')){
				int num = std::stoi(token);
				toAdd.push_back(num);
			} 
			dataset.push_back(toAdd);
		}

	}else{
		std::cout << "Error opening file." << std::endl;
		return 0;
	}

	infile.close();

	if(partitionAttribute != "NO"){
		int attributeToPartitionBy = std::stoi(partitionAttribute) - 1;
      		//std::cout<<attributeToPartitionBy << std::endl;
		FastWin::partitionBy(combined_dataset, attributeToPartitionBy, dataset);
	}else{
		
	}
        

	if(orderAttribute != "NO"){
		int attributeToOrderBy = std::stoi(orderAttribute) - 1;
		FastWin::orderBy(attributeToOrderBy, combined_dataset);
	}else{
	}
        FastWin::WindowFunctionHandle(funName, funAttribute, rangeOrRow, numPreceding, numFollowing, combined_dataset);
        int count = 0;
        for(int i = 0; i < combined_dataset.size() && count < 100; i++){
          for(int j = 0; j < combined_dataset[i].size() && count < 100; j++){
            for(int k = 0; k < combined_dataset[i].at(j).size(); k++){
              std::cout<< combined_dataset[i].at(j).at(k) << " ";
            }
            count++;
            std::cout<<std::endl;
          }
        }  

        return 0;
}


