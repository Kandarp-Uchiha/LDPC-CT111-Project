#include <iostream>
#include <bits/stdc++.h>
using namespace std ;
string binaryConversion(int,int) ;
void Create_Hmatrix(string filename, vector<vector<int>> &Array);

// I am using the conditional probability as passing means which can be derived from lamda as lamda = p / 1-p

struct edge
{
    int r;
    double prev ;
    double p_VNtoCN;
    double p_CNtoVN;
    double p_next ;
};

struct CN_node
{
    vector<edge> edges;
};

struct VN_node
{
    vector<edge> edges;
};

int no_one = 2 ;       // no of ones in each column. 2 for this example. 4 in real Hmatrix.mat    // degree of V
int no_one_row = 4 ;   // no of ones in each row. 4 for this example. 5 in real Hmatrix.mat       // degree of C


int main()
{
    srand((unsigned)time(NULL)) ;
    int number ;
    int xy = 0 ;
    vector<vector<int>> H;
    vector<string>inFile ;
    fstream file ;
    file.open("Hmatrix_out.txt") ;
    cout<<"Process starts\n" ;
    cout<<"Select the number of H matrix file you want to execute\n1. Hmatrix.mat\t2. Hmatrix2.mat\t3. 9*12 matrix\n" ;
    cin>>number ;

    switch(number)
    {
        case 1:
        {
            inFile = {"H.csv"};
            for (int f = 0; f < inFile.size(); f++)
            {
                Create_Hmatrix(inFile[f], H);
            }
            break ;
        }
        case 2:
        {
            vector<string> inFile = {"H2.csv"} ;
            for (int f = 0; f < inFile.size(); f++)
            {
                Create_Hmatrix(inFile[f], H);
            }
            break ;
        }
        case 3:
        {
            vector<string> inFile = {"H3.csv"} ;
            for (int f = 0; f < inFile.size(); f++)
            {
                Create_Hmatrix(inFile[f], H);
            }
            break ;
        }
    }
    int no_row = H.size();
    int no_col = H[0].size();

    float v[no_col] = {0};           // the initially loaded value depending on received bit from channel
    float prev[no_col] ;        // value of same edge in previous iteration
    int c[no_row] ;
    //int H[no_row][no_col] = {{0,1,0,1,1,0,0,1},{1,1,1,0,0,1,0,0},{0,0,1,0,0,1,1,1},{1,0,0,1,1,0,1,0}} ;
    int input[no_col] = {0} ;  // input is all zero as per sir
    int no_prob = 101 ;
    float p[no_prob] ;
    int xx = 0 ;
    for (double num = 0.0 ; num <= 1 ;)
    {
        p[xx] = num ;
        num = num + 0.01 ;
        xx++ ;
    }
    int Nsim = 1000 ;               // no. of simulations

    int p_value ;

    double success_each_p[no_prob] ;
    double failure_each_p[no_prob] ;

    vector <VN_node> V_CN ;         // objects of struct as vector
    vector <CN_node> C_VN ;


 //////////////////////////////////////////////////////////////////////////////////

    for (int i=0 ; i<no_col ; i++)
    {
        struct VN_node tmp ;
        for (int j=0 ; j<no_row ; j++)
        {
            if (H[j][i] == 1)    // loading connections and edges Vn to Cn
            {
                struct edge temp ;
                temp.p_VNtoCN = v[i] ;      // probabilities are stored in edges
                temp.prev = v[i] ;
                temp.r = i ;
                tmp.edges.push_back(temp) ;
            }

        }
        V_CN.push_back(tmp) ;
    }

    for (int j=0 ; j<no_row ; j++)
    {
        struct CN_node tmp ;
        for (int i=0 ; i<no_col ; i++)
        {
            if (H[j][i] == 1)          // loading connections and edges Cn to Vn
            {
                struct edge temp ;
                temp.p_VNtoCN = v[i] ;
                temp.r = i ;
                temp.prev = v[i] ;
                tmp.edges.push_back(temp) ;

            }
        }
        C_VN.push_back(tmp) ;
    }

    cout<<"Loaded" ;

 ///////////////////////////////////////////////////////////////////////////////////


   /* for (int i=0 ; i<C_VN.size() ; i++)     // traversing through C nodes
    {
        for (int j=0 ; j<C_VN[i].edges.size() ; j++)    // traversing each edge of c node
        {
            C_VN[i].edges[j].p_VNtoCN = v[C_VN[i].edges[j].r] ;
        }
    }*/



/////////////////////////////////////////////////////////////////////////////////////////



    for (int p_value = 0 ; p_value < no_prob ; p_value++) // for different values of error probability
    {
        int success = 0 ;
        int error = 0 ;

        for (int n_sim = 0 ; n_sim<Nsim ; n_sim++)   // Monte carlo
        {
            vector<int>output ;              // output vector
            vector<int>noise ;

            for (int i=0 ; i<no_col ; i++)         // noise and loading the probabilities in v array
            {
                int x_noise = (float) rand()/RAND_MAX < p[p_value] ;    // random noise
                if (x_noise == 1)
                {
                    v[i] = 0.5 ;
                    input[i] = -1 ;      // -1 for erasure
                }
                else
                {
                    v[i] = input[i] ;       // conditional probability is 1 for r = 1 and 0  for r = 0 ;
                }
                noise.push_back(x_noise) ;
            }

            ////////////////////////////////////////////////////////////////////////

            for (int i=0 ; i<C_VN.size() ; i++)     // traversing through C nodes
            {
                for (int j=0 ; j<C_VN[i].edges.size() ; j++)    // traversing each edge of c node
                {
                    C_VN[i].edges[j].p_VNtoCN = v[C_VN[i].edges[j].r] ;
                    C_VN[i].edges[j].prev = v[C_VN[i].edges[j].r] ;
                }
            }

            for (int i=0 ; i<V_CN.size() ; i++)
            {
                for (int j=0 ; j<V_CN[i].edges.size() ; j++)
                {
                    V_CN[i].edges[j].p_VNtoCN = v[V_CN[i].edges[j].r] ;
                    V_CN[i].edges[j].prev = v[V_CN[i].edges[j].r] ;
                }
            }

              /////////////////////////////////////////

            float new_value[no_col] ;
            for (int x=0 ; x<no_col ; x++)
            {
                new_value[x] = 1 ;
            }


            for (int iteration = 1; iteration <= 10 ; iteration++)   // max 10 iterations
            {
                for (int i=0 ; i<C_VN.size() ; i++)     // traversing through C nodes
                {
                    for (int j=0 ; j<C_VN[i].edges.size() ; j++)    // traversing each edge of c node
                    {
                        int max = pow(2,C_VN[i].edges.size()-1);
                        int x=0;
                        int xx=0 ;
                        double prob = 0 ;
                        double prob_r = 0 ;
                        while (x<max)   // for making all possible combinations of odd number of 1
                        {
                            double tmp_p = 1.0 ;
                            double tmp_p_r = 1.0 ;
                            int flag = 0;
                            if (__builtin_popcount(x)%2)
                            {
                                flag = 1 ;
                                string number = binaryConversion(x, C_VN[i].edges.size()-1) ;
                                int extra = 0 ;
                                for (int y=0 ; y<number.length() ; y++)
                                {
                                    if (extra == j)
                                        {extra++ ;}
                                    if (number[y] == '1')
                                        {
                                            tmp_p = tmp_p * C_VN[i].edges[extra].p_VNtoCN ;
                                        }
                                    else if (number[y] == '0')
                                        {
                                            tmp_p = tmp_p * (1 - C_VN[i].edges[extra].p_VNtoCN) ;
                                        }
                                    extra++ ;
                                }


                            }
                            if (flag == 1)
                                    {prob = prob + tmp_p ;}

                            flag = 0 ;

                            if (__builtin_popcount(x)%2 == 0)      // making all possible combinations for even number of 1
                            {
                                flag = 1 ;
                                string number = binaryConversion(xx, C_VN[i].edges.size()-1) ;
                                int extra = 0 ;
                                for (int y=0 ; y<number.length() ; y++)
                                {
                                    if (extra == j)
                                        {
                                            extra++ ;
                                        }
                                    if (number[y] == '1')
                                        {
                                            tmp_p_r = tmp_p_r * C_VN[i].edges[extra].p_VNtoCN ;
                                        }
                                    else if (number[y] == '0')
                                        {
                                            tmp_p_r = tmp_p_r * (1 - C_VN[i].edges[extra].p_VNtoCN) ;
                                        }
                                        extra++ ;
                                }
                            }
                            x++;
                            if (flag == 1)
                            {
                                prob_r = prob_r + tmp_p_r ;
                            }

                        }

                        C_VN[i].edges[j].p_CNtoVN = (prob/prob_r)/(1.0+(prob/prob_r)) ;   //sending message to kth node in the list of that C node
                    }
                }

                for (int i=0 ; i<C_VN.size() ; i++)     // traversing through C nodes
                {
                    for (int j=0 ; j<C_VN[i].edges.size() ; j++)    // traversing each edge of c node
                    {
                        for (int xy=0 ; xy<no_col ; xy++)
                        {
                            new_value[xy] = 1 ;
                        }
                        for (int ii=0 ; ii<C_VN.size() ; ii++)
                        {
                            for (int jj=0 ; jj<C_VN[ii].edges.size() ; jj++)
                            {
                                if (ii != i && jj != j)
                                {
                                    new_value[C_VN[ii].edges[jj].r] *= (C_VN[ii].edges[jj].p_CNtoVN/(1-C_VN[ii].edges[jj].p_CNtoVN) ) ;
                                    // all connections except itself
                                }
                                if (ii == i  && jj == j)
                                {
                                    new_value[C_VN[i].edges[j].r] *= (C_VN[i].edges[j].prev / (1 - C_VN[i].edges[j].prev)) ;
                                }
                            }

                        }
                        C_VN[i].edges[j].p_VNtoCN =  new_value[C_VN[i].edges[j].r] / (1 + new_value[C_VN[i].edges[j].r]);
                        // getting values of p_hat from value of lamda which is stored in new_value vector


                        C_VN[i].edges[j].prev = C_VN[i].edges[j].p_VNtoCN ;      // *****UNDER DOUBT************

                    }
                }


            }

            vector<float>prob_mult[no_col] ;
            float hat[no_col] ;

            for (int i=0 ; i < C_VN.size() ; i++)
            {
                for (int j=0 ; j<C_VN[i].edges.size() ; j++)
                {
                    prob_mult[C_VN[i].edges[j].r].push_back(C_VN[i].edges[j].p_VNtoCN) ;
                    // finding all possible inputs sent to a particular Vnode
                }
            }

            for (int i=0 ; i<no_col ; i++)
            {
                float prod = 1 ;
                for (int j=0 ; j<prob_mult[i].size() ; j++)
                {
                    prod = prod * prob_mult[i][j] / (1 - prob_mult[i][j]) ;   // calculating lamda for decision
                }
                prod = prod * v[i]/(1 - v[i]) ;            // multiplying self value
                hat[i] = prod ;

                if (hat[i] > 1)                 // hat is lamda for decision
                {
                    output.push_back(1) ;
                }
                else if (hat[i] < 1)
                {
                    output.push_back(0) ;
                }
                else
                {
                    output.push_back(-1) ;
                }

            }


            int flag = 1 ;
            for(int i=0 ; i<no_col ; i++)
            {
                if (output[i] != input[i])
                {
                    error += 1 ;
                    flag = 0 ;
                    break ;
                }
            }
            if (flag == 1)
            {
                success += 1 ;
            }

            //V_CN.clear() ;
            //C_VN.clear() ;
            output.clear() ;

        }
        success_each_p[p_value] = (double)success / Nsim ;
        failure_each_p[p_value] = (double)error/Nsim ;
        //cout<<success_each_p[p_value] <<"\t" ;
        file<<success_each_p[p_value]<<"," ;

    }

    cout<<"\nprobability of successful output" ;


}

string binaryConversion(int n, int length)
{
    string ans = "" ;
    while (n>0){
        if (n%2) ans = '1' + ans;
        else ans = '0' + ans;
        n /= 2;
    }
    int size = ans.size();
    while (size < length){
        ans = '0' + ans;
        size++;
    }
    return ans;
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
