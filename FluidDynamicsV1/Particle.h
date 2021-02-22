#ifndef PARTICLE_H
#define PARTICLE_H

// defines

class Particle {
private:
	float size;
	float velocity;

	std::vector<Particle*> neighbors = std::vector<Particle*>();

public:
	Particle() {
		velocity = 0;
	}

	Particle(float startSize) {
		size = startSize;
		velocity = 0;
	}

	// setters
	void setSize(float size) {
		this->size = size;
	}

	void setVelocity(float velocity) {
		this->velocity = velocity;
	}

	void setNeighbors(std::vector<Particle*> neighbors) {
		this->neighbors = neighbors;
	}

	// getters
	float getSize() {
		return size;
	}

	float getVelocity() {
		return velocity;
	}

	std::vector<Particle*> getNeighbors() {
		return neighbors;
	}
};

#endif
