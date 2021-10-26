#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<stack>
#include<queue>
#include<set>
#include<algorithm>
using namespace std;

//---------------------正则表达式的分词部分------------------------------
//分词规则表
//char类型:空格:0 字母:1 符号:2 数:3
//string类型:空或行末:0,变量名:1 特殊符号:2 数字:3 字符串:4 关键字:5 字符:6 注释:7

char syntaxTable_RegularExpressions[100][6]=
{
    "01110", //空到字母的间断
    "0_110", //开始到_的间断,首字母不为数字
    "11100", //变量名的延续,可以承接_,数字
    "1_100", //变量名到_的延续
    "13100", //变量名到数字延续
    "10010", //变量名到空格的间断
    "1((10", //变量名到(的间断
    "(1110", //(到字母的间断
    "1||10", //字母到|的间断
    "1))10", //字母到)的间断
    "|1110", //|到字母的间断
    ")**10", //)到*的间断
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
    int* nextTypeIndex = nextTypeIndex0;
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
            //cout << last_start << endl;
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
            else if (now_char == '|')now_type[1] = '|';
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
    vector<pair<string,int>> link;//节点的路径,如果int为-1,即为终止状态
    void pushLink(string data,int nextNodeIndex)
    {
        pair<string,int> tempLink;
        tempLink.first=data;
        tempLink.second=nextNodeIndex;
        link.push_back(tempLink);
    }
    virtual void deleteNode()
    {
        link.clear();
    }
}*node;
_node nodeArr[200];//存放节点
int nowNodeNum;//记录当前

typedef class _nodeDFA :public _node
{
public:
    int isTheEnd;

    set<int> numList;//节点的下标集合
    void pushNode(int index)
    {
        numList.insert(index);
    }
    bool cmpWithSet(const nodeDFA a,const nodeDFA b)
    {
        return a->numList==b->numList;
    }
    virtual void deleteNode()
    {
        link.clear();
        numList.clear();
    }
}*nodeDFA;
_nodeDFA nodeDFAArr[100];//存放节点
int nowNodeDFANum;
void deletearr(node a){
    a->deleteNode();
}

typedef class _tree{
public:
    int front;
    int end;
    _tree(int f,int e):front(f),end(e){}
    void copyTree(int fromNodeIdx,int toNodeIdx)//将链表内的一串节点复制到新的下标
    {
        bfs(fromNodeIdx,findminmax);
        cout<<minmax.first<<" "<<minmax.second<<endl;
        int bias=nowNodeNum-minmax.first;
        

    }
    pair<int,int> minmax;

    void findminmax(int nowIndex)
    {
        if(nowIndex<minmax.first)minmax.first=nowIndex;
        if(nowIndex>minmax.second)minmax.second=nowIndex;
    }
    void bfs(int fromNodeIdx,void (_tree::*func)(int nowIndex))
    {
        if(func==findminmax){
            this->minmax.first=10000;
            this->minmax.second=0;
        }
        set<int> isVisited;
        queue<int> que;
        int p;
        que.push(fromNodeIdx);
        while (!que.empty()&&p==-1)
        {
            p=que.front();que.pop();
            if( isVisited.count(p)>=1){
                p=-1;
                continue;
            }
            isVisited.insert(p);
            func(p);
            for(int i=0;i<nodeArr[p].link.size();i++)
            {
                que.push(nodeArr[p].link[i].second);
            }
        }
    }
}*tree;//既可以表示nfa树,也可以表示dfa树

stack<char> symStack;//符号栈
stack<tree> treeStack;//节点栈
//符号优先级的执行规则
//如果栈外的符号优先级小于栈内优先级,从符号栈拿出一个符号,
//从变量栈拿出两个变量,进行相应操作,
//如果栈外的符号优先级大于栈内优先级,则继续把该符号放入符号栈
int isp(char a)//栈内优先级别
{
	switch (a)
	{

	case '+':
	case '-':return 3;//一般比外面的相同符号多一级,倾向把栈内的符号进行计算
	case '*':
	case '/':
	case '%':return 5;
	case '(':return 1;//在栈内(应该尽量小

	case ')':return 6;//其实可以不用这个,因为')'不会入栈,而是直接执行到上一个
	default:return 0;
	}
}
int icp(char a)//栈外优先级别,优先级是针对双目运算符的,单目运算符是直接执行的
{
	switch (a)
	{
	case '+':
	case '*':
	case '?':return 2;
    case '-':
	case '[':return 6;
	case ']':return 1; 
	case '(':return 6;
	case ')':return 1;
	default:return 0;
	}
}

