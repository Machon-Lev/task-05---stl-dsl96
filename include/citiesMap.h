#pragma once
#include <iostream>
#include <string>
#include <map>
#include <functional>

using DistanceFunction = std::function<double(double, double, double, double)>;
class citiesMap {

private:

    struct coordinate {
        double x;
        double y;


        std::string toString() const {
            return "Coordinates (x, y): " + std::to_string(x) + ", " + std::to_string(y);
        }
        friend std::ostream& operator<<(std::ostream& os, const coordinate& city)
        {
            os <<city.toString() << std::endl;
            return os;
        }

      
    };


    std::map<std::string, coordinate> cityMap;
  
    std::map<int, DistanceFunction> distanceFunctions;

    void initilizedistanceFunctions();

public:

    citiesMap() {
        this->initilizedistanceFunctions();
    }
    
    void insertOrUpdate(std::string name, double x, double y);

    void printCities(std::map<std::string, coordinate>::const_iterator beginIter, std::map<std::string, coordinate>::const_iterator endIter)const;

    void printAllCities()const;

    void readCitiesFromFile(std::string path);

    void closeCities(std::string cityName, double radius, int distanceCalc) const;

     
};
