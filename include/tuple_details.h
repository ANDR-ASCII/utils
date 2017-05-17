#pragma once

namespace utils
{
	template <typename... Args>
	struct tuple;

	template <typename, typename...>
	struct tuple_impl;

	namespace details
	{
		template <template <typename> class>
		constexpr bool is_any_of()
		{
			return false;
		}

		template <template <typename> class Pred, typename Head, typename... Tail>
		constexpr bool is_any_of()
		{
			return Pred<Head>::value || is_any_of<Pred, Tail...>();
		}

		template <typename T>
		struct is_tuple : std::false_type
		{
		};

		template <typename... Args>
		struct is_tuple<tuple<Args...>> : std::true_type
		{
		};

		template <typename T>
		struct is_tuple_impl : std::false_type
		{
		};

		template <std::size_t... Idx, typename... Args>
		struct is_tuple_impl<tuple_impl<std::index_sequence<Idx...>, Args...>> : std::true_type
		{
		};

		// this two template functions counts T occurrences in Pack...
		template <std::size_t C, typename T, typename Head, typename... Pack>
		constexpr std::size_t count_specified_type_in_pack()
		{
			return std::is_same<T, Head>::value ?
				count_specified_type_in_pack<C + 1, T, Pack...>() :
				count_specified_type_in_pack<C, T, Pack...>();
		}

		template <std::size_t C, typename T>
		constexpr std::size_t count_specified_type_in_pack()
		{
			return C;
		}

		//--------------------------------------------------------------------------

		template <typename T, typename... Pack>
		struct first_type_from_pack_helper
		{
			using type = T;
		};

		template <typename... Pack>
		struct first_type_from_pack
		{
			using type = typename first_type_from_pack_helper<Pack...>::type;
		};

		template <typename... Pack>
		using first_type_from_pack_t = typename first_type_from_pack<Pack...>::type;

		//--------------------------------------------------------------------------

		template <typename T, typename Head, typename... Tail>
		constexpr std::size_t index_of_type_from_pack()
		{
			return std::is_same<T, Head>::value ? 0 : 1 + index_of_type_from_pack<T, Tail...>();
		}

		// stop condition
		template <typename>
		constexpr std::size_t index_of_type_from_pack()
		{
			return 0;
		}
	}
}

