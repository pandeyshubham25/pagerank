#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
struct Node {
    int id;
    double val;
};
int main(int argc, char** argv) {
    char * base_ranks = argv[1];
    char * ranks = argv[2];

    ifstream cin(base_ranks);

    unordered_map<int,int> comp;
    vector<Node> vbase;
    vector<Node> v;

    auto compare = [](Node &a, Node &b) {
        return a.val>b.val;
    };

    //read base ranks
    int id;
    double val;
    while(cin >> id >> val) {
        vbase.push_back({id,val});
    }

    //sort base ranks
    sort(vbase.begin(), vbase.end(), compare);
    //add vals to map
    for(int i=0; i<vbase.size(); i++) {
        comp[vbase[i].id]=i;
    }

    //read target ranks
    ifstream cinr(ranks);

    while(cinr >> id >> val) {
        v.push_back({id,val});
    }

    //sort target ranks
    sort(v.begin(), v.end(), compare);

    double sqdev=0.0;
    int true_id;
    for(int i=0; i<v.size(); i++) {
        true_id = comp[v[i].id];
        sqdev = (true_id-i)*(true_id-i);
    }
    sqdev/=v.size();
    cout << sqrt(sqdev);
    return 0;
}