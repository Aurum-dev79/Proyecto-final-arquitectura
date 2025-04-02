#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "data_types.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// Función para parsear los datos del CSV
DiabetesData parse_line(const std::string& line);

// Función para limpiar cachés del sistema
void clear_system_cache();

// Función para generar claves aleatorias
std::vector<KeyType> generate_random_keys(const MapType1& map, size_t count);

// Función para realizar inserciones
void perform_insertions(MapType1& map1, MapType2& map2, const std::string& filename);

// Función para realizar búsquedas
void perform_lookups(MapType1& map1, MapType2& map2, const std::vector<KeyType>& random_keys);

// Función para imprimir tiempos
template <typename TimePoint>
void print_time(TimePoint start, TimePoint end, const std::string& name);

#endif // OPERATIONS_H