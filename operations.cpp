#include "operations.h"
#include <cstdlib>

DiabetesData parse_line(const std::string& line) {
    std::istringstream iss(line);
    std::string field;
    std::vector<int> fields;
    
    while (std::getline(iss, field, '\t')) {
        fields.push_back(std::stoi(field));
    }

    return DiabetesData{
        fields[0], fields[1], fields[2], fields[3], fields[4],
        fields[5], fields[6], fields[7], fields[8], fields[9],
        fields[10], fields[11], fields[12], fields[13], fields[14],
        fields[15], fields[16], fields[17], fields[18], fields[19],
        fields[20], fields[21]
    };
}

void clear_system_cache() {
    #ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    const SIZE_T memSize = si.dwPageSize * 1000;
    void* p = VirtualAlloc(NULL, memSize, MEM_COMMIT, PAGE_READWRITE);
    if (p) {
        memset(p, 0, memSize);
        VirtualFree(p, 0, MEM_RELEASE);
    }
    #elif __linux__
    system("sync; echo 3 > /proc/sys/vm/drop_caches");
    #endif
}

std::vector<KeyType> generate_random_keys(const MapType1& map, size_t count) {
    std::vector<KeyType> random_keys;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<KeyType> dist(0, map.size()-1);

    for (size_t i = 0; i < count; ++i) {
        random_keys.push_back(dist(gen));
    }
    return random_keys;
}

void perform_insertions(MapType1& map1, MapType2& map2, const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line); // Saltar encabezado
    
    KeyType key = 0;
    
    clear_system_cache();

    // Inserción en map1
    auto start_insert1 = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        DiabetesData data = parse_line(line);
        map1.insert(std::make_pair(key, data));
        ++key;

        if (key % 100 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();
        }
    }
    auto end_insert1 = std::chrono::high_resolution_clock::now();

    clear_system_cache();

    // Inserción en map2
    file.clear();
    file.seekg(0);
    std::getline(file, line);
    key = 0;
    
    auto start_insert2 = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        DiabetesData data = parse_line(line);
        map2.insert(std::make_pair(key, data));
        ++key;

        if (key % 100 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();
        }
    }
    auto end_insert2 = std::chrono::high_resolution_clock::now();

    std::cout << "\nResultados de insercion:\n";
    print_time(start_insert1, end_insert1, "Insercion contenedor 1");
    print_time(start_insert2, end_insert2, "Insercion contenedor 2");
}

void perform_lookups(MapType1& map1, MapType2& map2, const std::vector<KeyType>& random_keys) {
    clear_system_cache();

    // Búsqueda en map1
    auto start_lookup1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < random_keys.size(); ++i) {
        auto it = map1.find(random_keys[i]);
        
        if (i % 500 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();
        }
    }
    auto end_lookup1 = std::chrono::high_resolution_clock::now();

    clear_system_cache();

    // Búsqueda en map2
    auto start_lookup2 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < random_keys.size(); ++i) {
        auto it = map2.find(random_keys[i]);
        
        if (i % 500 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();
        }
    }
    auto end_lookup2 = std::chrono::high_resolution_clock::now();

    std::cout << "\nResultados de busqueda:\n";
    print_time(start_lookup1, end_lookup1, "Busqueda contenedor 1");
    print_time(start_lookup2, end_lookup2, "Busqueda contenedor 2");
}

template <typename TimePoint>
void print_time(TimePoint start, TimePoint end, const std::string& name) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << name << ": " << duration.count() << " ms\n";
}