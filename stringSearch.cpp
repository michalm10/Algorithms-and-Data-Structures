#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>

using namespace std;

vector<unsigned int> stringSearch(const string str, const string pattern) {
    vector<unsigned int> positions;
    for (unsigned int i = 0; i < str.size() - pattern.size() + 1; i++) {
        unsigned int j = 0;
        if (pattern[j] == str[i + j] && str.compare(i, pattern.size(), pattern) == 0)
            positions.push_back(i);
    }
    return positions;
}

namespace Rabin_Karp {
    class Hash {
    public:
        int mPower;
        int m, d, q;
        Hash(int m, int d, int q): m{m}, d{d}, q{q} {
            int pow = 1;
            for (int i = 1; i < m; i++)
                pow = (pow * d) % q;
            mPower = pow;
        }
        int hash(string::iterator it1, string::iterator it2) {
            int hashValue = 0;
            while (it1 != it2) {
                hashValue = (hashValue * d + *it1) % q;
                it1++;
            }
            return hashValue;
        }
        int hash(string::iterator it1, string::iterator it2, int prevHash) {
            int temp = (*(it1 - 1) * mPower) % q;
            prevHash += prevHash < temp ? q : 0;
            return ((prevHash - temp) * d + *(it2 - 1)) % q;
        }
    };
    vector<unsigned int> stringSearch(string str, string pattern);
}

vector<unsigned int> Rabin_Karp::stringSearch(string str, string pattern) {
    vector<unsigned int> positions;
    Hash H = Hash(pattern.size(), 128, 27077);
    int p = H.hash(pattern.begin(), pattern.end());
    int t = H.hash(str.begin(), str.begin() + pattern.size());
    for (unsigned int i = 0; i < str.size() - pattern.size() + 1; i++) {
        if (p == t && str.compare(i, pattern.size(), pattern) == 0)
            positions.push_back(i);
        if (i < str.size() - pattern.size())
            t = H.hash(str.begin() + i + 1, str.begin() + i + 1 + pattern.size(), t);
    }
    return positions;
}

namespace KMP {
    vector<int> prefixTable(string pattern) {
        vector<int> pref(pattern.size(), 0);
        for (unsigned int i = 1; i < pattern.size(); i++) {
            int k = pref[i - 1];
            while (k > 0 && pattern[k] != pattern[i])
                k = pref[k - 1];
            if (pattern[k] == pattern[i])
                k++;
            pref[i] = k;
        }
        return pref;
    }
    vector<unsigned int> stringSearch(string str, string pattern);
}

vector<unsigned int> KMP::stringSearch(string str, string pattern) {
    vector<unsigned int> positions;
    vector<int> pref = prefixTable(pattern);
    int q = 0;
    for (unsigned int i = 0; i < str.size(); i++) {
        while (q > 0 && pattern[q] != str[i])
            q = pref[q - 1];
        if (pattern[q] == str[i]) q++;
        if (q == (int)pattern.size()) {
            positions.push_back(i - pattern.size() + 1);
            q = pref[q - 1];
        }
    }
    return positions;
}

string readFile(string fname) {
    ifstream in;
    in.open(fname, ios::in);
	if (!in.is_open()) {
		cerr << "Nie udalo sie otworzyc" << endl;
		exit(1);
	}
    string line;
    string text;
    while (getline(in, line))
        text += line;
    in.close();
    return text;
}

int main () {
    string text1 = readFile("text1.txt");
    string pattern1 = readFile("pattern1.txt");
    string text2 = readFile("text2.txt");
    string pattern2 = readFile("pattern2.txt");

    cout << "Wprost\n";
    auto start = chrono::steady_clock::now();
    auto pos = stringSearch(text1, pattern1);
    auto end = chrono::steady_clock::now();
    for (auto x : pos)
        cout << x << " ";

    cout << "\nMilisekund: " << chrono::duration_cast<chrono::microseconds>(end - start).count() * 1.0 / 1000 << endl;

    cout << "\nRabin-Karp\n";
    start = chrono::steady_clock::now();
    pos = Rabin_Karp::stringSearch(text1, pattern1);
    end = chrono::steady_clock::now();
    for (auto x : pos)
        cout << x << " ";

    cout << "\nMilisekund: " << chrono::duration_cast<chrono::microseconds>(end - start).count() * 1.0 / 1000 << endl;

    cout << "\nKMP\n";
    start = chrono::steady_clock::now();
    pos = KMP::stringSearch(text1, pattern1);
    end = chrono::steady_clock::now();
    for (auto x : pos)
        cout << x << " ";

    cout << "\nMilisekund: " << chrono::duration_cast<chrono::microseconds>(end - start).count() * 1.0 / 1000 << endl;

    return 0;
}
