#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> BSC_channel(vector<int>& v,double p,int n);
void Create_Hmatrix(string filename, vector<vector<int>> &Array);
double prob_even(int i, vector<double> &v);
double prob_odd(int i, vector<double> &v);




// 3 below structs help in tanner graph implementation of H matrix
struct edge
{
    double p_VNtoCN;
    double p_CNtoVN;
};// edge between VNi and CNj , with 2 prob.'s p(VNi to CNj) and p(CNj to VNi)

struct CN_node
{
    vector<edge*> edges;
};// CN node contains a vector of edges (i.e all VN's it is connected to)

struct VN_node
{
    double r_prob;
    vector<edge*> edges;
};// VN node contains a vector of edges (i.e all CN's it is connected to) and r_prob = 1-p if r=1 where p = p_flip of BSC
                                                                           // r_prob = p   if r=0




int main()
{
    vector<vector<int>> H;
    Create_Hmatrix("Hmatrix3.txt", H);

    int n_CN = H.size();
    int n_VN = H[0].size();

    vector<int> m(n_VN,0);// All 0 codeword

    double p = 0.3; // for making error graph I will vary p also from 0 to 1 in steps
    //vector<int> r = BSC_channel(m,p,n_VN);// Received bits for n VN's
    vector<int> r = {1,0,1,0,1,0,0,0,0,0,0,0};

    vector<CN_node*> v_CN; // vector contains pointer to n_CN CN nodes
    vector<VN_node*> v_VN; // vector contains pointer to n_VN vN nodes

    vector<int> c_hat_prev(n_VN,-1); // vectors to compare bit decisions; if after one iteration they are same
    vector<int> c_hat_next; // then stop iterating(break)


    for(int i=0;i<n_CN;i++)// Creating CN_nodes and assigning pointers to them and adding in v_CN
    {
        struct CN_node *ptr = new CN_node;
        v_CN.push_back(ptr);
    }

    for(int i=0;i<n_VN;i++)// Creating VN_nodes and assigning pointers to them and adding in v_VN
    {
        struct VN_node *ptr = new VN_node;
        v_VN.push_back(ptr);
        if(r[i]==1)
        {
            ptr->r_prob = 1-p;
        }
        else{
            ptr->r_prob = p;
        }
    }


    for(int i=0;i<n_CN;i++) // Creating edge connection according to the 1's in H matrix
    {
        for(int j=0;j<n_VN;j++)
        {
            if(H[i][j]==1)
            {
                edge *tmp = new edge;
                v_CN[i]->edges.push_back(tmp);
                v_VN[j]->edges.push_back(tmp);
            }
        }
    }

    int itr = 0; // Iteration initialized to 0

    int Ncorr = 0;
    int Nerr = 0;

    while(itr<20) // iteration_max = 10 ; after this stop iterating ; but it could also
    {            // stop in the middle if bit decisions c_hat become equal in 2 consecutive iterations

        if(itr == 0) // In this 0 iteration just pass 1-p , p from VN to CN
        {
            for(int i=0; i<n_VN; i++) // This for loop for VN to CN
            {
                for (edge* e : v_VN[i]->edges)
                {
                    e->p_VNtoCN = v_VN[i]->r_prob;
                }
            }

            for(int i=0; i<n_CN; i++) // This for loop for CN to VN.
                                      // Here what prob to send from the CN to the VN while taking prob's from
                                      // other VN's connected to that CN was difficult to find first , but then I
                                      // used 2 recursive functions prob_odd, prob_even(given after int main()) to find it.
                                      // I am just sending prob's as messages from VN to CN or CN to VN instead of sending
                                      // likelihoods as this was easier for me . So i first calculated lambda(to send)
                                      // then as lambda = p_hat/(1-p_hat) -> p_hat = lambda/(1+lambda), I send this, also
                                      // here p_to_send is therefore acting as lambda.

            {
                int n_edge = v_CN[i]->edges.size();
                for (int j=0; j<n_edge; j++)
                {
                    vector<double> tmp;
                    double p_to_send = 0.5;
                    for(int k=0; k<n_edge; k++)
                    {
                        if(k!=j)
                        {
                            p_to_send = p_to_send*(1-2*v_CN[i]->edges[k]->p_VNtoCN);
                            //tmp.push_back(v_CN[i]->edges[k]->p_VNtoCN);
                        }

                    }
                    v_CN[i]->edges[j]->p_CNtoVN = 0.5 - p_to_send;
                    //p_to_send = prob_odd(0,tmp)/prob_even(0,tmp);
                    //v_CN[i]->edges[j]->p_CNtoVN = p_to_send/(1+p_to_send);
                }
            }
        }
        else
        {
            c_hat_next.clear();
            for(int i=0; i<n_VN; i++) // for VN to CN ,when iteration > 0 , again lambda = p_to_send,
                                      // then as lambda = p_hat/(1-p_hat) -> p_hat = lambda/(1+lambda), I send this
            {
                double p_c_hat = 1;
                int n_edge = v_VN[i]->edges.size();
                for (int j=0; j<n_edge; j++)
                {
                    double a = v_VN[i]->edges[j]->p_CNtoVN;
                    p_c_hat = p_c_hat * (a/(1-a));
                    double p_to_send = 1;
                    for (int k=0; k<n_edge; k++)
                    {
                        if(k!=j)
                        {
                            double x = (v_VN[i]->edges[k]->p_CNtoVN);
                            p_to_send = p_to_send * (x/(1-x));
                        }
                    }
                    double y = v_VN[i]->r_prob;
                    p_to_send = p_to_send * (y/(1-y));
                    v_VN[i]->edges[j]->p_VNtoCN = p_to_send/(1+p_to_send);
                }
                double b = v_VN[i]->r_prob;
                p_c_hat = p_c_hat * (b/(1-b));
                if(p_c_hat>=1)
                 {
                     c_hat_next.push_back(1);
                 }
                 else{
                    c_hat_next.push_back(0);
                 }
            }
            //if(c_hat_next == c_hat_prev) // if c_hat_next and c_hat_prev vectors are same then end iterating
            //{
            //    break;
            //}
            //else{
            //    c_hat_prev = c_hat_next; // else ,new prev = next
            //    c_hat_next.clear();
            //}


            for(int i=0; i<n_CN; i++) // This is same CN to VN code I used even in iteration = 0
            {
                int n_edge = v_CN[i]->edges.size();
                for (int j=0; j<n_edge; j++)
                {
                    vector<double> tmp;
                    double p_to_send = 0.5;
                    for(int k=0; k<n_edge; k++)
                    {
                        if(k!=j)
                        {
                            p_to_send = p_to_send*(1-2*v_CN[i]->edges[k]->p_VNtoCN);
                            //tmp.push_back(v_CN[i]->edges[k]->p_VNtoCN);
                        }

                    }
                    v_CN[i]->edges[j]->p_CNtoVN = 0.5 - p_to_send;
                    //p_to_send = prob_odd(0,tmp)/prob_even(0,tmp);
                    //v_CN[i]->edges[j]->p_CNtoVN = p_to_send/(1+p_to_send);
                }
            }
        }
        itr++;

        cout << "p = " << p << endl;

        for (int i = 0; i < v_CN.size(); i++) {
            cout << "cn" << i << " ";
            for (int j = 0; j < v_CN[i]->edges.size(); j++) {
                cout << v_CN[i]->edges[j]->p_VNtoCN << ", ";
            }
            cout << endl;

        }
        cout << endl << endl;

        for (int i = 0; i < v_VN.size(); i++) {
            cout << "vn" << i << " ";
            for (int j = 0; j < v_VN[i]->edges.size(); j++) {
                cout << v_VN[i]->edges[j]->p_CNtoVN << ", ";
            }
            cout << endl;

        }
        cout << endl << endl;

        for (int val : c_hat_next) {
            cout << val << ",";
        }
        cout << endl << endl;

        for (int val : r) {
            cout << val << ",";
        }
        cout << endl << endl;
    }


    if(c_hat_next == m)// After all iterations if c_hat_decoded = r then correctly decoded
    {
        Ncorr++;
    }
    else{
        Nerr++;
    }

    cout<<Ncorr<<endl;

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

vector<int> BSC_channel(vector<int>& v,double p,int n)
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

double prob_even(int i, vector<double> &v) {
    if (i == v.size() - 1) {
        return 1-v[i];
    }
    return v[i]*prob_odd(i+1, v) + (1-v[i])*prob_even(i+1,v);
}

double prob_odd(int i, vector<double> &v) {
    if (i == v.size() - 1) {
        return v[i];
    }
    return v[i]*prob_even(i+1, v) + (1-v[i])*prob_odd(i+1,v);
}





