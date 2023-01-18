#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> BSC_channel(vector<int>& v,double p,int n);
void Create_Hmatrix(string filename, vector<vector<int>> &Array);
void write_csv(string filename, vector<double> &p, vector<double> &p_success);
void printVectorDouble(vector<double> &v);
void printVectorInt(vector<int> &v);

struct CN_node
{
    vector<int> edges;
};// CN node contains a vector of edges (i.e all VN's it is connected to)

struct VN_node
{
    int r_bit;
    vector<int> edges;
};// VN node contains a vector of edges (i.e all CN's it is connected to) and r_bit = 1 if r=1
                                                                           // r_bit = 0 if r=0
int main()
{
    // For random function
    srand((unsigned)time(NULL));

    vector<string> inFile = {"Hmatrix3.txt", "Hmatrix.csv", "Hmatrix2.csv"}; // For eg if you wish to run only the 9 X 12 H matrix ,
    vector<string> outFile = {"Hmatrix3_out.csv", "Hmatrix_out.csv", "Hmatrix2_out.csv"}; // then remove Hmatrix , Hmatrix2 from inFile and outFile

    for (int f = 0; f < inFile.size(); f++) {
        vector<vector<int>> H;
        Create_Hmatrix(inFile[f], H);

        int n_CN = H.size();
        int n_VN = H[0].size();

        cout<<n_CN<<" "<<n_VN<<endl;

        vector<int> m(n_VN,0);// All 0 codeword

        vector<CN_node> v_CN; // vector contains pointer to n_CN CN nodes
        vector<VN_node> v_VN; // vector contains pointer to n_VN vN nodes

        for (int i = 0; i < n_CN; i++) {
            v_CN.push_back(CN_node());
        }
        for (int i = 0; i < n_VN; i++) {
            v_VN.push_back(VN_node());
        }

        vector<int> edge_CnToVn;
        vector<int> edge_VnToCn;
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

        int Nsim = 1000;
        vector<double> p_val;
        vector<double> p_success;

        // Simulations
        for (double p = 0; p < 1; p += 0.01) {
            int corr = 0;
            int err = 0;
            for (int sim = 0; sim < Nsim; sim++) {
                vector<int> r = BSC_channel(m,p,n_VN);

                for (int i = 0; i < n_VN; i++) {
                    if (r[i] == 1) {
                        v_VN[i].r_bit= 1;
                    } else {
                        v_VN[i].r_bit = 0;
                    }
                }

                // CHat
                vector<int> cHat;
                for (int i = 0; i < n_VN; i++) {
                    cHat.push_back(-1);
                }

                // CHat previous
                vector<int> cHat_prev;
                for (int i = 0; i < n_VN; i++) {
                    cHat_prev.push_back(-1);
                }

                // First VN to CN
                for (int i = 0; i < n_VN; i++) {
                    for (int e : v_VN[i].edges) {
                        edge_VnToCn[e] = v_VN[i].r_bit;
                    }
                }

                // All Iterations CN -> VN and VN -> CN
                for (int itr = 0; itr < 10; itr++) {
                    // CN -> VN
                    for (int i = 0; i < n_CN; i++) {
                        for (int currEdge : v_CN[i].edges) {
                            int bit = 0;
                            for (int otherEdge : v_CN[i].edges) {
                                if (currEdge != otherEdge) {
                                    bit = bit + edge_VnToCn[otherEdge];
                                }
                            }
                            bit = bit%2;
                            edge_CnToVn[currEdge] = bit;
                        }
                    }


                    // VN -> CN
                    for (int i = 0; i < n_VN; i++) {
                        int n = v_VN[i].edges.size();
                        for (int currEdge : v_VN[i].edges) {
                            int bit = v_VN[i].r_bit;
                            for (int otherEdge : v_VN[i].edges) {
                                if (currEdge != otherEdge) {
                                    bit = bit + edge_CnToVn[otherEdge];
                                }
                            }
                            if(bit >= ((double)n)/2){
                                edge_VnToCn[currEdge] = 1;
                            }
                            else{
                                edge_VnToCn[currEdge] = 0;
                            }
                        }
                    }

                    // Check cHat
                    for (int i = 0; i < n_VN; i++) {
                        int n = v_VN[i].edges.size();
                        int bit = v_VN[i].r_bit;
                        for (int currEdge : v_VN[i].edges) {
                            bit = bit + edge_CnToVn[currEdge];
                        }
                        if(bit >= ((double)(n+1))/2){
                            cHat[i] = 1;
                        }
                        else{
                            cHat[i] = 0;
                        }
                    }

                    // Termination condition
                    if (cHat == cHat_prev) {
                        break;
                    }

                    cHat_prev = cHat;
                }

                if (cHat == m) {
                    corr++;
                } else {
                    err++;
                }
            }
            p_val.push_back(p);
            p_success.push_back((double)corr/Nsim);
        }
        printVectorDouble(p_success);
        write_csv(outFile[f], p_val, p_success);
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

void write_csv(string filename, vector<double> &p, vector<double> &p_success) {
    ofstream myFile(filename);
    for (int i = 0; i < p.size(); i++) {
        myFile << p[i] << "," << p_success[i] << endl;
    }
    myFile.close();
};

vector<int> BSC_channel(vector<int> &v, double p, int n)
{
    vector<int> r;
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
