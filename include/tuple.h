#pragma once

#include <type_traits>
#include <utility>
#include "tuple_details.h"

//
// tuple with specified construction/destruction sequence of objects
//

namespace utils
{
	template <std::size_t I, typename T>
	struct tuple_element
	{
		tuple_element() = default;
		virtual ~tuple_element() = default;

		template <typename U,
			typename = typename std::enable_if<!std::is_same<U, tuple_element>::value>::type
		>
		tuple_element(U&& arg)
			: value(std::forward<U>(arg))
		{
		} 

		T value;
	};

	template <std::size_t... Indices, typename... Args>
	struct tuple_impl<std::index_sequence<Indices...>, Args...>
		: tuple_element<Indices, Args>...
	{
		tuple_impl() = default;

		template <typename... CompatibleTypes,
			typename = typename std::enable_if<!details::is_any_of<details::is_tuple_impl, std::decay_t<CompatibleTypes>...>()>::type
		>
			tuple_impl(CompatibleTypes&&... pack)
			: tuple_element<Indices, Args>(std::forward<CompatibleTypes>(pack))...
		{
		}

		template <typename... CompatibleTypes>
		tuple_impl(tuple_impl<std::index_sequence<Indices...>, CompatibleTypes...> const& tupleImpl)
			: tuple_element<Indices, Args>(static_cast<tuple_element<Indices, CompatibleTypes> const&>(tupleImpl).value)...
		{
		}
	};

	template <typename... Args>
	struct tuple
		: tuple_impl<std::index_sequence_for<Args...>, Args...>
	{
		using direct_base_t = tuple_impl<std::index_sequence_for<Args...>, Args...>;

		tuple() = default;
		tuple(tuple const&) = default;
		~tuple() = default;
		tuple& operator=(tuple const&) = default;
		tuple& operator=(tuple&&) = default;

		template <typename... CompatibleTypes,
			typename = typename std::enable_if<!details::is_any_of<details::is_tuple, std::decay_t<CompatibleTypes>...>()>::type
		>
			tuple(CompatibleTypes&&... pack)
			: direct_base_t(std::forward<CompatibleTypes>(pack)...)
		{
		}

		template <typename... OtherTypes>
		tuple(tuple<OtherTypes...> const& other)
			: direct_base_t(other)
		{
		}

		template <typename... OtherTypes>
		tuple(tuple<OtherTypes...>&& other)
			: direct_base_t(std::move(other))
		{
		}
	};

	// this metafunction extract type by specified index
	// in a template parameter pack

	//--------------------------------------------------------------------

	template <std::size_t Idx, typename Head, typename... Tail>
	struct type_at_index
	{
		using type = typename type_at_index<Idx - 1, Tail...>::type;
	};

	template <typename Head, typename... Tail>
	struct type_at_index<0, Head, Tail...>
	{
		using type = Head;
	};

	template <std::size_t Idx, typename... Args>
	using type_at_index_t = typename type_at_index<Idx, Args...>::type;

	// get by index implementation

	template <std::size_t Idx, typename... Args>
	constexpr type_at_index_t<Idx, Args...> const& get(tuple<Args...> const& tupleObject)
	{
		static_assert(Idx < sizeof...(Args), "Invalid index");

		return static_cast<tuple_element<Idx, type_at_index_t<Idx, Args...>> const&>(tupleObject).value;
	}

	template <std::size_t Idx, typename... Args>
	constexpr type_at_index_t<Idx, Args...>& get(tuple<Args...>& tupleObject)
	{
		static_assert(Idx < sizeof...(Args), "Invalid index");

		return static_cast<tuple_element<Idx, type_at_index_t<Idx, Args...>>&>(tupleObject).value;
	}

	// TODO: correct rvalue reference for return type
	template <std::size_t Idx, typename... Args>
	constexpr type_at_index_t<Idx, Args...> get(tuple<Args...>&& tupleObject)
	{
		static_assert(Idx < sizeof...(Args), "Invalid index");

		return static_cast<tuple_element<Idx, type_at_index_t<Idx, Args...>>&&>(tupleObject).value;
	}

	// get by type implementation

	template <typename SearchType, typename... Args>
	SearchType const& get(tuple<Args...> const& tupleObject)
	{
		static_assert(details::count_specified_type_in_pack<0, SearchType, Args...>() == 1,
			"Tuple must contain only one occurrence of specified type in pack");

		return static_cast<tuple_element<details::index_of_type_from_pack<SearchType, Args...>(), SearchType>&>(tupleObject).value;
	}

	template <typename SearchType, typename... Args>
	SearchType& get(tuple<Args...>& tupleObject)
	{
		static_assert(details::count_specified_type_in_pack<0, SearchType, Args...>() == 1,
			"Tuple must contain only one occurrence of specified type in pack");

		return static_cast<tuple_element<details::index_of_type_from_pack<SearchType, Args...>(), SearchType>&>(tupleObject).value;
	}

	template <typename SearchType, typename... Args>
	SearchType&& get(tuple<Args...>&& tupleObject)
	{
		static_assert(details::count_specified_type_in_pack<0, SearchType, Args...>() == 1,
			"Tuple must contain only one occurrence of specified type in pack");

		return static_cast<tuple_element<details::index_of_type_from_pack<SearchType, Args...>(), SearchType>&>(tupleObject).value;
	}
}