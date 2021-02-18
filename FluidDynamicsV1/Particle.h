#ifndef PARTICLE_H
#define PARTICLE_H

// defines

class Particle {
private:
	float size;
	float velocity;

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

	// getters
	float getSize() {
		return size;
	}

	float getVelocity() {
		return velocity;
	}
};

#endif
