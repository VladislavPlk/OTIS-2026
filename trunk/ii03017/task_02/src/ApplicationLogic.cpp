#include "ApplicationLogic.h"
#include <fstream>
#include <iomanip>
#include <ostream>
#include <string>

bool requiresIntegrationStep(const char* modelName)
{
    return modelName != nullptr && std::string(modelName).find("Model 3.7") != std::string::npos;
}

bool isValidSignalChoice(int choice)
{
    return choice >= 1 && choice <= 3;
}

bool isValidModelChoice(int choice)
{
    return choice >= 1 && choice <= 3;
}

void printResultsTable(std::ostream& out, const std::vector<Row>& rows)
{
    out << "\n" << std::setw(8) << "tau" << std::setw(15) << "u"
        << std::setw(15) << "y" << '\n';
    out << std::string(38, '-') << '\n';
    for (const Row& row : rows)
        out << std::setw(8) << row.tau
            << std::setw(15) << std::fixed << std::setprecision(6) << row.u
            << std::setw(15) << row.y << '\n';
}

bool saveResultsCsv(const std::string& path, const std::vector<Row>& rows)
{
    std::ofstream file(path);
    if (!file)
        return false;
    file << "tau,u,y\n";
    for (const Row& row : rows)
        file << row.tau << ',' << row.u << ',' << row.y << '\n';
    return static_cast<bool>(file);
}
