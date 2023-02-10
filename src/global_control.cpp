#include <chrono>
#include <cstdio>
#include <fea/numerics/random.hpp>
#include <gtest/gtest.h>
#include <numeric>
#include <shared.hpp>
#include <tbb/combinable.h>
#include <tbb/global_control.h>
#include <tbb/parallel_for.h>
#include <thread>

namespace {
using partitioner_t = tbb::static_partitioner;
tbb::blocked_range<size_t> block_range{ 0, 10'000, 100 };
// constexpr std::chrono::milliseconds sleep_time(1);

TEST(reeantrance, global_control_off) {
	std::string help = R"xx(
	Demos nested tbb parallel_for calls rescheduling the current thread before
	completing its work. Thus creating a recursive re-entrance. Not
	*necessarily* a bug, but very surprising and an absolute PITA to debug ;)

	Offering a tbb structure that has state would allow us to fix this :

	tbb::rock_solid_combinable<bool> comb; // doesn't offer .local()
	tbb::combinable_lock<bool> l(comb); // a-la std::lock_guard and friends.
	bool& mbool = l.local();

	With this pattern, rock_solid_combinable can allow recursiveness and create
	new storage for re-entering threads.
)xx";
	printf("%s\n", help.c_str());

	tbb::combinable<bool> mt_bools;
	auto lvl1_work = [&](const tbb::blocked_range<size_t>& range) {
		bool& thread_bool = mt_bools.local();
		if (thread_bool == true) {
			// Our thread has already locked the bool previously,
			// yet hasn't completed. Re-entrance.
			printf("-- Re-entering un-completed thread task. Abort!\n\n");
			// tbb::task::current_context()->cancel_group_execution();
			return;
		}

		thread_bool = true;
		for (size_t i = range.begin(); i < range.end(); ++i) {
			// Absolutely magical work, unicorns, rainbows.
			shrd::do_threaded_work(range.begin(), range.end());
		}
		thread_bool = false;
	};

	tbb::parallel_for(block_range, lvl1_work, partitioner_t{});
}

TEST(reeantrance, global_control_on) {
	std::string help = R"xx(
	Demos same nested re-entrance for a currently in-process thread,
	even though we have turned on global control to limit the nested
	threading (max_allowed_parallelism == 1).

	Global control set to 1 should simply execute its callback in-line and
	return from the caller thread. It shouldn't allow rescheduling current
	thread before it has had time to exit.
)xx";
	printf("%s\n", help.c_str());


	tbb::combinable<bool> mt_bools;
	auto lvl1_work = [&](const tbb::blocked_range<size_t>& range) {
		bool& thread_bool = mt_bools.local();
		if (thread_bool == true) {
			printf("-- Re-entering un-completed thread task. Abort!\n\n");
			// tbb::task::current_context()->cancel_group_execution();
			return;
		}

		thread_bool = true;
		for (size_t i = range.begin(); i < range.end(); ++i) {
			// Absolutely magical work, unicorns, rainbows.
			shrd::do_threaded_work(range.begin(), range.end());
		}
		thread_bool = false;
	};

	tbb::global_control g(tbb::global_control::max_allowed_parallelism, 1);
	tbb::parallel_for(block_range, lvl1_work, partitioner_t{});
}
} // namespace