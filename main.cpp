#include "data_types.h"
#include "operations.h"
#include <iostream>

int main() {
    const stxxl::unsigned_type CACHE_SIZE = 64 * 1024 * 1024; // 64MB
    
    MapType1 map1(CACHE_SIZE, CACHE_SIZE);
    MapType2 map2(CACHE_SIZE, CACHE_SIZE);

    // Realizar inserciones
    perform_insertions(map1, map2, "diabetes_012_health_indicators_BRFSS2015.csv");

    // Generar claves aleatorias para búsqueda
    auto random_keys = generate_random_keys(map1, 10000);

    // Realizar búsquedas
    perform_lookups(map1, map2, random_keys);

    return 0;
}