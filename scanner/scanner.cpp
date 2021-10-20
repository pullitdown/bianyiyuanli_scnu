#include<iostream>
#include<fstream>
#include <string>
#include<vector>
using namespace std;


//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5
char translator_table[100][5] = //预编译语法规则表
{
        "0@@1",
        "@1h1",
        "h1h0",
        "h.h0", //头文件的引入 eg:@iostream

        "0111", //空到字母的间断
        "0_11", //开始到_的间断,首字母不为数字
        "1110", //变量名到字母的延续,可以承接_,数字
        "1_10", //变量名到_的延续
        "1310", //变量名到数字延续

        "1((1", //函数名到(的间断
        //"(001",//括号到空格的间隔
        "(111", //括号到字母的间隔
        "(_11", //括号到_的姐间隔

        "0**1", //空到*的间隔
        "1**1", //变量名到*的间隔
        "3**1", //数字到*的间隔
        "*111", //*到字母的间隔
        "*_11", //*到_的间隔

        "0++1", //空到+的间隔
        "1++1", //变量名到+的间隔
        "3++1", //数字到+的间隔
        "+111", //+到字母的间隔
        "+_11", //+到_的间隔

        //"0001",//空格到空格的间隔
        //"*001",//*到空格的间隔

        "0,01", //空到,的间隔
        "1,01", //变量名到,的间隔
        //",111",//,到字母的间隔
        //",_11",//,到_的间隔

        "0==1", //空到=的间隔
        "1==1", //变量名到=的间隔
        "=111", //=到变量名的间隔
        "=331", //=到数字的间隔
        "0331", //空到数字的间隔

        "0>>1", //空到>的间隔
        "1>>1", //变量到>的间隔
        ">>20", //>>的延续

        "0<<1", //空到<的间隔
        "1<<1", //变量到<的间隔
        "<<20", //<<的延续
        "2111", //<<到变量的间隔
        "2_11", //<<到_的间隔

        "1+21",

};

char check_table[100][5] = //语法规则表
    {
        "0##1", //开始到#的间断
        "#1i1", //#到include的间断
        "i1i0", //include内的字母延续
        "i<<1", //include到<的间断
        "<1h1", //<到头文件的间断
        "h1h0", //头文件的字母延续
        "h.h0", //头文件的.号延续
        "h>>1", //头文件到>的间断

        "0111", //空到字母的间断
        "0_11", //开始到_的间断,首字母不为数字
        "1110", //变量名的延续,可以承接_,数字
        "1_10", //变量名到_的延续
        "1310", //变量名到数字延续
        "1001", //变量名到空格的间断
        "0;01", //空到;的间隔
        "1;01", //变量名到;的间断
        "3;01", //数字到;的间隔
        "3.30",//数字到.的延续
        "3330",//数字到数字的延续
        "0331", //空到数字的间隔

        "1((1", //函数名到(的间断
        //"(001",//括号到空格的间隔
        "(111", //括号到字母的间隔
        "(_11", //括号到_的姐间隔

        "0**1", //空到*的间隔
        "1**1", //变量名到*的间隔
        "3**1", //数字到*的间隔

        "*111", //*到字母的间隔
        "*_11", //*到_的间隔

        "0++1", //空到+的间隔
        "1++1", //变量名到+的间隔
        "3++1", //数字到+的间隔
        "+111", //+到字母的间隔
        "+_11", //+到_的间隔

        //"0001",//空到空格的间隔
        //"*001",//*到空格的间隔

        "0,01", //空到,的间隔
        "1,01", //变量名到,的间隔
        //",111",//,到字母的间隔
        //",_11",//,到_的间隔

        "0==1", //空到=的间隔
        "1==1", //变量名到=的间隔
        "=111", //=到变量名的间隔
        "=331", //=到数字的间隔
        "0331", //空到数字的间隔

        "0{01", //空到{的间隔
        "){01", //)到{的间隔
        "0}01", //空到}的间隔

        "0>>1", //空到>的间隔
        "1>>1", //变量到>的间隔
        ">>20", //>>的延续

        "0<<1", //空到<的间隔
        "1<<1", //变量到<的间隔
        "<<20", //<<的延续
        "2111", //<<到变量的间隔
        "2_11", //<<到_的间隔

        "1+21", //
        "0\"\"1",//空与"的间隔
        "=\"\"1",//=与"的间隔
        "2\"\"1",//符号和"的间隔
        //"1\"\"1",//字母和"的间隔
        "\"0s1",//"和空的间隔
        "\"1s1",//"和字母的间隔
        "\"2s1",//"和符号的间隔
        "\"3s1",//"和数字的间隔
        "s\"01",//字符串和"的间隔

        "s0s0",//字符串的延续
        "s1s0",
        "s2s0",
        "s3s0",
        "s\\\\0",//字符串与\的延续
        "\\\"s0",//\和"的延续
        ">\n01",
        "2\n01",
        "0\n01"
};



int pointout(string str, char now_char, int file_type)
{

}

int len_check_table = sizeof(check_table) / sizeof(check_table[0]); //记录c++语法表的长度
string keyword[100] = {"asm","auto","bool","break","case","catch","char","cin","class","const",
"const_cast","continue","cout","default","define","delete","do","double","dynamic_cast","else",
"endl","enum","explicit","export","extern","false","float","for","friend","goto","if","inline",
"int","long","mutable","namespace","new","operator","private","protected","public","register",
"reinterpret_cast","return","short","signed","sizeof","static","static_cast","struct","switch",
"template","this","throw","true","try","typedef","typeid","typename","union","unsigned","using"
,"virtual","void","volatile","wchar_t","while"};

string type_of_str[5] = {"变量名", "特殊符号", "数字", "字符串", "关键字"};

