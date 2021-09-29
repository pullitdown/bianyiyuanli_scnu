#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<algorithm>
using namespace std;


//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5
char translator_table[100][6] = //预编译语法规则表
{
        "0@@10",
        "@1h10",
        "h1h00",
        "h.h00", //头文件的引入 eg:@iostream

        "01110", //空到字母的间断
        "0_110", //开始到_的间断,首字母不为数字
        "11100", //变量名到字母的延续,可以承接_,数字
        "1_100", //变量名到_的延续
        "13100", //变量名到数字延续

        "1((10", //函数名到(的间断
        //"(0010",//括号到空格的间隔
        "(1110", //括号到字母的间隔
        "(_110", //括号到_的姐间隔

        "0**10", //空到*的间隔
        "1**10", //变量名到*的间隔
        "3**10", //数字到*的间隔
        "*1110", //*到字母的间隔
        "*_110", //*到_的间隔

        "0++10", //空到+的间隔
        "1++10", //变量名到+的间隔
        "3++10", //数字到+的间隔
        "+1110", //+到字母的间隔
        "+_110", //+到_的间隔

        //"00010",//空格到空格的间隔
        //"*0010",//*到空格的间隔

        "0,010", //空到,的间隔
        "1,010", //变量名到,的间隔
        //",1110",//,到字母的间隔
        //",_110",//,到_的间隔

        "0==10", //空到=的间隔
        "1==10", //变量名到=的间隔
        "=1110", //=到变量名的间隔
        "=3310", //=到数字的间隔
        "03310", //空到数字的间隔

        "0>>10", //空到>的间隔
        "1>>10", //变量到>的间隔
        ">>200", //>>的延续

        "0<<10", //空到<的间隔
        "1<<10", //变量到<的间隔
        "<<200", //<<的延续
        "21110", //<<到变量的间隔
        "2_110", //<<到_的间隔

        "1+210",

};

char check_table[100][6] = //语法规则表
    {
        "0##10", //开始到#的间断
        "#1i10", //#到include的间断
        "i1i00", //include内的字母延续
        "i<<10", //include到<的间断
        "<1h10", //<到头文件的间断
        "h1h00", //头文件的字母延续
        "h.h00", //头文件的.号延续
        "h>>10", //头文件到>的间断

        "01110", //空到字母的间断
        "0_110", //开始到_的间断,首字母不为数字
        "11100", //变量名的延续,可以承接_,数字
        "1_100", //变量名到_的延续
        "13100", //变量名到数字延续
        "10010", //变量名到空格的间断
        "0;010", //空到;的间隔
        "1;010", //变量名到;的间断
        "3;010", //数字到;的间隔
        "3.300",//数字到.的延续
        "33300",//数字到数字的延续
        "03310", //空到数字的间隔

        "1((10", //函数名到(的间断
        //"(0010",//括号到空格的间隔
        "(1110", //括号到字母的间隔
        "(_110", //括号到_的姐间隔

        "0**10", //空到*的间隔
        "1**10", //变量名到*的间隔
        "3**10", //数字到*的间隔

        "*1110", //*到字母的间隔
        "*_110", //*到_的间隔

        "0++10", //空到+的间隔
        "1++10", //变量名到+的间隔
        "3++10", //数字到+的间隔
        "+1110", //+到字母的间隔
        "+_110", //+到_的间隔

        //"00010",//空到空格的间隔
        //"*0010",//*到空格的间隔

        "0,010", //空到,的间隔
        "1,010", //变量名到,的间隔
        //",1110",//,到字母的间隔
        //",_110",//,到_的间隔

        "0==10", //空到=的间隔
        "1==10", //变量名到=的间隔
        "=1110", //=到变量名的间隔
        "=3310", //=到数字的间隔
        "03310", //空到数字的间隔

        "0{010", //空到{的间隔
        "){010", //)到{的间隔
        "0}010", //空到}的间隔

        "0>>10", //空到>的间隔
        "1>>10", //变量到>的间隔
        ">>200", //>>的延续

        "0<<10", //空到<的间隔
        "1<<10", //变量到<的间隔
        "<<200", //<<的延续
        "21110", //<<到变量的间隔
        "2_110", //<<到_的间隔

        "1+210", //
        "0\"\"10",//空与"的间隔
        "=\"\"10",//=与"的间隔
        "2\"\"10",//符号和"的间隔
        //"1\"\"10",//字母和"的间隔
        "\"0s10",//"和空的间隔
        "\"1s10",//"和字母的间隔
        "\"2s10",//"和符号的间隔
        "\"3s10",//"和数字的间隔
        "s\"010",//字符串和"的间隔

        "s0s00",//字符串的延续
        "s1s00",
        "s2s00",
        "s3s00",
        "s\\\\00",//字符串与\的延续
        "\\\"s00",//\和"的延续
        ">\n010",
        "2\n010",
        "0\n010"
};
//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5
//行类型:句子完成0,1引入,2命名空间,3函数命名或变量命名,4函数参数,5括号代替,6变量赋值,7特殊输入

