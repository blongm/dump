#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <numeric>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 1) return 1;

    ifstream file;
    file.open(argv[1]);

    vector<int> l;
    vector<int> r;

    string line;

    while (getline(file, line))
    {
        istringstream lstream(line);
        
        int n;

        lstream >> n;
        l.push_back(n);

        lstream >> n;
        r.push_back(n);
    }

    file.close();

    sort(l.begin(), l.end());
    sort(r.begin(), r.end());

    vector<int> distances;
    vector<int> similarites;

    transform(l.begin(), l.end(), r.begin(), back_inserter(distances), [](int a, int b){return abs(a-b);} );
    transform(l.begin(), l.end(), back_inserter(similarites), [r](int n){return n * count(r.begin(), r.end(), n);});
    
    cout << "Dist: " << reduce(distances.begin(), distances.end(), 0) << "\n";
    cout << "Similarity: " << reduce(similarites.begin(), similarites.end(), 0) << "\n";
    
    return 0;
}