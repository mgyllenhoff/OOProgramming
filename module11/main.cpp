#include "Elevator.h"
#include <iomanip>

int main() {
    try {
        const std::string csvPath = "Mod10_Assignment_Elevators.csv";

        // Run normal system (10 sec per floor)
        Simulation simNormal(10);
        simNormal.loadCSV(csvPath);
        std::cout << "=== Simulation: 10 seconds per floor ===\n";
        auto [wait10, travel10] = simNormal.run();

        // // Run upgraded system (5 sec per floor)
        // Simulation simFast(5);
        // simFast.loadCSV(csvPath);
        // std::cout << "\n=== Simulation: 5 seconds per floor (upgraded) ===\n";
        // auto [wait5, travel5] = simFast.run();

        // // Compute percentage improvement
        // double waitReduction = 100.0 * (wait10 - wait5) / wait10;
        // double travelReduction = 100.0 * (travel10 - travel5) / travel10;

        // std::cout << std::fixed << std::setprecision(2);
        // std::cout << "\n% reduction in average wait time: " << waitReduction << "%\n";
        // std::cout << "% reduction in average travel time: " << travelReduction << "%\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
