#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<algorithm>

using namespace std;


//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5
char translator_table[200][6] = //预编译语法规则表
{
        "0@@10",
        "@1h10",
        "h1h00",
        "h.h00", //头文件的引入 eg:@iostream
        "h\n010",

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

char check_table[200][6] = //语法规则表
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
    "3))10",//数字到)的间隔
    "1))10",//字符到)的间隔
    ");010",//)到;的间隔

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
    "0\n010",
    "1[[10",//变量名到中括号的间断
    "[3310",//中括号到数字的间断
    "3]]10",//数值到中括号的间断
    "{3310",//括号到数字的间断
    "3}}10",//数字到}的间断
    "};010",//}到;的间断
    "]==10",//]到=的间断
    "={{10",//=到{的间断
    "3,010",//数字到,的间断
    "=\'\'10",//等号到'的j
    "\'0610",
    "0\'\'10",
    "6\'\'10",
    "\'1610",
    "\';010",
    "\'2610",
    "\'3610",

    "(\"\"10",
    "\",010",
    "0//10",
    "1//10",
    "2//10",
    "3//10",
    "4//10",
    "5//10",
    "6//10",
    "7//00",
    "//700",
    "70700",
    "71700",
    "72700",
    "73700",
    "7\n710",
    "/*z00",
    "z0700",
    "z1700",
    "z2700",
    "z3700",
    "z*Z00",
    "Z/700",

 
};





string pykeyword[100] = {
    //     0       1           2         3                    4           5          6            7           8            9           
    "asm"       ,"auto"    ,"bool"    ,"break"           ,"case"     ,"catch"   ,"char"       ,"pyin"    ,"class"       ,"const",
    "const_cast","continue","pyout"   ,"default"         ,"define"   ,"delete"  ,"do"         ,"double"  ,"dynamic_cast","else",
    "pyend"     ,   "enum" ,"explicit","export"          ,"extern"   ,"false"   ,"float"      ,"for"     ,"friend"      ,"goto",
    "if"        ,"end"     ,"inline"  ,"int8"            ,"int16"    ,"mutable" ,"namespace"  ,"new"     ,"operator"    ,"private" ,
    "protected" ,"public"  ,"register","reinterpret_cast","callback" ,"int4"    ,"signed"     ,"sizeof"  ,"static"      ,"static_cast",
    "struct"    ,"switch"  ,"template","this"            ,"throw"    ,"true"    ,"try"        ,"typedef" ,"typeid"      ,"typename",
    "union"     ,"unsigned","use"     ,"virtual"         ,"void"     ,"volatile","wchar_t"    ,"while"   ,"__main__"    ,"begin",

};
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
int sys[200] = {
    '@',0,-1,1,'#',-131,'<',-1,'>','\n',-999,
    ';',1,';','\n',-999,
    -162,-1,1,-162,' ',-136,' ',-1,-999,
    -133,1,-133,' ',-999,
    -134,1,-134,' ',-999,
    -145,1,-145,' ',-999,
    -144,1,-144,' ',-999,
    -168,1,-168,-999,
    -169,1,'{','\n',-999,
    -131,1,'}',-999,
    -112,1,-112,-999,
    -120,1,-120,-999
};
int level[50];
int level_len = 0;
void init_sys()
{
    level[level_len++] = -1;
    for (int i = 0; i <= 200; i++)
    {
        if (sys[i] == -999)level[level_len++] = i;
    }
    level_len -= 1;
    //cout<<level_len<<endl;
}





//--------------------------init---------------------------------------------------------------------------------------------
int nextTypeIndex0[200];//用来记录 某个词类在词法表中首次出现的下标
int nextTypeIndex1[200];
int compar_words(const void* p, const void* q)
{
    return strcmp((char*)p, (char*)q);
}
void init_table(int file_type = 0)//把语法规则表进行排序,并且获得不同语法规则的承接关系,以便搜索规则时具有更快的速度
{
    int* nextTypeIndex = file_type == 1 ? nextTypeIndex1 : nextTypeIndex0;

    char(*all_check_table)[6] = (file_type == 0 ? check_table : translator_table);
    int lenOfCheckTable = -1;//获取词法表的长度
    while (all_check_table[++lenOfCheckTable][0] != '\0');
    qsort(all_check_table, lenOfCheckTable, 6, compar_words);//对词法表进行排序
    //memset(nextTypeIndex,9999,200*8);//设置初值为300
    for (int i = 0; i < 200; i++) { nextTypeIndex[i] = 300; }

    int temp;
    for (int i = 0; i < lenOfCheckTable; i++)//对所有词法进行遍历
    {
        temp = nextTypeIndex[(int)all_check_table[i][0]];//获取某个词类在词法中的位置
        if (temp > i)//如果现在的下标比原来下标要小
            nextTypeIndex[all_check_table[i][0]] = i;//首次出现的位置改为i
    }

    for (int i = 0; i < lenOfCheckTable; i++)
        all_check_table[i][4] = nextTypeIndex[all_check_table[i][2]];
    //for (int i = 0; i < 200; i++)if (nextTypeIndex[i] != 300)cout << nextTypeIndex[i] << " " << nextTypeIndex0[i] << " " << nextTypeIndex1[i] << endl;
}
//---------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------
int len_check_table = sizeof(check_table) / sizeof(check_table[0]); //记录c++语法表的长度




