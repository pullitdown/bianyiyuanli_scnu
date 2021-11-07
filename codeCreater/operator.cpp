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
char syntaxTable_RegularExpressions[100][6] =
{
    "0((10",//空到(的间断
    "01110", //空到字母的间断
    "0_110", //开始到_的间断,首字母不为数字
    "11100", //变量名的延续,可以承接_,数字
    "1_100", //变量名到_的延续
    "13100", //变量名到数字延续
    "10010", //变量名到空格的间断
    "1((10", //变量名到(的间断
    "1++10",//变量名到+的间断
    "+1110",//+到变量名的间断
    "+((10",//+到(的间断
    "1**10",//变量名到*的间断
    "*((10",//*到(的间断
    "*1110",//*到变量名的间断
    "1..10",
    ".1110",
    ".((10",
    ")((10",
    "(1110", //(到字母的间断
    "1||10", //字母到|的间断
    "1))10", //字母到)的间断
    "|1110", //|到字母的间断
    ")**10", //)到*的间断
    ")..10",//)到.的间断
    "*..10",//*到.的间断
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
    int* nextTypeIndex = nextTypeIndex0;

    char(*all_check_table)[6] = syntaxTable_RegularExpressions;
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
    int len_all_check_table = len_check_table;
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
                //cout << last_start << endl;
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

typedef class _node {
public:
    vector<pair<string, int>> link;//节点的路径,如果int为-1,即为终止状态
    void pushLink(string data, int nextNodeIndex)
    {
        pair<string, int> tempLink;
        tempLink.first = data;
        tempLink.second = nextNodeIndex;
        link.push_back(tempLink);
    }
    virtual void deleteNode()
    {
        link.clear();
    }
}*node;
_node nodeArr[200];//存放节点
int nowNodeNum;//记录当前


void deletearr(node a) {
    a->deleteNode();
}

//----------------------------------------------------------------
typedef int (*func)(int);//定义了func函数指针
pair<int, int> minmax_;
int visit(int nowIndex)
{
    cout << "现在访问的节点是" << nowIndex << "   \n他的路径信息是" << endl;
    for (int i = 0; i < nodeArr[nowIndex].link.size(); i++)
    {
        cout << " the link value:" << nodeArr[nowIndex].link[i].first << "  next jump:" << nodeArr[nowIndex].link[i].second << endl;
    }
    return 0;
}
int findminmax_(int nowIndex)//函数作为可选的参数,进行
{
    if (nowIndex < minmax_.first)minmax_.first = nowIndex;
    if (nowIndex > minmax_.second)minmax_.second = nowIndex;
    return 0;
}
void bfs(int fromNodeIdx, func functionPtr)
{
    set<int> isVisited;
    queue<int> que;
    int p = -1;
    que.push(fromNodeIdx);
    while (!que.empty())
    {
        p = que.front(); que.pop();
        if (isVisited.count(p) >= 1) {
            p = -1;
            continue;
        }
        isVisited.insert(p);
        (*functionPtr)(p);
        for (int i = 0; i < nodeArr[p].link.size(); i++)
        {
            que.push(nodeArr[p].link[i].second);
        }
    }
}
int copyTree(int fromNodeIdx)//将链表内的一串节点复制到新的下标,返回新生成的tree
{
    bfs(fromNodeIdx, &findminmax_);//更新minmax_的值,
    cout << minmax_.first << " " << minmax_.second << endl;
    int bias = nowNodeNum - minmax_.first;//得到当前节点的和复制起点的偏移量
    for (int i = minmax_.first; i <= minmax_.second; i++)//所有的顺序节点
    {
        nodeArr[nowNodeNum] = nodeArr[i];
        for (int j = 0; j < nodeArr[nowNodeNum].link.size(); j++)//修改该节点内所有的link
        {
            nodeArr[nowNodeNum].link[j].second += bias;
        }
        nowNodeNum++;
    }

    return bias;
}



//---------------------------------------------------------------
typedef class _tree {
public:
    int front;
    int end;
    _tree(int f, int e) :front(f), end(e) {}



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
    case '.':return 6;
    case '|':return 4;
    case '[':return 2;
    case '(':return 1;
    default:return 0;
    }
}
int icp(char a)//栈外优先级别,优先级是针对双目运算符的,单目运算符是直接执行的
{
    switch (a)
    {
    case '.':return 5;
    case '|':return 3;
    case '[':return 6;
    case '(':return 7;
    default:return 0;
    }
}

