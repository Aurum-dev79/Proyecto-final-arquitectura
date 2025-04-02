#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <stxxl/map>
#include <limits>
#include <iostream>

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
                int d15, int d16, int d17, int d18, int d19, int d20, int d21) :
        Diabetes_012(d0), HighBP(d1), HighChol(d2), CholCheck(d3), BMI(d4),
        Smoker(d5), Stroke(d6), HeartDiseaseorAttack(d7), PhysActivity(d8),
        Fruits(d9), Veggies(d10), HvyAlcoholConsump(d11), AnyHealthcare(d12),
        NoDocbcCost(d13), GenHlth(d14), MentHlth(d15), PhysHlth(d16),
        DiffWalk(d17), Sex(d18), Age(d19), Education(d20), Income(d21) {}
};

// Operador de salida para DiabetesData
inline std::ostream& operator<<(std::ostream& os, const DiabetesData& data) {
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

// Comparador personalizado
template <typename KeyType>
struct MyComparator : public std::less<KeyType> {
    static KeyType max_value() {
        return std::numeric_limits<KeyType>::max();
    }
};

// Configuración de tipos
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

#endif // DATA_TYPES_H