#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<stack>
#include<algorithm>
using namespace std;

//---------------------正则表达式的分词部分------------------------------
//分词规则表
char syntaxTable_RegularExpressions[100][6]=
{

};
int len_check_table = sizeof(syntaxTable_RegularExpressions) / sizeof(syntaxTable_RegularExpressions[0]);
int nextTypeIndex0[200];//用来记录 某个词类在词法表中首次出现的下标
vector<pair<string, char>> vstrs;
int compar_words(const void* p, const void* q)
{
    return strcmp((char*)p, (char*)q);
}
void init_table(int file_type = 0)//把语法规则表进行排序,并且获得不同语法规则的承接关系,以便搜索规则时具有更快的速度
{
    int* nextTypeIndex = nextTypeIndex0 ;

    char(*all_check_table)[6] = syntaxTable_RegularExpressions ;
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

void scanner(string str, int file_type = 0) //扫描每一行,file_type表明是扫描c++文件(file_type=0) 还是 自定义语言文件(file_type=1)
{
    int* nextTypeIndex = = nextTypeIndex0;
    unsigned int last_start = 0;//记录本次的匹配语法规则,相承接的下一次规则起始点
    char(*all_check_table)[6] = syntaxTable_RegularExpressions;
    int len_all_check_table =  len_check_table ;
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
//the results of separation are stored in vstrs
//-------------------------------------regular_expression syntax scanner ending------------






//-----------------------------------------------------------------------------------------
//confirmation syntax execution order

typedef class _node{
public:
    vector<int> numlist;//节点的下标
    vector<pair<string,int>> link;//节点的路径
    int lenLink;
    _node(int index)
    {
        lenLink=0;
        numlist.push_back(index);
    }
}*node;
struct _node tree[200];
stack<char> symStack;//符号栈
stack<node> nodeStack;//节点栈

int isp(char a)
{
	switch (a)
	{

	case '+':
	case '-':return 3;
	case '*':
	case '/':
	case '%':return 5;
	case '(':return 1;

	case ')':return 6;
	default:return 0;
	}
}
int icp(char a)
{
	switch (a)
	{
	case '+':
	case '-':return 2;
	case '*':
	case '/':
	case '%':return 4;
	case '(':return 6;
	case ')':return 1;
	default:return 0;
	}
}


void operateBysym(char sym,vector<node> arr)
{
    if()
}

void pushvstrs()
{
    int nodeIndex=0;
    int lenVstrs=vstrs.size();
    for(int i=0;i<lenVstrs;i++)
    {
        if(vstrs[i].second=='1')
        {
            node nodeptr=new _node(nodeIndex);//通过nodeIndex初始化节点值
        }
        else{
            if(vstrs[i].first=="|"){
                operateBysym()
            }
            else if(vstrs[i].first=="("){
            }
            else if(vstrs[i].first==")"){
            }
            else if(vstrs[i].first=="["){
            }
            else if(vstrs[i].first==""){
            }
            else if(vstrs[i].first=="*"){
            }
            else if(vstrs[i].first=="?"){
            }
            else{

            }
        }
    }
}
//-----------------------------------------------------------------------------------------


int main(){
    string regularExpressions="";//正则表达式
    cin>>regularExpressions;
    int len_regularExpressions=regularExpressions.size();
    for(int i=0;i<len_regularExpressions;i++)
    {
        
    }
}