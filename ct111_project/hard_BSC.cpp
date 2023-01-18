#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> BSC_channel(vector<int>& v,double p,int n)
{
    vector<int> r;
    for(int i=0;i<n;i++)
    {
        int x = (float) rand()/RAND_MAX < p;
        r.push_back(x);
    }
    return r;
}

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


struct edge
{
    int VNtoCN;
    int CNtoVN;
};

struct CN_node
{
    vector<edge*> edges;
};

struct VN_node
{
    int r_bit;
    vector<edge*> edges;
};


int main()
{
    srand((unsigned)time(NULL));
    vector<vector<int>> H;
    Create_Hmatrix("Hmatrix3.txt", H);

    int n_CN = H.size();
    int n_VN = H[0].size();

    vector<int> m(n_VN,0);// All 0 codeword

    vector<CN_node*> v_CN; // vector contains pointer to n_CN CN nodes
    vector<VN_node*> v_VN; // vector contains pointer to n_VN vN nodes


    for(int i=0;i<n_CN;i++)// Creating CN_nodes and assigning pointers to them and adding in v_CN
    {
        struct CN_node *ptr = new CN_node;
        v_CN.push_back(ptr);
    }

    for(int i=0;i<n_VN;i++)// Creating VN_nodes and assigning pointers to them and adding in v_VN
    {
        struct VN_node *ptr = new VN_node;
        v_VN.push_back(ptr);
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


    int Nsim = 50;

    vector<double> p_success;



    for(float p = 0; p <= 1 ; p = p+0.1)
    {
        vector<int> r = BSC_channel(m,p,n_VN);// Received bits for n VN's
        for(int j=0; j<n_VN; j++)
        {
            if(r[j]==1)
            {
                v_VN[j]->r_bit = 1;
            }
            else
            {
                v_VN[j]->r_bit = 0;
            }
        }

        int Ncorr = 0;
        int Nerr = 0;

        for(int n_sim = 0; n_sim < Nsim; n_sim++)
        {
            int itr = 0; // Iteration initialized to 0
            vector<int> c_hat_prev(n_VN,-1); // vectors to compare bit decisions; if after one iteration they are same
            vector<int> c_hat_next; // then stop iterating(break)

            while(itr<10) // iteration_max = 10 ; after this stop iterating ; but it could also
            {
                // stop in the middle if bit decisions c_hat become equal in 2 consecutive iterations

                if(itr == 0) // In this 0 iteration just pass 1-p , p from VN to CN
                {
                    for(int i=0; i<n_VN; i++) // This for loop for VN to CN
                    {
                        for (edge* e : v_VN[i]->edges)
                        {
                            e->VNtoCN = v_VN[i]->r_bit;
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
                            int to_send = 0;
                            for(int k=0; k<n_edge; k++)
                            {
                                if(k!=j)
                                {
                                    to_send = to_send + v_CN[i]->edges[k]->VNtoCN;
                                }
                            }
                            v_CN[i]->edges[j]->CNtoVN = to_send%2;
                        }
                    }
                }
                else
                {
                    for(int i=0; i<n_VN; i++) // for VN to CN ,when iteration > 0 , again lambda = p_to_send,
                        // then as lambda = p_hat/(1-p_hat) -> p_hat = lambda/(1+lambda), I send this
                    {
                        int c_hat_bit = 0;

                        int n_edge = v_VN[i]->edges.size();
                        for (int j=0; j<n_edge; j++)
                        {
                            c_hat_bit = c_hat_bit + v_VN[i]->edges[j]->CNtoVN;
                            int to_send = 0;
                            for (int k=0; k<n_edge; k++)
                            {
                                if(k!=j)
                                {
                                    to_send = to_send + v_VN[i]->edges[k]->CNtoVN;
                                }
                            }
                            if(to_send > (n_edge-1)/2.0)
                            {
                                v_VN[i]->edges[j]->VNtoCN = 1;
                            }
                            else
                            {
                                v_VN[i]->edges[j]->VNtoCN = 0;
                            }
                        }

                        if(c_hat_bit > n_edge/2.0)
                        {
                            c_hat_next.push_back(1);
                        }
                        else
                        {
                            c_hat_next.push_back(0);
                        }
                    }
                    if(c_hat_next == c_hat_prev) // if c_hat_next and c_hat_prev vectors are same then end iterating
                    {
                        break;
                    }
                    else
                    {
                        c_hat_prev = c_hat_next; // else ,new prev = next
                        c_hat_next.clear();
                    }


                    for(int i=0; i<n_CN; i++) // This for loop for CN to VN.
                    {
                        int n_edge = v_CN[i]->edges.size();
                        for (int j=0; j<n_edge; j++)
                        {
                            int to_send = 0;
                            for(int k=0; k<n_edge; k++)
                            {
                                if(k!=j)
                                {
                                    to_send = to_send + v_CN[i]->edges[k]->VNtoCN;
                                }
                            }
                            v_CN[i]->edges[j]->CNtoVN = to_send%2;
                        }
                    }
                }
                itr++;
            }


            if(c_hat_next == m)// After all iterations if c_hat_decoded = r then correctly decoded
            {
                Ncorr++;
            }
            else
            {
                Nerr++;
            }
        }
        p_success.push_back((Ncorr*1.0)/Nsim);
        cout<<Ncorr<<endl;
    }


    for(int k=0;k<p_success.size();k++)
    {
        cout<<p_success[k]<<" ";
    }


    return 0;
};







