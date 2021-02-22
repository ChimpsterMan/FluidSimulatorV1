#ifndef FLUIDSIMULATOR_H
#define FLUIDSIMULATOR_H

// defines
#include <vector>
#include <chrono>

#include "Particle.h"

using namespace std;

class FluidSimulator {
private:
	unsigned int WIDTH;
	unsigned int HEIGHT;

	float fluidity;

	// floating field (fluid bars)
	vector<vector<Particle>> field;

	// randomized order field for calculations
	vector<Particle*> calcField;

public:
	// default width = 100, default height = 100
	FluidSimulator() {
		WIDTH = 100;
		HEIGHT = 100;

		fluidity = 0.01;

		clearField();
	}

	FluidSimulator(unsigned int width, unsigned int height) {
		this->WIDTH = width;
		this->HEIGHT = height;

		fluidity = 0.01;

		clearField();
	}

	// takes timechange millisec (chrono)
	void update() {
		float maxDisplacement = 100;
		float minDisplacement = 0.1;

		for (int j = 0; j < calcField.size(); j++) {
			vector<Particle*> sortedList = calcField[j]->getNeighbors();

			// bubble sort so the smallest size is at the bottom
			for (int i = 0; i < sortedList.size(); i++) {
				for (int j = 0; j < (sortedList.size() - 1) - i; j++) {
					if (sortedList[j]->getSize() > sortedList[j + 1]->getSize()) {
						Particle* temp = sortedList[j + 1];
						sortedList[j + 1] = sortedList[j];
						sortedList[j] = temp;
					}
				}
			}

			// apply displacement
			for (int i = 0; i < sortedList.size(); i++) {
				// break if adjacent columns are greater than the main one (There is nothing to dispurse)
				if (calcField[j]->getSize() <= 0 || sortedList[i]->getSize() >= calcField[j]->getSize()) {
					break;
				}

				// find displacement
				float displacement = calcField[j]->getSize() * (1 - sortedList[i]->getSize() / calcField[j]->getSize()) * fluidity;

				// limiters
				if (displacement > maxDisplacement) {
					displacement = maxDisplacement;
				}

				else if (displacement < minDisplacement) {
					displacement = minDisplacement;
				}

				// reduce if the base size will be zero
				if (calcField[j]->getSize() - displacement < 0) {
					displacement -= (displacement - calcField[j]->getSize());
				}

				//std::cout << displacement << " " << calcField[j]->getSize() * (1 - sortedList[i]->getSize() / calcField[j]->getSize()) << std::endl;

				// subtract displacement from main
				calcField[j]->setSize(calcField[j]->getSize() - displacement);

				// apply to i column
				sortedList[i]->setSize(sortedList[i]->getSize() + displacement);
			}
		}

	}

	// utility
	// position, width, height
	void addFluid(int x, int y, int width, int height, int size) {
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if (j > x && j < width + x && i > y && i < y + height) {
					field[i][j].setSize(field[y][x].getSize() + size);
				}
			}
		}
	}

	// A reset of sorts
	void clearField() {
		field = vector<vector<Particle>>();

		// init particles
		for (int y = 0; y < HEIGHT; y++) {
			field.push_back(vector<Particle>());
			for (int x = 0; x < WIDTH; x++) {
				field[y].push_back(Particle());
			}
		}

		// set neighbors for each particle
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				vector<Particle*> tempList = vector<Particle*>();

				if (x - 1 >= 0) {
					tempList.push_back(&field[y][x - 1]);
				}
				if (x + 1 < WIDTH) {
					tempList.push_back(&field[y][x + 1]);
				}
				if (y - 1 >= 0) {
					tempList.push_back(&field[y - 1][x]);
				}
				if (y + 1 < HEIGHT) {
					tempList.push_back(&field[y + 1][x]);
				}

				field[y][x].setNeighbors(tempList);
			}
		}

		// set calcField
		vector<int> randX = vector<int>();
		vector<int> randY = vector<int>();

		srand(time(NULL));

		// x axis
		for (int i = 0; i < WIDTH; i++) {
			int tempRand;

			bool inList = true;
			while (inList) {
				inList = false;

				tempRand = rand() % WIDTH;

				for (int j = 0; j < randX.size(); j++) {
					if (tempRand == randX[j]) {
						inList = true;
						break;
					}
				}
			}

			randX.push_back(tempRand);
		}

		// y axis
		for (int i = 0; i < HEIGHT; i++) {
			int tempRand;

			bool inList = true;
			while (inList) {
				inList = false;

				tempRand = rand() % HEIGHT;

				for (int j = 0; j < randY.size(); j++) {
					if (tempRand == randY[j]) {
						inList = true;
						break;
					}
				}
			}

			randY.push_back(tempRand);
		}

		// construct random map
		calcField = vector<Particle*>();
		for (int y = 0; y < randY.size(); y++) {
			for (int x = 0; x < randX.size(); x++) {
				//std::cout << "a " << calcField.size() << " " << randY[y] << " " << randX[x] << std::endl;
				calcField.push_back(&field[randY[y]][randX[x]]);
			}
		}
	}

	// setters
	void setField(vector<vector<Particle>> newField) {
		field = newField;
	}

	void setFluidity(float val) {
		fluidity = val;
	}

	// getters
	vector<vector<float>> getFloatField() {
		vector<vector<float>> temp = vector<vector<float>>();

		for (int y = 0; y < HEIGHT; y++) {
			temp.push_back(vector<float>());
			for (int x = 0; x < WIDTH; x++) {
				temp[y].push_back(field[y][x].getSize());
			}
		}

		return temp;
	}

	float getFluidity() {
		return fluidity;
	}
};

#endif;
