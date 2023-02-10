#pragma once
#include <fea/numerics/random.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// So our benchmarks aren't optimized out.

struct printer {
	~printer() {
		std::cerr << std::endl << ss.str() << std::endl;
	}

	template <class T>
	void queue(const T& v) {
		ss << v;
	}

	std::stringstream ss;
};

template <class Alloc>
void print_rnd(const std::vector<glm::vec3, Alloc>& v) {
	static printer p;
	size_t rnd_idx = fea::random_idx(v.size());
	p.queue(v[rnd_idx].x);
}

template <class CAlloc, class Alloc>
void print_rnd(const std::vector<std::vector<glm::vec3, CAlloc>, Alloc>& v) {
	size_t rnd_idx = fea::random_idx(v.size());
	print_rnd(v[rnd_idx]);
}
