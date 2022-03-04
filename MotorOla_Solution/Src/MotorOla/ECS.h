#pragma once
#include <array>
#include <iostream>
#include <bitset>
#include <memory>

class Entidad;
class Componente;
using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept {
	static const ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t MAX_COMPONENTS = 32;

using ComponentBitSet = std::bitset<MAX_COMPONENTS>;
using ComponentArray = std::array<Componente*, MAX_COMPONENTS>;