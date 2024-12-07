#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

bool is_safe(vector<int> report)
{
    vector<int> diffs;

    adjacent_difference(report.begin(), report.end(), back_inserter(diffs));
    diffs.erase(diffs.begin());

    if (all_of(diffs.begin(), diffs.end(), [](int n){return n > 0 and n <= 3;}) or 
        all_of(diffs.begin(), diffs.end(), [](int n){return n < 0 and n >= -3;}) )
    {
        return true;
    }     

    return false;
}


bool is_tolerable(vector<int> report)
{
    if (is_safe(report)) return true;

    vector<vector<int>> report_vars;

    for (size_t i = 0; i < report.size(); i++)
    {
        report_vars.push_back(report);
        report_vars.back().erase(report_vars.back().begin() + i);
    }      
    for (size_t i = 0; i < report_vars.size(); i++)
    {
        if (is_safe(report_vars.at(i))) return true;
    }  

    return false;
}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;

    ifstream file;
    file.open(argv[1]);
    
    string line;
    int safe = 0;
    int tolerable = 0;

    while (getline(file, line))
    {
        istringstream lstream(line);

        vector<int> report;

        for (int n; lstream >> n;)
        {
            report.push_back(n);
        }

        if (is_safe(report))
        {
            safe++;
        }
        if (is_tolerable(report))
        {
            tolerable++;
        }
    }

    cout << "Safe: " << safe << "\n";
    cout << "Tolerable: " << tolerable << "\n";

    file.close();

    return 0;
}