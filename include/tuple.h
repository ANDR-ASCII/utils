#pragma once

#include "std_headers.hpp"
#include "tuple_details.h"


namespace utils
{

template <typename T, size_t N>
struct tuple_element
{
	T value;
};

template <typename, typename...>
struct tuple_impl;

template <size_t... Idx, typename... Args>
struct tuple_impl<std::index_sequence<Idx...>, Args...> 
	: public tuple_element<Args, Idx>...
{
};

template <typename... Args>
struct tuple : public tuple_impl<Args, std::index_sequence_for<sizeof...(Args)>>
{

};

}