void operateBysym_3(char sym)
{
    if (sym == '\0')
    {

    }
    else if (sym == '-')
    {

    }
    else if (sym == '^')
    {

    }
}

void operateBysym_2(char sym)
{
    tree opetree1, opetree2;
    if (treeStack.empty()) {
        cout << "treeStack is empty,check out operateBysym_2 ,first treenode" << endl;
        return;
    }
    opetree1 = treeStack.top(); treeStack.pop();
    if (treeStack.empty()) {
        cout << "treeStack is empty,check out operateBysym_2 ,second treenode" << endl;
        return;
    }
    opetree2 = treeStack.top(); treeStack.pop();
    if (sym == '|')
    {


        nodeArr[nowNodeNum].pushLink("", opetree1->front);//新头连上两个头
        nodeArr[nowNodeNum].pushLink("", opetree2->front);
        nodeArr[opetree1->end].pushLink("", nowNodeNum + 1);//两个旧尾连上新尾
        nodeArr[opetree2->end].pushLink("", nowNodeNum + 1);
        opetree1->front = nowNodeNum; opetree1->end = nowNodeNum + 1;
        delete(opetree2);
        nowNodeNum += 2;

    }
    else if (sym == '.')
    {
        //tree2指针后出,具有更高的优先级
        nodeArr[opetree2->end].pushLink("", opetree1->front);
        opetree1->front = opetree2->front;
        delete(opetree2);
    }
    treeStack.push(opetree1);

}

void operateBysym_1(char sym)
{
    tree operateNode = NULL;
    if (!treeStack.empty()) {
        operateNode = treeStack.top(); treeStack.pop();//把变量节点取出
    }
    else {
        cout << "treeStack is empty, check out operateBysym_1";
    }

    if (sym == '?')
    {
        nodeArr[nowNodeNum].pushLink("", operateNode->front);//将原有的头连接新头
        operateNode->front = nowNodeNum;//更新头节点
        nodeArr[operateNode->end].pushLink("", nowNodeNum + 1);//将原有的尾连接新尾
        operateNode->end = nowNodeNum + 1;//更新尾节点
        nodeArr[nowNodeNum].pushLink("", operateNode->end);//将头节点直接连到新尾节点
        nowNodeNum += 2;//将节点数置为标记过的节点下标
    }
    else if (sym == '*')
    {
        nodeArr[operateNode->end].pushLink("", operateNode->front);//将原有的尾连接尾
        nodeArr[nowNodeNum].pushLink("", operateNode->front);//将新的头连接原有的头
        operateNode->front = nowNodeNum;//更新头节点
        nodeArr[operateNode->end].pushLink("", nowNodeNum + 1);//将原有的尾连接新尾
        operateNode->end = nowNodeNum + 1;//更新尾节点
        nodeArr[nowNodeNum].pushLink("", operateNode->end);//将头节点直接连到新尾节点
        nowNodeNum += 2;//将节点数置为标记过的节点下标
    }
    else if (sym == '+')
    {
        findminmax_(operateNode->front);//找到该tree内节点的最大最小下标
        int bias = copyTree(operateNode->front);
        int newfront = operateNode->front + bias;
        int newend = operateNode->end + bias;

        nodeArr[operateNode->end].pushLink("", operateNode->front);//将原有的尾连接头
        nodeArr[nowNodeNum].pushLink("", operateNode->front);//将新的头连接原有的头
        nodeArr[operateNode->end].pushLink("", nowNodeNum + 1);//将原有的尾连接新尾
        operateNode->end = nowNodeNum + 1;//更新尾节点
        nodeArr[nowNodeNum].pushLink("", operateNode->end);//将头节点直接连到新尾节点
        nodeArr[newend].pushLink("", nowNodeNum);//将复制的新节点的尾连接上现在的头节点
        operateNode->front = newfront;//更新头节点
        nowNodeNum += 2;//将节点数置为标记过的节点下标

    }
    treeStack.push(operateNode);
}