string type_of_str[7] = { "变量名", "特殊符号", "数字", "字符串", "关键字" ,"字符","注释"};

int len_translator_table = sizeof(translator_table) / sizeof(translator_table[0]); //记录自定义语言语法表的长度


vector<pair<string, char>> vstrs;                     //装载所有的类型
//---------------------------------------------------------------------------------------------------------------------------
int pointout(string str, char now_char)
{
    string* keyword_ = pykeyword;
    if (now_char >= 48 && now_char <= 57)
    {
        if (now_char == 48)
            return str[0];
        else {
            if (now_char - 48 == 1)
            {
                for (int i = 0; i < 100; i++)
                {
                    if (keyword_[i] == "")
                        break;
                    if (str == keyword_[i])
                        return -100 - i;
                }

                return -1;
            }
            return -(int)(now_char - 48);
        }

    }
    else if ((now_char >= 65 && now_char <= 90) || (now_char >= 97 && now_char <= 122)) //如果是字母
    {
        if (now_char == 's')return 4;
        return -5;
    }
    else //如果是符号
    {
        return now_char;
    }
}

int pointin(string str, char now_char, int file_type = 0) //输入(字符串,字符串的中间类型,扫描的语言类型0为c++,1为自定义语言),返回字符串的最终类型
{
    string* keyword_ = (file_type == 0 ? keyword : pykeyword);
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
        if (now_char == 's')return 4;
        return 5;
    }
    else //如果是符号
    {
        return 2;
    }
}
//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5

void scanner(string str, int file_type = 0) //扫描每一行,file_type表明是扫描c++文件(file_type=0) 还是 自定义语言文件(file_type=1)
{
    int* nextTypeIndex = file_type == 0 ? nextTypeIndex0 : nextTypeIndex1;
    unsigned int last_start = 0;//记录本次的匹配语法规则,相承接的下一次规则起始点
    char(*all_check_table)[6] = (file_type == 0 ? check_table : translator_table);
    int len_all_check_table = (file_type == 0 ? len_check_table : len_translator_table);
    char now_char = str[0];         //当前指向的字符
    int lenofeachline = str.size(); //代码行的长度
    int index = 0;                  //表示当前指向的字符下标
    string n = "";                  //所得到的token
    char now_type[6] = "0000\0";      //初始化当前字符状态
    while (index <= lenofeachline)
    {
        now_char = str[index];
        now_type[0] = now_type[2];
        last_start = now_type[4];
        if (last_start < 0)
        {
            last_start = 256 + last_start;
            cout << last_start << endl;
        }
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
                        pair<string, char>* last_str_type = new pair<string, char>(n, now_type[0]);
                        vstrs.push_back(*last_str_type);
                        n = "";
                    }
                    now_type[2] = '0';
                    index++;
                    now_type[4] = nextTypeIndex[(int)now_type[2]];
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
            else if (now_char == '[')now_type[1] = '[';
            else if (now_char == ']')now_type[1] = ']';
            else if (now_char == '(')now_type[1] = '(';
            else if (now_char == '_')now_type[1] = '_';
            else if (now_char == ')')now_type[1] = ')';
            else if (now_char == '*')
            now_type[1] = '*';
            else if (now_char == '/')
            now_type[1] = '/';
            else if (now_char == '=')now_type[1] = '=';
            else if (now_char == ',')now_type[1] = ',';
            else if (now_char == '+')now_type[1] = '+';
            else if (now_char == '\"')now_type[1] = '\"';
            else if (now_char == '\\')now_type[1] = '\\';
            else if (now_char == '@')now_type[1] = '@';
            else if (now_char == '#')now_type[1] = '#';
            else if (now_char == '\n')
            now_type[1] = '\n';
            else if (now_char == '\'')
                now_type[1] = '\'';
            else now_type[1] = '2';
        }
        for (int i = last_start; i < len_all_check_table; i++) //匹配规则表
        {

            if (all_check_table[i][0] == now_type[0] && all_check_table[i][1] == now_type[1])
            {
                cout << last_start << endl;
                now_type[2] = all_check_table[i][2];
                now_type[3] = all_check_table[i][3];

                now_type[4] = all_check_table[i][4];

                break;
            }
        }
        if (now_type[3] != '0')
        {
            if (n != "")
            {
                pair<string, char>* last_str_type = new pair<string, char>(n, now_type[0]);
                //cout<<"values:"<<last_str_type->first<<"  type:"<<last_str_type->second<<endl;
                vstrs.push_back(*last_str_type);
                n = "";
            }
        }
        if (now_char != '\n')n = n + now_char;
        index++;
    }
    if (file_type == 1 && n[0] != '\0') //因为c++的每一行最后都有一个结束符 ;,},>等,但pycpp语言中不一定有,所以在一行的末尾判断,是否有没存入符号表的符号
    {
        pair<string, char>* last_str_type = new pair<string, char>(n, now_type[0]);
        vstrs.push_back(*last_str_type);
        n = "";
    }
}

