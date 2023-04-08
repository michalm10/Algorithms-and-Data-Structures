#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <array>
#include <cmath>
#include <iomanip>

using namespace std;

class twoDimLP {
public:
    double objective[2];
    vector<array<double, 3> > constraintsBase;
    std::mt19937::result_type seed; 

    twoDimLP(string filename) {
        ifstream in;
        char c;
        in.open(filename, ios::in);
        if (!in.is_open()) {
            cout << "Nie udalo się otworzyć";
            exit(1);
        }
        in >> objective[0] >> objective[1];
        int i = 0;
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            array<double, 3> b;
            ss >> b[0] >> b[1] >> b[2];
            if (ss.fail()) 
                continue;
            constraintsBase.push_back(b);
        }
        std::random_device rd;
        seed = rd() ^ (
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count() +
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count() );              
    }

    double objValue(double x1, double x2) {
        return x1 * objective[0] + x2 * objective[1];
    }

    pair<vector<double>, string> solve() {
        vector<double> cos;
        double objVecLen = sqrt(objective[0] * objective[0] + objective[1] * objective[1]);
        double cos1, cos2;
        cos1 = cos2 = -10000000000;
        int ind1, ind2;
        ind1 = ind2 = -1;
        int i = 0;
        for (auto con : constraintsBase) {
            double temp = con[0] * con[0] + con[1] * con[1];
            temp = (con[0] * objective[0] + con[1] * objective[1]) / sqrt(temp) / objVecLen;
            cos.push_back(temp);
            if (temp > cos1) {
                cos1 = temp;
                ind1 = i;
            }
            i++;
        }
        i = 0;
        bool noSol = true;
        double m;
        for (auto c : cos) {
            if ((constraintsBase[ind1][0] == -constraintsBase[i][0] 
                && constraintsBase[ind1][1] == -constraintsBase[i][1]) 
                && (constraintsBase[ind1][1] > 0 ? constraintsBase[ind1][2] < -constraintsBase[i][2] 
                : -constraintsBase[ind1][2] > constraintsBase[i][2]))
                return {{0, 0}, "Niedopuszczalny"};
            double temp = sol(constraintsBase[ind1], constraintsBase[i]);
            if (i != ind1 && acos(cos1) + acos(c) < _Pi && (noSol || temp >= m)) {
                cos2 = c;
                ind2 = i;
                m = temp;
                noSol = false;
            }
            i++;
        }
        if (cos1 <= 0 || ind2 < 0) return {{0, 0}, "Nieograniczony"};
        swap(constraintsBase[0], constraintsBase[ind1]);
        if (ind2 == 0) ind2 = ind1;
        swap(constraintsBase[1], constraintsBase[ind2]);
        string mes = "";
        return {solveSeidel(constraintsBase, 2, mes), mes};
    }

    double sol(array<double, 3> e1, array<double, 3> e2);
    vector<double> solveSeidel(vector<array<double, 3> >& constraints, int dim, string& mes);
};

