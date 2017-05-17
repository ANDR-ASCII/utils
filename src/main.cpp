#include <unordered_set>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <functional>
#include "delegate.h"
#include "tuple.h"

int main()
{
	utils::tuple<int, double, float> tuple;

	utils::get<0>(tuple) = 9;
	utils::get<1>(tuple) = 10.006;
	utils::get<2>(tuple) = 11.003f;

	// checking TypeAtIndex metafunction vagueness implementation
	static_assert(std::is_same<utils::type_at_index_t<0, int, double, float>, int>::value, "Invalid type");
	static_assert(std::is_same<utils::type_at_index_t<1, int, double, float>, double>::value, "Invalid type");
	static_assert(std::is_same<utils::type_at_index_t<2, int, double, float>, float>::value, "Invalid type");

	std::cout << utils::get<0>(tuple) << std::endl;
	std::cout << utils::get<1>(tuple) << std::endl;
	std::cout << utils::get<2>(tuple) << std::endl;

	std::cin.get();
	return 0;
}