#include "city_graph.h"
#include "dynamic_weight.h"
#include "traffic_simulator.h"
#include "signal_controller.h"
#include "emergency_routing.h"
#include "visualization_controller.h"
#include <iostream>

using namespace std;

void displayMenu() {
    cout << "\n=========================================\n";
    cout << "  SMART TRAFFIC MANAGEMENT SYSTEM MENU\n";
    cout << "=========================================\n";
    cout << " 1. Show City Graph\n";
    cout << " 2. Add vehicles manually to a road\n";
    cout << " 3. Generate random traffic\n";
    cout << " 4. Simulate traffic & signals (advance 1 tick)\n";
    cout << " 5. Find shortest path (Dijkstra & BFS)\n";
    cout << " 6. Show most congested junctions\n";
    cout << " 7. Simulate accident (add traffic to an edge)\n";
    cout << " 8. Exit\n";
    cout << "=========================================\n";
    cout << "Select an option: ";
}

int main() {
    CityGraph city = buildSampleCity();
    DynamicWeightManager dwm(city);
    TrafficSimulator sim(city);
    SignalController sc(city, SignalMode::FIXED);
    EmergencyRouting routing(city);
    VisualizationController vc(city, sc);

    // Initial setup
    dwm.update();
    city.updateNodeVehicleCounts();

    bool running = true;
    while (running) {
        displayMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                vc.printCityGraph();
                break;
            case 2: {
                int from, to, count;
                cout << "Enter source node ID (from): "; cin >> from;
                cout << "Enter destination node ID (to): "; cin >> to;
                cout << "Enter number of vehicles: "; cin >> count;
                sim.addVehicleToEdge(from, to, count);
                dwm.update();
                cout << "Vehicles added. Density updated.\n";
                break;
            }
            case 3: {
                int count;
                cout << "Enter number of random vehicles to generate overall: "; cin >> count;
                sim.generateRandomTraffic(count);
                dwm.update();
                cout << "Random traffic generated across the city.\n";
                break;
            }
            case 4: {
                int mode;
                cout << "Select Signal Mode (0=FIXED, 1=GREEDY, 2=ADAPTIVE): "; cin >> mode;
                if (mode >= 0 && mode <= 2) {
                    sc.setMode(static_cast<SignalMode>(mode));
                }
                sc.tick();
                sim.tick();
                dwm.update();
                cout << "\n--- Simulation advanced by 1 tick ---\n";
                vc.printCityGraph();
                break;
            }
            case 5: {
                int src, dest;
                cout << "Enter source node ID: "; cin >> src;
                cout << "Enter destination node ID: "; cin >> dest;
                
                vector<int> pathD = routing.findShortestPathDijkstra(src, dest);
                vc.printPath("Dijkstra", pathD);
                
                vector<int> pathB = routing.findShortestPathBFS(src, dest);
                vc.printPath("BFS", pathB);
                break;
            }
            case 6:
                vc.printTopCongestedJunctions();
                break;
            case 7: {
                int from, to, count;
                cout << "Accident Simulator:\n";
                cout << "Enter source node ID of accident edge: "; cin >> from;
                cout << "Enter destination node ID of accident edge: "; cin >> to;
                cout << "Enter traffic density penalty to add: "; cin >> count;
                dwm.simulateAccident(from, to, count);
                dwm.update();
                cout << "Accident simulated. New edge weights applied.\n";
                break;
            }
            case 8:
                running = false;
                break;
            default:
                cout << "Invalid option. Please try again.\n";
                break;
        }
    }

    cout << "Exiting system.\n";
    return 0;
}