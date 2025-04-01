
#include <stxxl/map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <limits>
#include <thread>
#include  <cstdlib>

// Estructura para almacenar cada registro de diabetes
struct DiabetesData {
    int Diabetes_012;
    int HighBP;
    int HighChol;
    int CholCheck;
    int BMI;
    int Smoker;
    int Stroke;
    int HeartDiseaseorAttack;
    int PhysActivity;
    int Fruits;
    int Veggies;
    int HvyAlcoholConsump;
    int AnyHealthcare;
    int NoDocbcCost;
    int GenHlth;
    int MentHlth;
    int PhysHlth;
    int DiffWalk;
    int Sex;
    int Age;
    int Education;
    int Income;
    DiabetesData() = default;
    
    DiabetesData(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7, 
                int d8, int d9, int d10, int d11, int d12, int d13, int d14,
                int d15, int d16, int d17, int d18, int d19, int d20, int d21) {
        // Inicializar campos
    }
};
std::ostream& operator<<(std::ostream& os, const DiabetesData& data) {
    os << "DiabetesData(" 
       << data.Diabetes_012 << ", "
       << data.HighBP << ", "
       << data.HighChol << ", "
       << data.CholCheck << ", "
       << data.BMI << ", "
       << data.Smoker << ", "
       << data.Stroke << ", "
       << data.HeartDiseaseorAttack << ", "
       << data.PhysActivity << ", "
       << data.Fruits << ", "
       << data.Veggies << ", "
       << data.HvyAlcoholConsump << ", "
       << data.AnyHealthcare << ", "
       << data.NoDocbcCost << ", "
       << data.GenHlth << ", "
       << data.MentHlth << ", "
       << data.PhysHlth << ", "
       << data.DiffWalk << ", "
       << data.Sex << ", "
       << data.Age << ", "
       << data.Education << ", "
       << data.Income << ")";
    return os;
}
template <typename KeyType>
struct MyComparator : public std::less<KeyType> {
    static KeyType max_value() {
        return std::numeric_limits<KeyType>::max();
    }
};


// Configuración de los contenedores STXXL
using KeyType = uint64_t;
const unsigned BLOCK_SIZE = 1024 * 1024;

// Configuración para map1
const unsigned NODE_SIZE1 = 64 * 1024;
const unsigned LEAF_SIZE1 = 256 * 1024;
using MapType1 = stxxl::map<KeyType, DiabetesData, MyComparator<KeyType>, NODE_SIZE1, LEAF_SIZE1>;

// Configuración para map2
const unsigned NODE_SIZE2 = 256 * 1024;
const unsigned LEAF_SIZE2 = 1024 * 1024;
using MapType2 = stxxl::map<KeyType, DiabetesData, MyComparator<KeyType>, NODE_SIZE2, LEAF_SIZE2>;


// Función para parsear los datos del CSV
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
// Función para limpiar cachés del sistema
void clear_system_cache() {
    #ifdef _WIN32
    // Libera memoria programáticamente
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    const SIZE_T memSize = si.dwPageSize * 1000; // ~4MB
    void* p = VirtualAlloc(NULL, memSize, MEM_COMMIT, PAGE_READWRITE);
    if (p) {
        memset(p, 0, memSize);
        VirtualFree(p, 0, MEM_RELEASE);
    }
    #elif __linux__
    system("sync; echo 3 > /proc/sys/vm/drop_caches");
    #endif
}

int main() {
    const stxxl::unsigned_type CACHE_SIZE = 64 * 1024 * 1024; // 64MB
    
    MapType1 map1(CACHE_SIZE, CACHE_SIZE);
    MapType2 map2(CACHE_SIZE, CACHE_SIZE);

    KeyType key = 0;
    std::ifstream file("diabetes_012_health_indicators_BRFSS2015.csv");
    std::string line;
    std::getline(file, line); // Saltar encabezado

    // Limpiar caché antes de la primera inserción
    clear_system_cache();

    // ---- INSERCIÓN MAP1 CON DELAY ----
    auto start_insert1 = std::chrono::high_resolution_clock::now();
    while (std::getline(file, line)) {
        DiabetesData data = parse_line(line);
        map1.insert(std::make_pair(key, data));
        ++key;

        if (key % 100 == 0) {  // Delay cada 100 inserciones
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();  // Limpieza periódica
        }
    }
    auto end_insert1 = std::chrono::high_resolution_clock::now();

    // Limpiar caché antes de la segunda inserción
    clear_system_cache();

    // ---- INSERCIÓN MAP2 CON DELAY ----
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

    // ---- PREPARACIÓN BÚSQUEDAS ----
    std::vector<KeyType> random_keys;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<KeyType> dist(0, key-1);

    for (int i = 0; i < 10000; ++i) {  // 10,000 búsquedas para mejor medición
        random_keys.push_back(dist(gen));
    }

    // Limpiar caché antes de búsquedas
    clear_system_cache();

    // ---- BÚSQUEDA MAP1 ----
    auto start_lookup1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < random_keys.size(); ++i) {
        auto it = map1.find(random_keys[i]);
        
        if (i % 500 == 0) {  // Delay cada 500 búsquedas
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();
        }
    }
    auto end_lookup1 = std::chrono::high_resolution_clock::now();

    // Limpiar caché antes de segunda búsqueda
    clear_system_cache();

    // ---- BÚSQUEDA MAP2 ----
    auto start_lookup2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < random_keys.size(); ++i) {
        auto it = map2.find(random_keys[i]);
        
        if (i % 500 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            clear_system_cache();
        }
    }
    auto end_lookup2 = std::chrono::high_resolution_clock::now();

    // ---- RESULTADOS ----
    auto print_time = [](auto start, auto end, const std::string& name) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << name << ": " << duration.count() << " ms\n";
    };

    std::cout << "\nResultados de rendimiento (con limpieza de caché):\n";
    print_time(start_insert1, end_insert1, "Insercion contenedor 1");
    print_time(start_insert2, end_insert2, "Insercion contenedor 2");
    print_time(start_lookup1, end_lookup1, "Busqueda contenedor 1 ");
    print_time(start_lookup2, end_lookup2, "Busqueda contenedor 2 ");

    return 0;
}
