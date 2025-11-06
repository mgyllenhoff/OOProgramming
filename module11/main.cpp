#include "Elevator.h"
#include <iomanip>

int main() {
    try {
        const std::string csvPath = "Mod10_Assignment_Elevators.csv";

        // Simulation with 10s per floor
        Simulation sim10(10);
        sim10.loadCSV(csvPath);
        std::cout << "Simulation with 10 seconds per floor" << std::endl;
        auto [avgWait10, avgTravel10] = sim10.run();
        std::cout << "\tAverage wait time:   " << avgWait10 << " s" << std::endl;
        std::cout << "\tAverage travel time: " << avgTravel10 << " s" << std::endl;

        // Simulation with 5s per floor
        Simulation sim5(5);
        sim5.loadCSV(csvPath);
        std::cout << "\nSimulation with 5 seconds per floor" << std::endl;
        auto [avgWait5, avgTravel5] = sim5.run();
        std::cout << "\tAverage wait time:   " << avgWait5 << " s" << std::endl;
        std::cout << "\tAverage travel time: " << avgTravel5 << " s" << std::endl;

        // Compute percentage improvement
        double avgWaitReduction = 100.0 * (avgWait10 - avgWait5) / avgWait10;
        double avgTravelReduction = 100.0 * (avgTravel10 - avgTravel5) / avgTravel10;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nReduction of average wait time: " << avgWaitReduction << "%\n";
        std::cout << "Reduction of average travel time: " << avgTravelReduction << "%\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
