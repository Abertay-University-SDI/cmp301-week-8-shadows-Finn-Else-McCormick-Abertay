#pragma once

#include <typeindex>
#include <typeinfo>

template <typename T>
std::type_index type_index() {
	return std::type_index(typeid(T));
}