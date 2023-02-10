#include "shared.hpp"

#include <algorithm>
#include <fea/numerics/random.hpp>
#include <tbb/parallel_for.h>
#include <vector>

namespace shrd {
void shrd::do_threaded_work(size_t begin, size_t end) {
	auto do_work = [](const tbb::blocked_range<size_t>& range) {
		std::vector<float> v(range.size());
		for (size_t i = range.begin(); i < range.end(); ++i) {
			fea::random_fill(v.begin(), v.end(), -42.f, 42.f);
			float res = std::accumulate(v.begin(), v.end(), 0.f);
			res;
		}
	};

	tbb::parallel_for(tbb::blocked_range<size_t>{ begin, end, 10 }, do_work,
			tbb::static_partitioner{});
}
} // namespace shrd
