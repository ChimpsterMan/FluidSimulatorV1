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
		float maxNetDisplacement;

		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				vector<Particle*> sortedList = vector<Particle*>();

				// compare size to adjacent sides for acceleration (average the difference of all of them)
				int avgDist = 0;

				if (x - 1 >= 0) {
					if (field[y][x - 1].getSize() - field[y][x].getSize() < 0) {
						avgDist++;
					}
					sortedList.push_back(&field[y][x - 1]);
				}
				if (x + 1 < WIDTH) {
					if (field[y][x + 1].getSize() - field[y][x].getSize() < 0) {
						avgDist++;
					}
					sortedList.push_back(&field[y][x + 1]);
				}
				if (y - 1 >= 0) {
					if (field[y - 1][x].getSize() - field[y][x].getSize() < 0) {
						avgDist++;
					}
					sortedList.push_back(&field[y - 1][x]);
				}
				if (y + 1 < HEIGHT) {
					if (field[y + 1][x].getSize() - field[y][x].getSize() < 0) {
						avgDist++;
					}
					sortedList.push_back(&field[y + 1][x]);
				}

				if (avgDist == 0) {
					field[y][x].setVelocity(field[y][x].getVelocity() / 2);
					avgDist = 1;
				}
				else {
					// update veloctiy
					field[y][x].setVelocity(field[y][x].getVelocity() + (-avgDist * fluidity));
				}

				if (field[y][x].getVelocity() < 0) {
					if (field[y][x].getSize() + field[y][x].getVelocity() < 0) {
						field[y][x].setSize(0);
						field[y][x].setVelocity(0);
					}

					// update size based on velocity and dispurse the displaced fluid to nextdoor particles
					float displacement = avgDist * fluidity;

					field[y][x].setSize(field[y][x].getSize() - displacement);

					//std::cout << displacement << std::endl;
					//std::cout << sortedList.size() << std::endl;

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

					float startDisplacement = displacement;
					float displacedMass = 0;
					// slowly fill up the bucket to its required level
					for (int i = 0; i < sortedList.size() - 1; i++) {
						//std::cout << "Loop " << i << " ";
						if (displacement != 0) {
							float toAdd = sortedList[i]->getSize() + (displacement / (i + 1));
							//std::cout << "#1 " << displacement << " " << toAdd << " ";

							//std::cout << "#2 " << toAdd << " " << sortedList[i + 1]->getSize() << " ";
							if (toAdd > sortedList[i + 1]->getSize()) {
								displacement = (toAdd - sortedList[i + 1]->getSize()) * (i + 1);
								//std::cout << "#3 " << displacement << " ";
								toAdd = sortedList[i + 1]->getSize();
								//std::cout << "#4 " << displacement << " ";
							}
							else {
								displacement = 0;
							}

							for (int j = 0; j <= i; j++) {
								displacedMass += toAdd - sortedList[j]->getSize();
								sortedList[j]->setVelocity(sortedList[j]->getVelocity() + (toAdd - sortedList[j]->getSize()));
								sortedList[j]->setSize(toAdd);
							}
						}
					}

					//std::cout << startDisplacement << " " << displacedMass << std::endl;

					//std::cout << std::endl;
					if (y == 25 && x == 30) {
						//std::cout << std::endl;
					}

					//if (displacedMass == startDisplacement) {
						//

					//}
				}
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

		for (int y = 0; y < HEIGHT; y++) {
			field.push_back(vector<Particle>());
			for (int x = 0; x < WIDTH; x++) {
				field[y].push_back(Particle());
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