int len_translator_table = sizeof(translator_table) / sizeof(translator_table[0]); //记录自定义语言语法表的长度
string translator_keyword[100] = {"iostream", "int8", "__main__", "pyout", "pyend", "begin", "end", "use", "std"};

vector<pair<string, char>> vstrs;                     //装载所有的类型
string synxax_table[100]={};

// void translator(vector<pair<string, char>> vstrs)
// {

//     int len=vstrs.size();
//     for(int i=0;i<len;i++)
//     {
//         vstrs[i].second
//     }
// }

int pointin(string str, char now_char, int file_type=0) //输入(字符串,字符串的中间类型,扫描的语言类型0为c++,1为自定义语言),返回字符串的最终类型
{
    string *keyword_ = (file_type == 0 ? keyword : translator_keyword);
    if (now_char >= 48 && now_char <= 57) //如果是数字
    {
        if (now_char == 48)
            return 2;
        else
        {
            if (now_char - 48 == 1)
            {
                for (int i = 0; i < 100; i++)
                {
                    if (keyword_[i] == "")
                        break;
                    if (str == keyword_[i])
                        return 5;
                }
                return 1;
            }
            return (int)(now_char - 48);
        }
    }
    else if ((now_char >= 65 && now_char <= 90) || (now_char >= 97 && now_char <= 122)) //如果是字母
    {
        if(now_char=='s')return 4;
        return 5;
    }
    else //如果是符号
    {
        return 2;
    }
}



void scanner(string str, int file_type=0) //扫描每一行,file_type表明是扫描c++文件(file_type=0) 还是 自定义语言文件(file_type=1)
{
    char (*all_check_table)[5]=(file_type==0?check_table:translator_table);
    int len_all_check_table=(file_type==0?len_check_table:len_translator_table);
    char now_char = str[0];         //当前指向的字符
    int lenofeachline = str.size(); //代码行的长度
    int index = 0;                  //表示当前指向的字符下标
    string n = "";                  //所得到的token
    char now_type[5] = "0000";      //初始化当前字符状态
    while (index <= lenofeachline)
    {
        now_char = str[index];
        now_type[0] = now_type[2];
        if (now_char >= 48 && now_char <= 57) //如果是数字
        {
            now_type[1] = '3';
        }
        else if ((now_char >= 65 && now_char <= 90) || (now_char >= 97 && now_char <= 122)) //如果是字母
        {
            now_type[1] = '1';
        }
        else //如果是符号
        {
            if (now_char == ' ')
            {
                if (now_type[0] != 's') //如果不在字符串中
                {
                    if (n != "" && now_type[0] != '0') //如果此时的n储存到内容,并且上一次内容不为0
                    {
                        pair<string, char> *last_str_type = new pair<string, char>(n, now_type[0]);
                        vstrs.push_back(*last_str_type);
                        n = "";
                    }
                    now_type[2] = '0';
                    index++;
                    continue;
                }
                else //如果在字符串中,空格认为是字母
                {
                    now_type[1] = '1';
                }
            }
            else if (now_char == '<')now_type[1] = '<';
            else if (now_char == '>')now_type[1] = '>';
            else if (now_char == '.')now_type[1] = '.';
            else if (now_char == ';')now_type[1] = ';';
            else if (now_char == '{')now_type[1] = '{';
            else if (now_char == '}')now_type[1] = '}';
            else if (now_char == '(')now_type[1] = '(';
            else if (now_char == '_')now_type[1] = '_';
            else if (now_char == ')')now_type[1] = ')';
            else if (now_char == '=')now_type[1] = '=';
            else if (now_char == '+')now_type[1] = '+';
            else if (now_char == '\"')now_type[1] = '\"';
            else if (now_char == '\\')now_type[1] = '\\';
            else if (now_char == '@')now_type[1] = '@';
            else if (now_char == '#')now_type[1] = '#';
            else if (now_char == '\n')now_type[1]='\n';
            else now_type[1] = '2';
        }
        for (int i = 0; i < len_all_check_table; i++) //匹配规则表
            {
                if (all_check_table[i][0] == now_type[0] && all_check_table[i][1] == now_type[1])
                {
                    now_type[2] = all_check_table[i][2];
                    now_type[3] = all_check_table[i][3];
                    break;
                }
            }
        if (now_type[3] != '0')
        {
            if (n != "")
            {
                pair<string, char> *last_str_type = new pair<string, char>(n, now_type[0]);
                //cout<<"values:"<<last_str_type->first<<"  type:"<<last_str_type->second<<endl;
                vstrs.push_back(*last_str_type);
                n = "";
            }
        }
        if(now_char!='\n')n = n + now_char;
        index++;
    }
    if (file_type == 1 && n[0] != '\0') //因为c++的每一行最后都有一个结束符 ;,},>等,但pycpp语言中不一定有,所以在一行的末尾判断,是否有没存入符号表的符号
    {
        pair<string, char> *last_str_type = new pair<string, char>(n, now_type[0]);
        vstrs.push_back(*last_str_type);
    }
}

int main()
{
    ifstream in("./compilers/main.cpp");
    string str;
    if(in){
        while(getline(in,str)){
            scanner(str);//假如是单个参数 ,默认为扫描cpp文件
        }
    }
    else{
        cout<<"file is not exist"<<endl;
    }
    for(int i=0;i<vstrs.size();i++){
        cout<<i<<"\t"<<"values:"<<vstrs[i].first<<"\ttype:"<<type_of_str[pointin(vstrs[i].first,vstrs[i].second)-1]<<endl;
    }
    vstrs.clear();
    ifstream in_("./compilers/pyc++.pycpp");
    
    if(in){
        while(getline(in_,str)){
            scanner(str,1);//假如有两个参数,并且第二个参数为1,那么扫描的是自定义文件
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

