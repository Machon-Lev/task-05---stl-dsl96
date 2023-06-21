#include "citiesMap.h"
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
 

void citiesMap::insertOrUpdate(std::string cityName, double x, double y)
{	 
	coordinate newCity = {  x, y };

	this->cityMap.insert({ cityName,newCity });
}

void citiesMap::printCities(std::map<std::string, coordinate>::const_iterator beginIter, std::map<std::string, coordinate>::const_iterator endIter) const
{
   
    std::transform(beginIter, endIter, std::ostream_iterator< std::string >(std::cout, "\n"),
        [](const auto& pair) {
            return  "name: "+ pair.first + " " + pair.second.toString();
        });
}



void citiesMap::printAllCities()const
{
    this->printCities(this->cityMap.cbegin(), this->cityMap.cend());
}


double calculateEuclideanDistance(double x0, double y0, double x1, double y1) {
    return std::sqrt(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2));
}

double calculateChebyshevDistance(double x0, double y0, double x1, double y1) {
    return std::max(std::abs(x0 - x1), std::abs(y0 - y1));
}

double calculateManhattanDistance(double x0, double y0, double x1, double y1) {
    return std::abs(x0 - x1) + std::abs(y0 - y1);
}

void citiesMap::initilizedistanceFunctions()
{
     this->distanceFunctions = {
        {0, calculateEuclideanDistance},
        {1, calculateChebyshevDistance},
        {2, calculateManhattanDistance}
    };
}



void citiesMap::readCitiesFromFile(std::string filename)
{
   
    std::ifstream file(filename);
    std::string line;
    std::string name;
    double x, y;

    while (std::getline(file, line)) {
        name = line;

        if (!std::getline(file, line)) {
            throw std::runtime_error("Invalid data format in file: " + filename);
        }

        std::istringstream iss(line);
        std::string xStr, yStr;
        if (!(std::getline(iss, xStr, '-') && std::getline(iss, yStr))) {
            throw std::runtime_error("Invalid data format in file: " + filename);
        }

        std::istringstream xIss(xStr);
        std::istringstream yIss(yStr);
        if (!(xIss >> x) || !(yIss >> y)) {
            throw std::runtime_error("Invalid data format in file: " + filename);
        }

        this->insertOrUpdate(name, x, y);
    }

    
}

bool inSquare(double x0, double y0, double x1, double y1,double radius) {
    return   std::max(std::abs(x0 - x1), std::abs(y0 - y1)) <= radius;
}

void citiesMap::closeCities( std::string  cityName, double radius, int distanceCalc) const
{
    auto chosenCity  = this->cityMap.find(cityName);

    if (chosenCity == this->cityMap.end()) {
        throw std::runtime_error("City not found in the map");
    }

    if (this->distanceFunctions.find(distanceCalc) == this->distanceFunctions.end()) {
        throw std::runtime_error("invalide distance calc function");
    }
   

    int northCity = 0;

    std::vector<std::pair<std::string,  coordinate>> closeCity;

    std::copy_if(this->cityMap.begin(), this->cityMap.end(), std::back_inserter(closeCity),
        [=, &northCity](const std::pair<std::string, coordinate> city) {


            if (city.first != chosenCity->first &&
                inSquare(chosenCity->second.x, chosenCity->second.y,  city.second.x, city.second.y, radius) &&
                this->distanceFunctions.at(distanceCalc)(chosenCity->second.x, chosenCity->second.y,
                    city.second.x, city.second.y) < radius)
            {
                if (chosenCity->second.y > city.second.y)
                    ++northCity;

                return true;
            }
            return false;
        });


    

    std::sort(closeCity.begin(), closeCity.end(), [=](const auto& city1, const auto& city2) {
        return this->distanceFunctions.at(distanceCalc)(chosenCity->second.x, chosenCity->second.y,
        city1.second.x, city1.second.y) <
        this->distanceFunctions.at(distanceCalc)(chosenCity->second.x, chosenCity->second.y,
            city2.second.x, city2.second.y);
            });


    std::cout << "Search result:" << std::endl;
    std::cout <<  closeCity.size() << " city/cities found in the given radius." << std::endl;
    std::cout <<  northCity<< " cities are to the north of the selected city." << std::endl;
    std::cout << "City list:" << std::endl;
    

    std::transform(closeCity.begin(), closeCity.end(), std::ostream_iterator<std::string>(std::cout, "\n"),
        [](const auto& element) {
            return element.first;
        });

}

 