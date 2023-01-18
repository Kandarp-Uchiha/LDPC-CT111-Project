#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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

int main()
{
    srand((unsigned)time(NULL));
    int n_VN = 10;
    double p = 0.3;
    vector<int> m(n_VN,0);// All 0 codeword
    vector<int> r = BSC_channel(m,p,n_VN);

    cout<<"The received bits after passing the all zero codeword through the BSC channel: "<<endl;
    for(int i=0;i<r.size();i++)
    {
        cout<<r[i]<<" ";
    }
    return 0;
}
