#include<bits/stdc++.h>
#include<iostream>
using namespace std;
void BEC(float p,int *a,int size)//Creating a function taking inputs of probability,message to be passed ,the message length.
{
    float p_erasure[size];
     for(int i=0;i<size;i++)
    {
        p_erasure[i] = (float) rand()/RAND_MAX;//To Generate random probabilities of erasure
    }
    for(int i=0;i<size;i++)
    {
        if(p_erasure[i]<p)//The erased bit is represented by -1 value if p_erasure is less than probability entered by the user
        {
            a[i]  = -1;
        }
    }
    cout<<"Code after passing over BEC channel: "<<endl;
    for(int i=0;i<size;i++)
    {
        cout<<a[i]<<" ";
    }
}
int main()
{
    int size;
    cout<<"Enter the length of the code: ";
    cin>>size;
    int code[size];
    cout<<"Enter Code: ";
    for(int i=0;i<size;i++)
    {
        cin>>code[i];
    }
    cout<<"Enter the probability: ";
    float p;
    cin>>p;
    BEC(p,code,size);

    return 0;
}
