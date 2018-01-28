#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

ofstream outputFile;


int main(int argc, char *argv[]){
	

	//number of attributes in dataset
	int numAttributes; 

	//number of tuples in dataset
	int numTuples; 

	//number of partition in 1st attribute
	int numPartition;

	if(argc != 4){
		cout << "Please enter right number of arguments!" << endl;
		return 0;
	}else{
		numAttributes = atoi(argv[1]);
		numTuples = atoi(argv[2]);
		numPartition = atoi(argv[3]);
	}

	std::cout << "Number of Attributes is " << numAttributes << endl;
	std::cout << "Number of Tuples is " << numTuples << endl;
	std::cout << "Number of Partition is " << numPartition << endl;
	std::cout << RAND_MAX << endl;

	std::string filename = std::to_string(numAttributes) + "_" + 
						   std::to_string(numTuples) + "_" + 
						   std::to_string(numPartition) + ".csv";

	std::cout << filename << endl;

	outputFile.open(filename);


	//output schema of dataset
	for(int i = 0; i < numAttributes; i++){
		std::string schema_name = "attribute_" + std::to_string(i + 1);
		if(i < numAttributes - 1){
			outputFile << schema_name << ",";
		}else{
			outputFile << schema_name << std::endl;
		}
	}

	//generate dataset, dataset is stored in a vector in sorted order
	//then shuffle the attribute of interest in dataset and write the dataset to outputFile

	
	std::vector<std::vector<int>> dataset;
	for(int i = 0; i < numTuples; i++){
		std::vector<int> toadd;
		for(int j = 0; j < numAttributes; j++){
			if(j == 0){
				toadd.push_back(rand() % numPartition + 1);
			}else{
				toadd.push_back(i);
			}
		}
		dataset.push_back(toadd);
	}

	//shuffle the attribute of interest of dataset
	for(int i = 1; i < numAttributes; i++){
		std::random_shuffle(dataset.begin(), dataset.end());
	}

	//output the dataset to outputFile
	for(int i = 0; i < numTuples; i++){
		for(int j = 0; j < numAttributes; j++){
			if(j < numAttributes - 1){
				outputFile << dataset.at(i).at(j) << ",";
			}else{
				outputFile << dataset.at(i).at(j) << std::endl;
			}
		}
	}

	outputFile.close();
	return 0;

}