string translator__(string text)
{
    string goal;
    init_sys();
    // ifstream in_("./compilers/pyc++.pycpp");
    // string str;
    // string text;
    // while(getline(in_,str)){
    //         text+=str;
    //         text+='\n';
    //     }
    // text+=' ';
    // in_.close();


    init_table(1);
    vstrs.clear();
    scanner(text, 1);
    for (int i = 0; i < vstrs.size(); i++)
        cout << i << "\t" << "values:" << vstrs[i].first << "\ttype:" << vstrs[i].second << endl;
    int type, flag;
    int i = 0;
    while (i < vstrs.size())
    {
        flag = 0;
        type = pointout(vstrs[i].first, vstrs[i].second);
        cout << "point out type:" << type << endl;
        for (int j = 0; j < level_len; j++)
        {

            if (sys[level[j] + 1] == type)
            {
                flag = 1;
                int step = level[j] + 2;
                string temp[10];
                int lenTemp = 0;//记录temp长度
                int indexTemp = 0;//记录temp的目前下标
                while (sys[step] != 1)
                {
                    if (sys[step] == -1)
                    {
                        i++;//到下一个字符串
                        temp[lenTemp++] = vstrs[i].first;
                    }
                    step++;
                }
                step++;//到达1分戈之后的数据
                while (step < level[j + 1])
                {
                    if (sys[step] > 0)goal += (char)sys[step];
                    else if (sys[step]<0 && sys[step]>-99 && indexTemp < lenTemp)goal += temp[indexTemp++];
                    else goal += keyword[-sys[step] - 100];
                    step++;
                }
                break;//如果规则符合,退出
            }
        }
        if (flag == 0)//如果没在转换表中得到数据
        {
            if (i <= vstrs.size())
                goal += vstrs[i].first;
            cout << "直接加入" << endl;
        }
        cout << goal << endl;
        i++;
    }
    return goal;
}

int main()
{
    init_table();
    ifstream in("C:/workspace/compilers/main.cpp");
    string str;
    if (in) {
        while (getline(in, str)) {
            scanner(str);//假如是单个参数 ,默认为扫描cpp文件
        }
    }
    else {
        cout << "file is not exist" << endl;
    }
    for (int i = 0; i < vstrs.size(); i++) {
        cout << i << "\t" << "values:" << vstrs[i].first << "\ttype:" << type_of_str[pointin(vstrs[i].first, vstrs[i].second) - 1] << endl;
    }

    vstrs.clear();

    // init_table(1);
    // ifstream in_("./compilers/pyc++.pycpp");
    // if(in_){
    //     while(getline(in_,str)){
    //         scanner(str,1);//假如是单个参数 ,默认为扫描cpp文件
    //     }
    // }
    // else{
    //     cout<<"file is not exist"<<endl;
    // }
    // for(int i=0;i<vstrs.size();i++){
    //     cout<<i<<"\t"<<"values:"<<vstrs[i].first<<"\ttype:"<<type_of_str[pointin(vstrs[i].first,vstrs[i].second)-1]<<endl;
    // }

//    cout<<translator__()<<endl;



    system("pause");


}