node operateBysym_3(char sym)
{

}

node operateBysym_2(char sym)
{
    
}

node operateBysym_1(char sym)
{
    tree operateNode=treeStack.top();treeStack.pop();//把节点取出
    if(sym=='?')
    {
        nodeArr[nowNodeNum].pushLink("",operateNode->front);//将原有的头连接新头
        operateNode->front=nowNodeNum;//更新头节点
        nodeArr[operateNode->end].pushLink("",nowNodeNum+1);//将原有的尾连接新尾
        operateNode->end=nowNodeNum+1;//更新尾节点
        nodeArr[nowNodeNum].pushLink("",operateNode->end);//将头节点直接连到新尾节点
        nowNodeNum+=2;//将节点数置为标记过的节点下标
    }
    if(sym=='*')
    {
        nodeArr[operateNode->end].pushLink("",operateNode->front);//将原有的尾连接尾
        nodeArr[nowNodeNum].pushLink("",operateNode->front);//将新的头连接原有的头
        operateNode->front=nowNodeNum;//更新头节点
        nodeArr[operateNode->end].pushLink("",nowNodeNum+1);//将原有的尾连接新尾
        operateNode->end=nowNodeNum+1;//更新尾节点
        nodeArr[nowNodeNum].pushLink("",operateNode->end);//将头节点直接连到新尾节点
        nowNodeNum+=2;//将节点数置为标记过的节点下标
    }
    if(sym=='+')
    {
        
        nodeArr[operateNode->end].pushLink("",operateNode->front);//将原有的尾连接尾
        nodeArr[nowNodeNum].pushLink("",operateNode->front);//将新的头连接原有的头
        operateNode->front=nowNodeNum;//更新头节点
        nodeArr[operateNode->end].pushLink("",nowNodeNum+1);//将原有的尾连接新尾
        operateNode->end=nowNodeNum+1;//更新尾节点
        nodeArr[nowNodeNum].pushLink("",operateNode->end);//将头节点直接连到新尾节点
        nowNodeNum+=2;//将节点数置为标记过的节点下标
        nodeArr[nowNodeNum]
    }
    treeStack.push(operateNode);
    
}

void pushvstrs()
{
    nowNodeNum=0;//初始节点数量
    char tempChar,symStackTopchar;
    int lenVstrs=vstrs.size();
    for(int i=0;i<lenVstrs;i++)
    {
        if(vstrs[i].second=='1')
        {
            nodeArr[nowNodeNum].pushLink(vstrs[i].first,nowNodeNum+1);//和下一个节点连接起来
            tree mytree=new _tree(nowNodeNum,++nowNodeNum);//将头和尾连接起来
            nowNodeNum++;
            treeStack.push(mytree);
        }
        else{
            tempChar=vstrs[i].first[0];
            symStackTopchar=symStack.top();
            if(tempChar=='*'||tempChar=='+'||tempChar=='?')//如果是单目运算符
            {
                operateBysym_1(tempChar);
            }
            else if(tempChar=='^'||tempChar=='-')//如果是范围运算符
            {
                symStack.push(tempChar);//将当前的范围限定符号入栈
                continue;
            }
            else if(icp(tempChar)>isp(symStackTopchar))//如果是双目运算符
            {
                symStack.push(tempChar);//将当前的符号入栈
                continue;
            }
            else if(icp(tempChar)<isp(symStackTopchar) && tempChar != ')' && tempChar != ']')
            {
                while(icp(tempChar)<isp(symStackTopchar))
                {
                    operateBysym_2(tempChar);
                }
            }
            else if(tempChar != ')')//如果是),那么直到(才结束
            {
                char aa=symStack.top();symStack.pop();
                while(aa!='(')
                {
                    operateBysym_1(aa);
                    aa=symStack.top();symStack.pop();
                }
            }
            else if(tempChar != ']')//如果是],那么直到[才结束
            {
                char aa=symStack.top();symStack.pop();
                while(aa!='[')
                {
                    operateBysym_3(aa);
                    aa=symStack.top();symStack.pop();
                }
            }
        }
    }
}
//-----------------------------------------------------------------------------------------






















int main(){
    string regularExpressions="digtal(char|digtal)*";//正则表达式
    //cin>>regularExpressions;
    init_table();
    scanner(regularExpressions);
    int len_vstrs=vstrs.size();
    for(int i=0;i<len_vstrs;i++)
    {
        cout<<vstrs[i].first<<" "<<vstrs[i].second <<endl;
    }
    system("pause");
}