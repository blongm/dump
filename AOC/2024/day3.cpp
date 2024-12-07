#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

array<int, 2> ins_nums(string s)
{
    istringstream stream(s);
    int a; int b;

    stream >> a;
    stream >> b;

    return {a, b};

}

int main(int argc, char** argv)
{
    if (argc < 1) return 1;

    ifstream file(argv[1]);
    stringstream buffer;
    buffer << file.rdbuf();
    string text = buffer.str();

    file.close();

    regex pattern("mul\\(\\d{1,3},\\d{1,3}\\)");

    vector<string> matches;

    copy(sregex_token_iterator(text.begin(), text.end(), pattern), sregex_token_iterator(), back_inserter(matches));

    vector<array<int, 2>> nums;  

    for_each(matches.begin(), matches.end(), [](string& s){s.erase(0, 4); replace(s.begin(), s.end(), ',', ' ');});



    cout << matches.at(0) << '\n';

    return 0;
}