vector<string> wordtable;
tree pushvstrs()//扫描vstrs 向量内的数据进行操作
{
    nowNodeNum = 0;//初始节点数量
    char tempChar, symStackTopchar;
    int lenVstrs = vstrs.size();
    for (int i = 0; i < lenVstrs; i++)
    {
        if (vstrs[i].second == '1')//是变量->生成一个记录两个节点的tree,放进变量栈内
        {
            if (std::find(wordtable.begin(), wordtable.end(), vstrs[i].first) == wordtable.end())
                wordtable.push_back(vstrs[i].first);
            nodeArr[nowNodeNum].pushLink(vstrs[i].first, nowNodeNum + 1);//和下一个节点连接起来
            tree mytree = new _tree(nowNodeNum, nowNodeNum + 1);//将头和尾连接起来
            nowNodeNum += 2;
            treeStack.push(mytree);
        }
        else {//如果是其他符号
            tempChar = vstrs[i].first[0];//得到符号内容
            if (!symStack.empty())symStackTopchar = symStack.top();//得到符号栈顶的内容
            else cout << "symstack is empty, check on function 'pushvstrs' " << endl;
            if (tempChar == '*' || tempChar == '+' || tempChar == '?')//如果是单目运算符,直接对变量栈内的元素进行操作
            {
                operateBysym_1(tempChar);
                continue;
            }
            else if (tempChar == '^' || tempChar == '-')//如果是范围运算符
            {
                symStack.push(tempChar);//将当前的范围限定符号入栈
                continue;
            }
            else if (icp(tempChar) > isp(symStackTopchar) && tempChar != ')' && tempChar != ']')//如果是双目运算符
            {
                symStack.push(tempChar);//将当前的符号入栈
                continue;
            }
            else if (icp(tempChar) < isp(symStackTopchar) && tempChar != ')' && tempChar != ']')
            {
                char aa = symStack.top();
                while (icp(tempChar) < isp(aa))//isp为栈内,如果小于栈内的top符号的优先级
                {
                    symStack.pop();//如果比当前栈顶元素优先级小,那么才pop出来
                    operateBysym_2(aa);//先对栈内元素进行操作
                    aa = symStack.top();//取下一个栈内元素,直到出现栈顶优先级小于的现在符号的
                }
                symStack.push(tempChar);//把当前的符号push进符号栈
                continue;
            }
            else if (tempChar == ')')//如果是),那么直到(才结束
            {
                char aa = symStack.top(); //取栈顶
                while (aa != '(')
                {
                    symStack.pop();//不是(,pop出来
                    operateBysym_2(aa);//操作
                    aa = symStack.top();//取下一个,直到出现(
                }
                symStack.pop();//把(也pop出来
            }
            else if (tempChar == ']')//如果是],那么直到[才结束
            {
                char aa = symStack.top(); symStack.pop();
                int hasSymInIt = 0;//设置有无符号在两个方框内
                while (aa != '[')
                {
                    hasSymInIt = 1;
                    operateBysym_3(aa);
                    aa = symStack.top(); symStack.pop();
                }
                if (hasSymInIt == 0)//没有符号内方括号内
                {
                    operateBysym_3('\0');
                }
                symStack.pop();
            }
        }
    }
    tree p1, p;
    p = treeStack.top();
    while (!treeStack.empty())
    {
        p = treeStack.top(); treeStack.pop();
        if (treeStack.empty()) {
            break;

        }
        p1 = treeStack.top(); treeStack.pop();

        nodeArr[p1->end].pushLink("", p->front);
        p->front = p1->front;
        delete(p1);
        treeStack.push(p);
    }
    return p;
}
//-----------------------------------------------------------------------------------------



