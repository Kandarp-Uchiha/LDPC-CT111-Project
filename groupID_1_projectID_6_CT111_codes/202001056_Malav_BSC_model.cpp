#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <vector>
using namespace std;

vector<float> BSC(float m[], float p, int size)
{
    vector <float> r;

    for(int i=0; i<size; i++)
    {
        float p_test = rand()%100;
        if(( p_test > p))
        {
            r.push_back(0);
        }
        else{
            r.push_back(1);
        }
    }
    return r;
}

int main()
{
    float prob = 0.3;
    float m[9]={0};
    vector <float> r;

    r = BSC(m, prob,9);
    cout <<"Input MSG: ";
    for (int i=0; i<9; i++)
    {
        cout<<m[i]<<" ";
    }
    cout <<"\n Final MSG: ";
    for(int i=0; i<9; i++)
    {
        cout << r[i]<<" ";
    }

}