vector<double> twoDimLP::solveSeidel(vector<array<double, 3> >& constraints, int dim, string& mes) {
    vector<double> x;
    if (dim == 1) {
        vector<pair<double, bool> > xt;
        double lp, rp;
        bool hasLp = false, hasRp = false;
        for (auto con : constraints)  
            if (con[0] != 0) xt.push_back({con[2] / con[0], con[0] >= 0 ? 1 : 0}); 
        if (xt.size() == 0) return {0, 0};
        lp = rp = xt[0].first;
        for (auto con : xt) {
            if ((con.first <= rp || !hasRp) && con.second) {
                rp = con.first;
                hasRp = true;
            }
            else if ((con.first >= lp || !hasLp) && !con.second) {
                lp = con.first;
                hasLp = true;
            }
        }
        if (lp > rp && hasRp && hasLp) {
            cout << "Niedopuszczalny" << endl;
            exit(1);
        }
        if (hasLp) x.push_back(lp);
        if (hasRp) x.push_back(rp);
        if (x.size() == 1 && mes.size() == 0) mes = hasLp ? 'l' : 'r';
        return x;
    }

    if (constraints.size() == 2 && dim == 2) {
        double det = constraints[0][0] * constraints[1][1] - constraints[0][1] * constraints[1][0];
        double inv[][2] = {{constraints[1][1] / det, -constraints[0][1] / det},
        {-constraints[1][0] / det, constraints[0][0] / det}};
        x.push_back(inv[0][0] * constraints[0][2] + inv[0][1] * constraints[1][2]);
        x.push_back(inv[1][0] * constraints[0][2] + inv[1][1] * constraints[1][2]);
        //cout << constraints[0][0] << constraints[0][1] << constraints[1][0] << constraints[1][1] << endl;
        return x;
    }

    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned> distrib(2, constraints.size() - 1);
    int index = distrib(gen);

    array<double, 3> h = constraints[index];
    vector<array<double, 3> > newConstraints(constraints);
    newConstraints.erase(newConstraints.begin() + index);
    x = solveSeidel(newConstraints, dim, mes);
    double sum = x[0] * h[0] + x[1] * h[1];
    if (sum <= h[2]) return x;

    if (h[1] != 0 && h[1] != -0)
        for (int i = 0; i < newConstraints.size(); i++) {
            newConstraints[i][0] += newConstraints[i][1] * (-h[0] / h[1]);
            newConstraints[i][2] -= newConstraints[i][1] * (h[2] / h[1]);
        }
    else {
        for (int i = 0; i < newConstraints.size(); i++) {
            newConstraints[i][2] -= newConstraints[i][0] * (h[2] / h[0]); 
            newConstraints[i][0] = newConstraints[i][1];
        }
    }
    x = solveSeidel(newConstraints, dim - 1, mes);
    if (h[1] != 0)
        if (x.size() == 1) 
            x.push_back((h[2] - h[0] * x[0]) / h[1]);
        else {
            if (objValue(x[0], (h[2] - h[0] * x[0]) / h[1]) > objValue(x[1], (h[2] - h[0] * x[1]) / h[1])) 
                x[1] = (h[2] - h[0] * x[0]) / h[1];
            else {
                x[0] = x[1];
                x[1] = (h[2] - h[0] * x[0]) / h[1];
            }
        }
    else {
        if (x.size() == 1) {
            x.push_back(h[2] / h[0]);
            swap(x[0], x[1]);
        }
        else {
            if (objValue(h[2] / h[0], x[0]) > objValue(h[2] / h[0], x[1])) {
                x[1] = x[0];
                x[0] = h[2] / h[0];
            } 
            else 
                x[0] = h[2] / h[0];
        }       
    }
    /*if (mes[0] != 'l' && mes[0] != 'r') return x;
    if (mes[0] == 'l' && objective[h[1] != 0 ? 0 : 1] > 0) {
        cout << "Nieograniczony" << endl;
        exit(1);
    }
    else if (mes[0] == 'r' && objective[h[1] != 0 ? 0 : 1] < 0) {
        cout << "Nieograniczony" << endl;
        exit(1);
    }*/
    mes = "";
    return x;
}

double twoDimLP::sol(array<double, 3> e1, array<double, 3> e2){
    array<double, 2> a;
    double det = e1[0] * e2[1] - e1[1] * e2[0];
    double inv[][2] = {{e2[1] / det, -e1[1] / det},
    {-e2[0] / det, e1[0] / det}};
    a[0] = inv[0][0] * e1[2] + inv[0][1] * e2[2];
    a[1] = inv[1][0] * e1[2] + inv[1][1] * e2[2];
    return objValue(a[0], a[1]);
}       

int main(int argc, char **argv) {
    if (argc < 2) return 0;
    twoDimLP testProg = twoDimLP(argv[1]);
    auto res = testProg.solve();
    int i = 1;
    if (!res.second.size()) {
        res.first[0] = round( res.first[0] * 1000.0 ) / 1000.0;
        res.first[1] = round( res.first[1] * 1000.0 ) / 1000.0;
        cout << "x1 = " << res.first[0] << endl;
        cout << "x2 = " << res.first[1] << endl;
        cout << "Wartość: " << testProg.objValue(res.first[0], res.first[1]) << endl;
    }
    else
        cout << res.second << endl;
    return 0;
}