//--------------------------------------dfa and mini------------------------------------------------
typedef class _nodeDFA :public _node
{
public:
    int isTheEnd;
    set<int> numList;//节点的下标集合
    void pushNode(int index)
    {
        numList.insert(index);
    }

    virtual void deleteNode()
    {
        isTheEnd=0;
        link.clear();
        numList.clear();
    }
}*nodeDFA;
_nodeDFA nodeDFAArr[100];//存放节点
int nowNodeDFANum;
set<int> firstDFA;
tree frontend;
string definetable[100];//存储
vector<pair<int, vector<int>>> firstDFA_;//DFA_idx,first

void bfs_(vector<int> fromNodeIdx, int fromDFANodeIdex)
{
    pair<string, int> tmpLink;
    set<int> isVisited;
    queue<int> que;
    int p;
    vector<pair<string, int>>linkset;

    for (int i = 0; i < fromNodeIdx.size(); i++)que.push(fromNodeIdx[i]);
    
    while (!que.empty())
    {
        p = que.front(); que.pop();
        if (p == frontend->end) 
            nodeDFAArr[fromDFANodeIdex].isTheEnd = 1;
        if (isVisited.count(p) >= 1)//假如nfa图中已经访问过 
        {
            continue;//不进新遍历
        }
        nodeDFAArr[fromDFANodeIdex].numList.insert(p);//在当前dfa下插入nfa节点值
        isVisited.insert(p);//设置当前nfa节点已经访问
        for (int i = 0; i < nodeArr[p].link.size(); i++)
        {
            tmpLink = nodeArr[p].link[i];
            if (tmpLink.second == frontend->end && tmpLink.first=="")//如果已经到达尾节点
            {
                nodeDFAArr[fromDFANodeIdex].isTheEnd = 1;
            }
            
            if (tmpLink.first != "")//如果不为空
            {
                linkset.push_back(tmpLink);
            }
            else//如果是空
            {
                que.push(tmpLink.second);
            }
        }
    }

    //得到所有的有价值的路径
    vector<pair<string, vector<int>>> nextjump;
    int flat = 0;
    for (int i = 0; i < linkset.size(); i++)//遍历路径
    {
        flat = 0;
        for (int j = 0; j < nextjump.size(); j++)//如果在表中已经有该路径的信息
        {
            if (linkset[i].first == nextjump[j].first)
            {
                nextjump[j].second.push_back(linkset[i].second);//直接插入现在的吓一跳节点
                flat = 1;
                break;
            }
        }
        if (flat == 1)continue;
        pair<string, vector<int>> tempjump;
        tempjump.first = linkset[i].first;//没有开始创建
        tempjump.second.push_back(linkset[i].second);
        nextjump.push_back(tempjump);
    }
    for (int j = 0; j < nextjump.size(); j++)
    {
        int flat = 0;
        for (int len = 0; len < firstDFA_.size(); len++)
        {
            if (firstDFA_[len].second == nextjump[j].second)
            {
                nodeDFAArr[fromDFANodeIdex].pushLink(nextjump[j].first, firstDFA_[len].first);
                flat = 1;
                break;
            }
        }
        if (flat == 1)continue;
        nodeDFAArr[fromDFANodeIdex].pushLink(nextjump[j].first, nowNodeDFANum);
        pair<int, vector<int>> tmpp(nowNodeDFANum, nextjump[j].second);
        firstDFA_.push_back(tmpp);

        bfs_(nextjump[j].second, nowNodeDFANum++);

    }
}


void bfs_read(int fromDFANodeIdx)
{
    set<int> isVisited;
    queue<int> que;
    int p = -1;
    que.push(fromDFANodeIdx);
    while (!que.empty())
    {
        p = que.front(); que.pop();
        if (isVisited.count(p) >= 1) {
            continue;
        }
        isVisited.insert(p);
        int k;
        set<int>::reverse_iterator it1 = nodeDFAArr[p].numList.rbegin();
        cout << "\n第" << p << "个dfa  " << nodeDFAArr[p].isTheEnd << endl;
        while (it1 != nodeDFAArr[p].numList.rend())
        {
            cout << *it1 << " ";
            ++it1;
        }
        cout << endl;
        cout << "\n";
        for (int i = 0; i < nodeDFAArr[p].link.size(); i++)
        {
            cout << "从" << p << "的 值" << nodeDFAArr[p].link[i].first << "下一跳" << nodeDFAArr[p].link[i].second << endl;
        }
        for (int i = 0; i < nodeDFAArr[p].link.size(); i++)
        {
            que.push(nodeDFAArr[p].link[i].second);
        }
    }
}

