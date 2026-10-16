#pragma once
#include <iosfwd>
#include <string>
#include <vector>
#include "Simulation.h"

// Small, deterministic application helpers separated from interactive main().
bool requiresIntegrationStep(const char* modelName);
bool isValidSignalChoice(int choice);
bool isValidModelChoice(int choice);
void printResultsTable(std::ostream& out, const std::vector<Row>& rows);
bool saveResultsCsv(const std::string& path, const std::vector<Row>& rows);
