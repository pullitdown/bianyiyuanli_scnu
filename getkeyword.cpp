#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    ifstream in_("C:/Users/stepf/Desktop/¹Ø¼ü×Ö.txt");
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