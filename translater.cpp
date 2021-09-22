#include<iostream>
#include<fstream>
#include <string>
#include<vector>
using namespace std;

//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5
char check_table[100][5]=//语法规则表
{
"0##1",//开始到#的间断
"#1i1",//#到include的间断
"i1i0",//include内的字母延续
"i<<1",//include到<的间断
"<1h1",//<到头文件的间断 
"h1h0",//头文件的字母延续
"h.h0",//头文件的.号延续
"h>>1",//头文件到>的间断

"0111",//空格到字母的间断
"0_11",//开始到_的间断,首字母不为数字
"1110",//变量名的延续,可以承接_,数字
"1_10",//变量名到_的延续
"1310",//变量名到数字延续
"1001",//变量名到空格的间断
"0;01",//空到;的间隔
"1;01",//变量名到;的间断
"3;01",//数字到;的间隔
"0331",//空格到数字的间隔

"1((1",//函数名到(的间断
//"(001",//括号到空格的间隔
"(111",//括号到字母的间隔
"(_11",//括号到_的姐间隔

"0**1",//空到*的间隔
"1**1",//变量名到*的间隔
"3**1",//数字到*的间隔
"*111",//*到字母的间隔
"*_11",//*到_的间隔

"0++1",//空到+的间隔
"1++1",//变量名到+的间隔
"3++1",//数字到+的间隔
"+111",//+到字母的间隔
"+_11",//+到_的间隔

//"0001",//空格到空格的间隔
//"*001",//*到空格的间隔

"0,01",//空格到,的间隔
"1,01",//变量名到,的间隔
//",111",//,到字母的间隔
//",_11",//,到_的间隔

"0==1",//空到=的间隔
"1==1",//变量名到=的间隔
"=111",//=到变量名的间隔
"=331",//=到数字的间隔

"0{01",//空到{的间隔
"){01",//)到{的间隔
"0}01",//空到}的间隔

"0>>1",//空到>的间隔
"1>>1",//变量到>的间隔
">>20",//>>的延续

"0<<1",//空到<的间隔
"1<<1",//变量到<的间隔
"<<20",//<<的延续
"2111",//<<到变量的间隔
"2_11",//<<到_的间隔

"1+21",//
};
int len_check_table=sizeof(check_table)/sizeof(check_table[0]);
string keyword[100] ={"include","iostream.h","int","main","return","cout","endl"}; 
string translation_keyword[100]={"import","iostream.h","int8",""};
string type_of_str[5]={"变量名","特殊符号","数字","字符串","关键字"};
vector<pair<string,char>> vstrs;//装载所有的类型
int pointin(string str,char now_char)
{
    if(now_char>=48&&now_char<=57)//如果是数字
        {
            if(now_char==48)return 2;
            else{
                if(now_char-48==1)
                {
                    for(int i=0;i<100;i++)
                    {
                        if(keyword[i]=="")break;
                        if(str==keyword[i])return 5;
                    }
                    return 1;
                }
                return (int)(now_char-48);
            }
        }
        else if((now_char>=65&&now_char<=90)||(now_char>=97&&now_char<=122))//如果是字母
        {
            return 5;
        }
        else//如果是符号
        {
            return 2;
        }
}
void scanner(string str)//扫描每一行
{
    char now_char=str[0];//当前指向的字符
    int lenofeachline=str.size();//代码行的长度
    int index=0;//表示当前指向的字符下标
    string n="";//所得到的token
    char now_type[5]="0000";//初始化当前字符状态
    while(index<=lenofeachline)
    {
        now_char=str[index];
        now_type[0]=now_type[2];
        if(now_char>=48&&now_char<=57)//如果是数字
        {
            now_type[1]='3';
        }
        else if((now_char>=65&&now_char<=90)||(now_char>=97&&now_char<=122))//如果是字母
        {
            now_type[1]='1';
        }
        else//如果是符号
        {
            if(now_char==' '){
                if(now_type[0]!='s')//如果不在字符串中
                {
                    if(n!=""&&now_type[0]!='0')//如果此时的n储存到内容,并且上一次内容不为0
                    {
                        pair<string,char> *last_str_type=new pair<string,char>(n,now_type[0]);
                        vstrs.push_back(*last_str_type);
                        n="";
                    }
                    now_type[2]='0';
                    index++;
                    continue;
                }
                else//如果在字符串中,空格认为是字母
                {
                    now_type[1]='1';
                }
            }
            else if(now_char=='#')now_type[1]='#';
            else if(now_char=='<')now_type[1]='<';
            else if(now_char=='>')now_type[1]='>';
            else if(now_char=='.')now_type[1]='.';
            else if(now_char==';')now_type[1]=';';
            else if(now_char=='{')now_type[1]='{';
            else if(now_char=='}')now_type[1]='}';
            else if(now_char=='(')now_type[1]='(';
            else if(now_char==')')now_type[1]=')';
            else if(now_char=='=')now_type[1]='=';
            else if(now_char=='+')now_type[1]='+';
            else now_type[1]='2';
        }
        for(int i=0;i<len_check_table;i++)//匹配规则表
        {
            if(check_table[i][0]==now_type[0]&&check_table[i][1]==now_type[1])
            {
                now_type[2]=check_table[i][2];
                now_type[3]=check_table[i][3];
                break;
            }
        }
        if(now_type[3]!='0')
        {
            if(n!="")
            {
                pair<string,char> *last_str_type=new pair<string,char>(n,now_type[0]);
                //cout<<"values:"<<last_str_type->first<<"  type:"<<last_str_type->second<<endl;
                vstrs.push_back(*last_str_type);
                n="";
            }
        }
        n=n+now_char;
        index++;
    }
}

int main()
{
    ifstream in("./compilers/main.cpp");
    string str;
    if(in){
        while(getline(in,str)){
            scanner(str);
        }
    }
    else{
        cout<<"file is not exist"<<endl;
    }
    for(int i=0;i<vstrs.size();i++){
        cout<<i<<"\t"<<"values:"<<vstrs[i].first<<"\ttype:"<<type_of_str[pointin(vstrs[i].first,vstrs[i].second)-1]<<endl;
    }
    system("pause");
}