//语句语法表
//定义一个不定长跳表,用另一个数组记录每一个item的起始位置,
int synx_table[10000]
{
    //每四格作为一个语法规则
    //第一个数字代表上一个行类型,第二格数字代表该string的类型
    //第三格数字代表加上该strin后,本行的行类型,第四格数字代表是否要分割
    //那么要确定的就是第二格的参数设置
    //如果是关键字,那么取负值的(关键字数组下标+100)
    //如果是符号,直接取符号本身
    //如果是数字,取-3,变量名取-1,字符串取-4,
    0,'@',1,1,//空到@的间隔
    1,'h',1,0,//引入到头文件延续
    1,'@',1,1,//引入到@的间断
    1,-162,2,1,//引入到use的间断
    2,1,2,0,//use到变量(变量空间)的延续
    2,-133,3,1,//变量空间到函数定义类型的延续
    2,-134,3,1,//int16
    2,-102,3,1,//bool
    2,-126,3,1,//float
    3,-168,3,0,//函数命名到主函数的延续
    3,'(',4,0,//函数命名到(的延续
    4,-133,4,0,//(到函数定义类型的延续
    4,-134,4,0,//int16
    4,-102,4,0,//bool
    4,-126,4,0,//float
    4,1,4,0,//函数参数到变量名的延续
    4,',',',',0,//函数参数到变量名的延续
    
    3,1,3,0,//函数返回类型到函数名的延续

    3,'(',3,0,


};

// int translator_table[10000]=
// {
//     //不同类型的转换规则不同


// };

bool cmp(char arr1[6],char arr2[6])
{
    return strcmp(arr1,arr2);
}
void init_table()//把语法规则表进行排序,并且获得不同语法规则的承接关系,以便搜索规则时具有更快的速度
{
    sort(check_table,check_table+20,cmp);
    for(int i=0;i<90;i++)
    {
        cout<<check_table[i]<<endl;
    }
}


int len_check_table = sizeof(check_table) / sizeof(check_table[0]); //记录c++语法表的长度
string keyword[100] = {
//     0       1           2         3                    4           5          6            7           8            9           
"asm"       ,"auto"    ,"bool"    ,"break"           ,"case"     ,"catch"   ,"char"       ,"cin"     ,"class"       ,"const",
"const_cast","continue","cout"    ,"default"         ,"define"   ,"delete"  ,"do"         ,"double"  ,"dynamic_cast","else",
"endl"      ,   "enum" ,"explicit","export"          ,"extern"   ,"false"   ,"float"      ,"for"     ,"friend"      ,"goto",
"if"        ,"include" ,"inline"  ,"int"             ,"long"     ,"mutable" ,"namespace"  ,"new"     ,"operator"    ,"private" ,
"protected" ,"public"  ,"register","reinterpret_cast","return"   ,"short"   ,"signed"     ,"sizeof"  ,"static"      ,"static_cast",
"struct"    ,"switch"  ,"template","this"            ,"throw"    ,"true"    ,"try"        ,"typedef" ,"typeid"      ,"typename",
"union"     ,"unsigned","using"   ,"virtual"         ,"void"     ,"volatile","wchar_t"    ,"while"   ,"main"
};

string pykeyword[100]={
//     0       1           2         3                    4           5          6            7           8            9           
"asm"       ,"auto"    ,"bool"    ,"break"           ,"case"     ,"catch"   ,"char"       ,"pyin"    ,"class"       ,"const",
"const_cast","continue","pyout"   ,"default"         ,"define"   ,"delete"  ,"do"         ,"double"  ,"dynamic_cast","else",
"pyend"     ,   "enum" ,"explicit","export"          ,"extern"   ,"false"   ,"float"      ,"for"     ,"friend"      ,"goto",
"if"        ,"@"       ,"inline"  ,"int8"            ,"int16"    ,"mutable" ,"namespace"  ,"new"     ,"operator"    ,"private" ,
"protected" ,"public"  ,"register","reinterpret_cast","callback" ,"int4"    ,"signed"     ,"sizeof"  ,"static"      ,"static_cast",
"struct"    ,"switch"  ,"template","this"            ,"throw"    ,"true"    ,"try"        ,"typedef" ,"typeid"      ,"typename",
"union"     ,"unsigned","use"     ,"virtual"         ,"void"     ,"volatile","wchar_t"    ,"while"   ,"__main__"
};

string type_of_str[5] = {"变量名", "特殊符号", "数字", "字符串", "关键字"};

int len_translator_table = sizeof(translator_table) / sizeof(translator_table[0]); //记录自定义语言语法表的长度
string translator_keyword[100] = {"iostream", "int8", "__main__", "pyout", "pyend", "begin", "end", "use", "std"};

vector<pair<string, char>> vstrs;                     //装载所有的类型



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
    int last_start=0;//记录本次的匹配语法规则,相承接的下一次规则起始点
    char (*all_check_table)[6]=(file_type==0?check_table:translator_table);
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
        for (int i = last_start; i < len_all_check_table; i++) //匹配规则表
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


//
