#pragma once
#include "city_graph.h"
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

enum class SignalMode { FIXED, GREEDY, ADAPTIVE };

struct JunctionSignalState {
    int activeIncomingIndex = 0; // Index in the incoming edges vector
    int currentTick = 0;
};

class SignalController {
public:
    CityGraph& city;
    SignalMode mode;
    int fixedGreenTime = 4;
    unordered_map<int, JunctionSignalState> state;

    explicit SignalController(CityGraph& c, SignalMode m = SignalMode::FIXED)
        : city(c), mode(m) {
        initSignals();
    }

    void initSignals() {
        for (auto& pair : city.nodes) {
            int id = pair.first;
            state[id] = {0, 0};
            auto incoming = city.getIncomingEdges(id);
            for (size_t i = 0; i < incoming.size(); ++i) {
                incoming[i]->isGreen = (i == 0); // First incoming edge is green initially
                incoming[i]->greenDuration = fixedGreenTime;
            }
        }
    }

    void tick() {
        for (auto& pair : city.nodes) {
            int id = pair.first;
            auto incoming = city.getIncomingEdges(id);
            if (incoming.empty()) continue;

            auto& jss = state[id];
            jss.currentTick++;
            
            Edge* activeEdge = incoming[jss.activeIncomingIndex];
            
            if (jss.currentTick >= activeEdge->greenDuration) {
                jss.currentTick = 0;
                activeEdge->isGreen = false;
                
                if (mode == SignalMode::FIXED) {
                    jss.activeIncomingIndex = (jss.activeIncomingIndex + 1) % incoming.size();
                    activeEdge = incoming[jss.activeIncomingIndex];
                    activeEdge->greenDuration = fixedGreenTime;
                } 
                else if (mode == SignalMode::GREEDY) {
                    int maxDensity = -1;
                    int nextIdx = 0;
                    for (size_t i = 0; i < incoming.size(); ++i) {
                        if (incoming[i]->trafficDensity > maxDensity) {
                            maxDensity = incoming[i]->trafficDensity;
                            nextIdx = i;
                        }
                    }
                    jss.activeIncomingIndex = nextIdx;
                    activeEdge = incoming[nextIdx];
                    activeEdge->greenDuration = fixedGreenTime;
                }
                else if (mode == SignalMode::ADAPTIVE) {
                    jss.activeIncomingIndex = (jss.activeIncomingIndex + 1) % incoming.size();
                    activeEdge = incoming[jss.activeIncomingIndex];
                    // proportional to density: base + density / 2
                    activeEdge->greenDuration = max(2, fixedGreenTime + (activeEdge->trafficDensity / 2));
                }
                
                activeEdge->isGreen = true;
            }
        }
    }

    void setMode(SignalMode m) {
        mode = m;
        initSignals();
    }
};