int type[100];//记录每个节点的所属集合
int checktype(int idx, string linkvalue)
{
    int pointed = -1;
    for (int i = 0; i < nodeDFAArr[idx].link.size(); i++)
    {
        if (linkvalue == nodeDFAArr[idx].link[i].first)
        {
            return type[nodeDFAArr[idx].link[i].second];
        }
    }
    return pointed;
}

class ftd {
public:
    int from;
    int to;
    set<int> numlist;

    ftd(int a, int b, int c)
    {
        from = a;
        to = b;
        numlist.insert(c);
    }
    bool operator==(const ftd &ant)
    {
        return this->from == ant.from && this->to == ant.to;
    }
};

_nodeDFA mindfa[100];
int nowmindfanum =1;
void minimiaz()
{
    int maxtype = 1;//记录最大的集合个数
    for (int i = 0; i < nowNodeDFANum; i++) {
        if (nodeDFAArr[i].isTheEnd == 1)type[i] = 1;
        else type[i] = 2;
    }
    int done = 0;//记录是否完成
    vector<ftd> templink;
    int max = 1;
    while (done == 0)//如果未完成
    {
        
        for (int k = 0; k < wordtable.size(); k++)
        {
            string tstr = wordtable[k];
            for (int i = 0; i < nowNodeDFANum; i++)//遍历所有节点
            {
                int type_idx = checktype(i, wordtable[k]);//返回指向的dfa节点状态
                ftd tmpftd(type[i], type_idx, i);//建立新的
                int flat = 0;
                for (int tm = 0; tm < templink.size(); tm++)
                {
                    if (templink[tm] == tmpftd)
                    {
                        flat = 1;
                        templink[tm].numlist.insert(i);
                        break;
                    }
                }
                if (flat == 0)templink.push_back(tmpftd);//如果没出现过该内容,新建一个这样的节点
            }

            //已经得到两个集合的各个方向的连接,选取第一个连接分开,其他的连接都原地不动
            set<int> haslink;
            for (int idx = 0; idx < templink.size(); idx++)
            {
                if (haslink.count(templink[idx].from) < 1)
                {
                    haslink.insert(templink[idx].from);//这个集合已经访问过,字
                    set<int>::reverse_iterator it1 = templink[idx].numlist.rbegin();
                    while (it1 != templink[idx].numlist.rend())
                    {
                        type[*it1] = type[*it1] * 2 + 2;
                        ++it1;
                    }
                }
                else {
                    set<int>::reverse_iterator it1 = templink[idx].numlist.rbegin();
                    while (it1 != templink[idx].numlist.rend())
                    {
                        type[*it1] = type[*it1] * 2 + 1;
                        ++it1;
                    }
                }
            }
            if (max != haslink.size())
            {
                max = haslink.size();
            }
            else {
                done = 1;
                for (int idx = 0; idx < nowNodeDFANum; idx++)cout << type[idx] << " ";
                break;
            }
            haslink.clear();
            templink.clear();
            cout << "\n";
            for (int idx = 0; idx < nowNodeDFANum; idx++)cout << type[idx] << " ";
        }
    }
    set<int> hadcreate;
    int information[100];
    for (int i = 0; i < nowNodeDFANum; i++)//遍历所有的dfa节点
    {
        if (hadcreate.count(type[i]) < 1)//如果未被访问
        {
            mindfa[nowmindfanum].numList.insert(i);//插入
            hadcreate.insert(type[i]);
            information[nowmindfanum] = type[i];
            if (nodeDFAArr[i].isTheEnd == 1)mindfa[nowmindfanum].isTheEnd = 1;
            type[i] = nowmindfanum;
            nowmindfanum++;
        }
        else {
            for (int j = 0; j < nowmindfanum; j++)
            {
                if (information[j] == type[i])
                {
                    mindfa[j].numList.insert(i);//插入现在的i
                    type[i] = j;
                }
            }
        }
    }
    for (int idx = 0; idx < nowNodeDFANum; idx++)
    {
        cout << type[idx] << " ";
        
    }
    cout << "\n" << "MINDFA NODE NUM" << nowmindfanum << endl;
    for (int idx = 0; idx < nowmindfanum; idx++)
    {
        set<string> haslinkword;
        set<int>::reverse_iterator it1 = mindfa[idx].numList.rbegin();
        while (it1 != mindfa[idx].numList.rend())
        {
            cout << *it1 << " " ;
            for(int n=0;n<nodeDFAArr[*it1].link.size();n++)
                if (haslinkword.count(nodeDFAArr[*it1].link[n].first) < 1)
                {
                    haslinkword.insert(nodeDFAArr[*it1].link[n].first);
                    mindfa[idx].pushLink(nodeDFAArr[*it1].link[n].first, type[nodeDFAArr[*it1].link[n].second]);
                }
            ++it1;
        }
        cout << "\n";
    }
    for (int idx = 0; idx < nowmindfanum; idx++)
    {
        cout << "now num" << idx<<mindfa[idx].isTheEnd<< "\n";
        for (int n = 0; n < mindfa[idx].link.size(); n++)
        {
            cout << mindfa[idx].link[n].first << "　　" << mindfa[idx].link[n].second << endl;
        }
        cout << "\n";
    }
}

