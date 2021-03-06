// Source.cpp
// This file serves to test the NIM AI
#include "AI.h"

// prototypes
std::string applyMoveToConfig(std::string& config, std::string move);
std::string genConfig();
void outputConfig(std::string config);
bool checkForWinner(std::string config);
bool showWinner(int turn, std::string& config);
bool checkForValidMove(std::string playerMove);
bool isInvalidMove(std::string playerMove, std::string config);

void main() {
	std::cout << "WELCOME TO NIM\n";
	std::cout << "TESTING AI...\n";

	std::string config = genConfig();
	bool nim = true;
	bool winner = false;
	while (nim) {
		outputConfig(config);
		// player turn - you'll want to add code for comments / forfeit
		std::string playerMove;
		std::cout << "Move? ";
		std::cin >> playerMove;
		while (isInvalidMove(playerMove, config)) {
			std::cout << "Invalid Move." << std::endl;
			std::cout << "Move? ";
			std::cin >> playerMove;
		}
		
		applyMoveToConfig(config, playerMove);
		if (checkForWinner(config)) {
			nim = showWinner(0, config);
		}
		else {
			outputConfig(config);

			// ai turn
			std::string optMove = optimalMove(config);
			std::cout << "AI MOVE: " << optMove << std::endl;
			applyMoveToConfig(config, optimalMove(config));

			if (checkForWinner(config)) {
				nim = showWinner(1, config);
			}
		}
	}
}

// takes a move in format "mnn"
std::string applyMoveToConfig(std::string& config, std::string move) {
	std::vector<int> sizes = convert_from_config(config);
	int heap = move[0] - '0';
	sizes[heap - 1] -= atoi(move.substr(1, 2).c_str());
	config = convert_to_config(sizes);
	return config;
}

std::string genConfig() {
	std::string config;
	// pick a random size between 1 and 9.
	int m = (rand() % 9) + 1;
	config.push_back('0' + m);
	for (int i = 0; i < m; i++) {
		// pick random heap sizes between 1 and 20.
		int heapSize = (rand() % 20) + 1;
		if (heapSize < 10) config.push_back('0');
		config.append(std::to_string(heapSize));
	}
	return config;
}

void outputConfig(std::string config) {
	std::cout << std::endl;
	int m = config[0] - '0';
	std::vector<int> sizes = convert_from_config(config);
	for (unsigned int i = 0; i < sizes.size(); i++){
		// output rocks
		std::cout << "Heap " << i + 1 << ": ";
		for (int k = 0; k < sizes[i]; k++)
			std::cout << '*';
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

bool checkForWinner(std::string config) {
	bool winner = true;
	std::vector<int> sizes = convert_from_config(config);
	for (unsigned int i = 0; i < sizes.size(); i++) {
		if (sizes[i] > 0) winner = false;
	} return winner;
}

bool showWinner(int turn, std::string& config) {
	bool cont = false; // continue playing?
	if (turn == 0) std::cout << "Player Wins!" << std::endl;
	else std::cout << "You Lost Sucker!" << std::endl;
	std::string yes;
	std::cout << "Play Again?(yes or no) ";
	std::cin >> yes;
	if (yes == "yes") {
		cont = true;
		config = genConfig();
		std::cout << std::endl;
	}
	else {
		std::cout << "Thanks for Playing!" << std::endl;
	}
	return cont;
}

bool isInvalidMove(std::string playerMove, std::string config) {
	bool valid = true;
	char m = config[0]; // number of heaps as char
	char player_move_m = playerMove[0];
	if (isdigit(player_move_m)) {
		int mint = m - '0'; // number of heaps as int
		int pmint = player_move_m - '0';
		// check for non-existent pile
		if (pmint == 0) valid = false; 
		else if (pmint > mint) valid = false;
		// check for wrong format other than "mnn"
		else if (playerMove.size() != 3) valid = false;
		else { // check for valid num rocks from pile
			std::vector<int> heapSizes = convert_from_config(config);
			int numToRemove = atoi(playerMove.substr(1, 2).c_str());
			if (numToRemove > heapSizes[pmint - 1]) valid = false;
			else if (numToRemove == 0) valid = false;
		}
	}
	else valid = false;
	return !valid;
}