#include "print.hpp"

#include <fea/benchmark/benchmark.hpp>
#include <fea/numerics/random.hpp>
#include <glm/vec3.hpp>
#include <gtest/gtest.h>
#include <tbb/parallel_for.h>
#include <tbb/scalable_allocator.h>
#include <vector>


/*
TEST(Allocator, FreeDegradation) {
	constexpr size_t num_items = 10'000'000;
	constexpr size_t num_subvecs = 20;

	// Preheat tbb.
	{
		std::vector<glm::vec3> v(num_items);
		std::iota(v.begin(), v.end(), glm::vec3{});
		fea::random_shuffle(v.begin(), v.end());

		auto preheat = [&](const tbb::blocked_range<size_t>& range) {
			for (size_t i = range.begin(); i < range.end(); ++i) {
				size_t next_i = (i + 1) % v.size();
				v[i] += v[next_i];
			}
		};
		tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() }, preheat);
		tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() }, preheat);
		tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() }, preheat);
		tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() }, preheat);
		tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() }, preheat);

		print_rnd(v);
	}

	// Benchmark single-threaded allocations vs multithreaded.
	fea::bench::suite s;
	s.title("Benchmark tbb::scalable_allocator allocations and releases.");
	s.average(5);
	s.sleep_between(std::chrono::milliseconds(500));

	s.benchmark(
			"Single-threaded vector alloc/dealloc (default allocator).", []() {
				std::vector<std::vector<glm::vec3>> v(num_subvecs);
				for (size_t i = 0; i < v.size(); ++i) {
					v[i] = std::vector<glm::vec3>(num_items);
				}
				print_rnd(v);
			});


	s.benchmark(
			"Multi-threaded vector alloc/dealloc (default allocator).", [&]() {
				using int_vec_t = std::vector<glm::vec3>;
				std::vector<int_vec_t> v(num_subvecs);

				tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() },
						[&](const tbb::blocked_range<size_t>& range) {
							for (size_t i = range.begin(); i < range.end();
									++i) {
								v[i] = int_vec_t(num_items);
							}
						});
				print_rnd(v);
			});

	s.benchmark(
			"Multi-threaded vector alloc/dealloc (scalable_allocator).", [&]() {
				using int_vec_t = std::vector<glm::vec3,
						tbb::scalable_allocator<glm::vec3>>;
				std::vector<int_vec_t, tbb::scalable_allocator<int_vec_t>> v(
						num_subvecs);

				tbb::parallel_for(tbb::blocked_range<size_t>{ 0, v.size() },
						[&](const tbb::blocked_range<size_t>& range) {
							for (size_t i = range.begin(); i < range.end();
									++i) {
								v[i] = int_vec_t(num_items);
							}
						});
				print_rnd(v);
			});

	s.print();
}
*/
