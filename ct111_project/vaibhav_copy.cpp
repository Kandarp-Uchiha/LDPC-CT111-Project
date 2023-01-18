#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> BSC_channel(vector<int>& v,double p,int n);
void Create_Hmatrix(string filename, vector<vector<int>> &Array);
void printVectorDouble(vector<double> &v);
void printVectorInt(vector<int> &v);

struct CN_node
{
    vector<int> edges;
};// CN node contains a vector of edges (i.e all VN's it is connected to)

struct VN_node
{
    double r_prob;
    vector<int> edges;
};// VN node contains a vector of edges (i.e all CN's it is connected to) and r_prob = 1-p if r=1 where p = p_flip of BSC
                                                                           // r_prob = p   if r=0
int main()
{
    vector<vector<int>> H;
    Create_Hmatrix("Hmatrix3.txt", H);

    int n_CN = H.size();
    int n_VN = H[0].size();

    cout<<n_CN<<" "<<n_VN<<endl;

    vector<int> m(n_VN,0);// All 0 codeword

    vector<CN_node> v_CN; // vector contains pointer to n_CN CN nodes
    vector<VN_node> v_VN; // vector contains pointer to n_VN vN nodes

    v_CN.reserve(n_CN);
    for (int i = 0; i < n_CN; i++) {
        v_CN.push_back(CN_node());
    }
    v_VN.reserve(n_VN);
    for (int i = 0; i < n_VN; i++) {
        v_VN.push_back(VN_node());
    }

    vector<double> edge_CnToVn;
    vector<double> edge_VnToCn;
    int edgeCount = 0;

    for(int i=0;i<n_CN;i++) // Creating edge connection according to the 1's in H matrix
    {
        for(int j=0;j<n_VN;j++)
        {
            if(H[i][j]==1)
            {
                edge_CnToVn.push_back(-1);
                edge_VnToCn.push_back(-1);
                v_CN[i].edges.push_back(edgeCount);
                v_VN[j].edges.push_back(edgeCount);
                edgeCount++;
            }
        }
    }

    double p = 0.3;
    vector<int> r = BSC_channel(m,p,n_VN);

    cout << "r = ";
    printVectorInt(r);

    for (int i = 0; i < n_VN; i++) {
        if (r[i] == 1) {
            v_VN[i].r_prob = 1 - p;
        } else {
            v_VN[i].r_prob = p;
        }
    }

    // CHat
    vector<int> cHat;
    cHat.reserve(n_VN);
    for (int i = 0; i < n_VN; i++) {
        cHat.push_back(-1);
    }

    // First VN to CN
    for (int i = 0; i < n_VN; i++) {
        for (int e : v_VN[i].edges) {
            edge_VnToCn[e] = v_VN[i].r_prob;
        }
    }

    cout << "edge_VnToCn = ";
    printVectorDouble(edge_VnToCn);
    cout << "edge_CnToVn = ";
    printVectorDouble(edge_CnToVn);

    // All Iterations CN -> VN and VN -> CN
    for (int itr = 0; itr < 10; itr++) {
        // CN -> VN
        for (int i = 0; i < n_CN; i++) {
            for (int currEdge : v_CN[i].edges) {
                double prob_0 = 0.5;
                for (int otherEdge : v_CN[i].edges) {
                    if (currEdge != otherEdge) {
                        prob_0 = prob_0*(1 - 2*edge_VnToCn[otherEdge]);
                    }
                }
                prob_0 = 0.5 + prob_0;
                edge_CnToVn[currEdge] = 1 - prob_0;
            }
        }

        // VN -> CN
        for (int i = 0; i < n_VN; i++) {
            for (int currEdge : v_VN[i].edges) {
                double lambda = v_VN[i].r_prob/(1 - v_VN[i].r_prob);
                for (int otherEdge : v_VN[i].edges) {
                    if (currEdge != otherEdge) {
                        lambda = lambda * (edge_CnToVn[otherEdge]/(1 - edge_CnToVn[otherEdge]));
                    }
                }
                edge_VnToCn[currEdge] = lambda/(1+lambda);
            }
        }

        // Check cHat
        for (int i = 0; i < n_VN; i++) {
            double lambda = v_VN[i].r_prob/(1 - v_VN[i].r_prob);
            for (int currEdge : v_VN[i].edges) {
                lambda = lambda * (edge_CnToVn[currEdge]/(1 - edge_CnToVn[currEdge]));
            }
            cHat[i] = lambda > 1 ? 1 : 0;
        }
        cout << "edge_VnToCn = ";
        printVectorDouble(edge_VnToCn);
        cout << "edge_CnToVn = ";
        printVectorDouble(edge_CnToVn);
        cout << "cHat = ";
        printVectorInt(cHat);

    }

    return 0;
};


void Create_Hmatrix(string filename, vector<vector<int>> &Array)
{
    string line;
    ifstream f(filename);
    while (getline (f, line)) {
        string val;
        vector<int> row;
        stringstream s(line);
        while (getline (s, val, ',')) {
            row.push_back (stoi(val));
        }
        Array.push_back (row);
    }
    f.close();
}

vector<int> BSC_channel(vector<int> &v, double p, int n)
{
    vector<int> r;
    srand( (unsigned)time( NULL ) );
    for(int i=0;i<n;i++)
    {
        int x = (float) rand()/RAND_MAX < p;
        r.push_back(x);
    }
    return r;
}

void printVectorDouble(vector<double> &v) {
    for (double val : v) {
        cout << val << " ";
    }
    cout << endl;
}

void printVectorInt(vector<int> &v) {
    for (int val : v) {
        cout << val << " ";
    }
    cout << endl;
}
