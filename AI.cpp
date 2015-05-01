// AI.cpp
// This files serves to provide AI code for either client / server side
#include "AI.h"

// returns optimal move formatted "mnn" given a rock configuration
// formatted: "mn1n1n2n2n3n3nmnm"
std::string optimalMove(std::string config) {
	std::string optMove;

	int x = calcNinSum(config);
	std::vector<int> sizes = convert_from_config(config);
	int bestHeap, numToRemove;

	if (x == 0) { // case: even parity
		// empty any non-empty heap
		for (int i = 0; i < sizes.size(); i++) {
			if (sizes[i] > 0) {
				bestHeap = i + 1; // vector index is one less than heap
				numToRemove = sizes[i];
				break;
			}
		}
	}
	else { // case: odd parity
		for (unsigned int i = 0; i < sizes.size(); i++) {
			// take ninsum of each heap with x
			int ninsum = convertDec(exclusive_or(sizes[i], x));

			// if the ninsum is less than the heap size, set best move
			if (ninsum < sizes[i]) {
				bestHeap = i + 1; // vector index is one less than heap
				numToRemove = sizes[i] - ninsum;
			}
		}
	}
	// create "mnn" representation of optimal move
	optMove.push_back(bestHeap + '0');
	if (numToRemove < 10) optMove.push_back('0');
	optMove.append(std::to_string(numToRemove));
	
	return optMove;
}

std::vector<int> convert_from_config(std::string config) {
	std::vector<int> sizes;

	// get number of heaps
	int m = config[0] - '0';

	// loop for number of heaps (i)
	for (int i = 1, j = 1; i < m + 1; i++, j += 2) {
		int heapSize = atoi(config.substr(j, 2).c_str());
		sizes.push_back(heapSize);
	}

	// return vector<int> sizes
	return sizes;
}

std::string convert_to_config(std::vector<int> sizes) {
	std::string config;
	config.push_back(sizes.size() + '0');
	for (unsigned int i = 0; i < sizes.size(); i++) {
		if (sizes[i] < 10) {
			config.push_back('0');
			config.push_back(sizes[i] + '0');
		}
		else config.append(std::to_string(sizes[i]));
	}
	return config;
}

int calcNinSum(std::string config) {
	// get number of heaps
	int m = config[0];

	// convert heap counts to vector of binary strings
	std::vector<std::string> binaryStrings;
	std::vector<int> heapSizes = convert_from_config(config);
	for (unsigned int i = 0; i < heapSizes.size(); i ++) {
		std::string binary = convertBinary(heapSizes[i]);
		binaryStrings.push_back(binary);
	}

	// return the binary digital sum
	return convertDec(exclusive_or(binaryStrings));
}

std::string convertBinary(int dec) {
	return std::bitset<8>(dec).to_string();
}

int convertDec(std::string bin) {
	return std::bitset<8>(bin).to_ulong();
}

std::string exclusive_or(std::vector<std::string> bstrs) {
	// ninsum is the binary digital sum of the heap sizes
	std::string ninsum;

	// loop through binary strings, count 1s in each column
	for (unsigned int col = 0; col < 8; col++) {
		int counter = 0;
		for (unsigned int str = 0; str < bstrs.size(); str++) {
			if (bstrs[str][col] == '1')
				counter++;
		}
		if (counter % 2 != 0) ninsum.push_back('1');
		else ninsum.push_back('0');
	}
	return ninsum;
}

std::string exclusive_or(int heapSize, int x) {
	// convert the two ints to binary strings
	std::string ninsum;
	std::vector<std::string> bstrs;
	std::string binHeap = convertBinary(heapSize);
	std::string binX = convertBinary(x);

	// create a vector and return the other xor
	bstrs.push_back(binHeap);
	bstrs.push_back(binX);
	return exclusive_or(bstrs);
}