#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    ifstream in_("C:/Users/stepf/Desktop/�ؼ���.txt");
    ofstream out("./compilers/keyword.txt");
    string str_;
    if (in_)
    {
        while (getline(in_, str_))
        {
            out<<"\""<<str_<<"\",";
        }
    }
}