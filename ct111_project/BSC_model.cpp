#include <iostream>
#include <bits/stdc++.h>
using namespace std;

vector<int> BSC_channel(vector<int>& v,double p,int n);

int main()
{
    vector<int> m = {0,0,0,0,0,0,0,0,0,0};
    double p = 1;
    vector<int> r = BSC_channel(m,p,10);
    for(int i=0;i<r.size();i++)
    {
        cout<<r[i]<<" ";
    }
    return 0;
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
