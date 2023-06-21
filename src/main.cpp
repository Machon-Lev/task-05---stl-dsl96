
#include"citiesMap.h"
int main() {
     
    auto cityMap = citiesMap();

   
    cityMap.readCitiesFromFile("data.txt");
    
    while (true) {
        std::string cityName;
        double radius;
        int distanceCalc;

        std::cout << "Enter city name (or 0 to exit): ";
        std::getline(std::cin, cityName);

        if (cityName == "0")
            break;

        std::cout << "Enter search radius: ";
        std::cin >> radius;

        std::cout << "Enter distance calculation option (0: Euclidean, 1: Chebyshev, 2: Manhattan): ";
        std::cin >> distanceCalc;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            cityMap.closeCities(cityName, radius, distanceCalc);
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: City not found." << std::endl;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}

  
 