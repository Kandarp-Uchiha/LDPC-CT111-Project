// mathematical analysis and algorithm convergence

#include<iostream>
#include<bits\stdc++.h>
#include<fstream>
using namespace std ;
void Create_Hmatrix(string filename, vector<vector<int>> &Array);


int main()
{
    int number ;
    vector<vector<int>> H;
    vector<string> outFile = {"Hmatrix_out.csv"};
    cout<<"Process starts\n" ;
    cout<<"Select the number of H matrix file you want to execute\n1. Hmatrix.mat\t2. Hmatrix2.mat\t3. 9*12 matrix\n" ;
    cin>>number ;

    switch(number)                // importing H matrix according to requirement by giving user the option
    {
        case 1:
        {
            vector<string> inFile = {"H.csv"};
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

    int no_itr = 50 ;          // no of iterations = 50

    int p_try = 101 ;                      // trials of p
    double xx = 0.0 ;
    double prob_err[p_try] ;               // prob_err is array of values of p
    for (int i=0 ; i<p_try ; i++)
    {
        prob_err[i] = xx ;
        xx = xx + 0.01 ;
    }

    double erasure[p_try][no_itr] ;         

    int dv = 0 ;                   // degree of variable node
    int dc = 0 ;                   // degree of check node
    for (int i=0 ; i<no_row ; i++)
    {
        if (H[i][0] == 1)
            dv++ ;
    }
    for (int i=0 ; i<no_col ; i++)
    {
        if (H[0][i] == 1)
            dc++ ;
    }

    fstream file ;          // file handling to export the erasure probability values calculated in csv format for MATLAB plotting
    file.open("analytic_out.txt") ;

    for (int p_count = 0 ; p_count < p_try ; p_count++)
    {
        double e_not ;
        e_not = prob_err[p_count] ;         // e_not is initially stored value of error probability dependent on bit from channel
        erasure[p_count][0] = e_not ;
        cout<<e_not<<"\t\t\t" ;
        for (int itr = 0 ; itr<no_itr ; itr++)
        {
            if (itr == 0)
            {
                erasure[p_count][itr] = e_not ;    // erasure array will store values of probability of erasure after each iterations
                file<<e_not ;
                file<<"," ;
                continue ;
            }
            erasure[p_count][itr] = e_not * ( pow( (1-(pow(1-erasure[p_count][itr-1],dc-1))),dv-1) ) ; 
            /* 
            The erasure probability can be calculated mathematically to check our simulated results. 
            The formula used above depends on probability of receiving erasures as message from other nodes in iteration process. 
            Reference of formula: Lecture slides of Prof.Vasavada

            take each erasure[p_count][itr] and plot it against itr
            output to CSV from here
            */
            file<<erasure[p_count][itr] ;
            file<<"," ;
        }
        file<<"\n" ;
        cout<<"\n" ;
    }


    file.close() ;

}


/* 
Plotted graphs have been attached in the presentation. 
1. Graphs included erasure probability after each iterations.
2. Graph included erasure correction probability wrt to initial erasure probability of channel
*/

// function to create matrix from the imported csv file
void Create_Hmatrix(string filename, vector<vector<int>> &Array)    
{
    string line;
    ifstream f(filename);
    while (getline (f, line))
    {
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