string number(int i)
{
    string goal="";
    temp=1;
    while(i/temp>=1)temp*=10; 
    temp/=10;
    while(temp>0)
    {
        goal+=(char)('0'+i/temp);
        temp/=10;
    }
    return goal;
}

string codeCreater()
{
    int tmp;
    string goal = "#include<iostream>\nusing namespace std;\nint main(){\n int state=1;\n string goal=\"\" ;\nchar temp;\nwhile(";
    
    for (int i = 1; i < nowmindfanum; i++)
    {
        if (i > 1)goal = goal + " || ";
        goal =goal+ "state=="+ number(i);
    }

    goal += "){cin>>temp;\ngoal=goal+temp;\nswitch(state){\n";
    for (int i = 1; i < nowmindfanum; i++)
    {
        goal = goal + "case "
        goal+=number(i)
        goal +=  ":\n";
        int type = 0;
        for (int j = 0; j < mindfa[i].link.size(); j++)
        {
            type = 1;
            if (j > 0) {
                goal = goal + "else ";
            }
            goal = goal + "if (type(temp)==" + mindfa[i].link[j].first + "){\nstate=" + number(mindfa[i].link[j].second)+";\n}\n";
        }
        if(mindfa[i].isTheEnd==1)
        {
            if(type==0){goal = goal + "\n state="+number(nowmindfanum)+";\n";}
            else { goal = goal + "else{\n state=" + number(nowmindfanum) + ";}\n"; }
        }
        goal = goal + "break;\n";
    }
    goal = goal + "}\\\\end of switch\n}\\\\end of while\ncout<<goal<<endl;\nreturn 0;\n}";
     
       return goal;


}
//--------------------------------------------------------------------------------------------------



int main() {


    string regularExpressions = "digtal(char|digtal)*";//正则表达式
    //string regularExpressions = "digtal*";
    string regularExpressions2 = "(digtal.digtal|char.char)*(digtal|char)(digtal.digtal|char.char)*";
    string regularExpressions1 = "(a|b)*(b|c)*a.b";
    //cin>>regularExpressions;
    init_table();
    symStack.push('#');
    scanner(regularExpressions1);
    int len_vstrs = vstrs.size();
    for (int i = 0; i < len_vstrs; i++)
    {
        cout << vstrs[i].first << " " << vstrs[i].second << endl;
    }

    frontend = pushvstrs();
    bfs(frontend->front, visit);
    nowNodeDFANum = 1;
    //bfs_nfa2dfa(frontend->front,0);
    vector<int> ok;
    ok.push_back(frontend->front);
    bfs_(ok, 0);
    bfs_read(0);
    minimiaz();
    cout << codeCreater() << endl;
    system("pause");
}