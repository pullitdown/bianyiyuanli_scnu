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

//---------------------������ʽ�ķִʲ���------------------------------
//�ִʹ����
//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5 �ַ�:6 ע��:7
char syntaxTable_RegularExpressions[100][6] =
{
    "0((10",//�յ�(�ļ��
    "01110", //�յ���ĸ�ļ��
    "0_110", //��ʼ��_�ļ��,����ĸ��Ϊ����
    "11100", //������������,���Գн�_,����
    "1_100", //��������_������
    "13100", //����������������
    "10010", //���������ո�ļ��
    "1((10", //��������(�ļ��
    "1++10",//��������+�ļ��
    "+1110",//+���������ļ��
    "+((10",//+��(�ļ��
    "1**10",//��������*�ļ��
    "*((10",//*��(�ļ��
    "*1110",//*���������ļ��
    "1..10",
    ".1110",
    ".((10",
    ")((10",
    "(1110", //(����ĸ�ļ��
    "1||10", //��ĸ��|�ļ��
    "1))10", //��ĸ��)�ļ��
    "|1110", //|����ĸ�ļ��
    ")**10", //)��*�ļ��
    ")..10",//)��.�ļ��
    "*..10",//*��.�ļ��
};
int len_check_table = sizeof(syntaxTable_RegularExpressions) / sizeof(syntaxTable_RegularExpressions[0]);
int nextTypeIndex0[200];//������¼ ĳ�������ڴʷ������״γ��ֵ��±�
vector<pair<string, char>> vstrs;
int compar_words(const void* p, const void* q)
{
    return strcmp((char*)p, (char*)q);
}
void init_table(int file_type = 0)//���﷨������������,���һ�ò�ͬ�﷨����ĳнӹ�ϵ,�Ա���������ʱ���и�����ٶ�
{
    int* nextTypeIndex = nextTypeIndex0;

    char(*all_check_table)[6] = syntaxTable_RegularExpressions;
    int lenOfCheckTable = -1;//��ȡ�ʷ���ĳ���
    while (all_check_table[++lenOfCheckTable][0] != '\0');
    qsort(all_check_table, lenOfCheckTable, 6, compar_words);//�Դʷ����������
    //memset(nextTypeIndex,9999,200*8);//���ó�ֵΪ300
    for (int i = 0; i < 200; i++) { nextTypeIndex[i] = 300; }

    int temp;
    for (int i = 0; i < lenOfCheckTable; i++)//�����дʷ����б���
    {
        temp = nextTypeIndex[(int)all_check_table[i][0]];//��ȡĳ�������ڴʷ��е�λ��
        if (temp > i)//������ڵ��±��ԭ���±�ҪС
            nextTypeIndex[all_check_table[i][0]] = i;//�״γ��ֵ�λ�ø�Ϊi
    }

    for (int i = 0; i < lenOfCheckTable; i++)
        all_check_table[i][4] = nextTypeIndex[all_check_table[i][2]];
    //for (int i = 0; i < 200; i++)if (nextTypeIndex[i] != 300)cout << nextTypeIndex[i] << " " << nextTypeIndex0[i] << " " << nextTypeIndex1[i] << endl;
}
void scanner(string str, int file_type = 0) //ɨ��ÿһ��,file_type������ɨ��c++�ļ�(file_type=0) ���� �Զ��������ļ�(file_type=1)
{
    int* nextTypeIndex = nextTypeIndex0;
    unsigned int last_start = 0;//��¼���ε�ƥ���﷨����,��нӵ���һ�ι�����ʼ��
    char(*all_check_table)[6] = syntaxTable_RegularExpressions;
    int len_all_check_table = len_check_table;
    char now_char = str[0];         //��ǰָ����ַ�
    int lenofeachline = str.size(); //�����еĳ���
    int index = 0;                  //��ʾ��ǰָ����ַ��±�
    string n = "";                  //���õ���token
    char now_type[6] = "0000\0";      //��ʼ����ǰ�ַ�״̬
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
        if (now_char >= 48 && now_char <= 57) //���������
        {
            now_type[1] = '3';
        }
        else if ((now_char >= 65 && now_char <= 90) || (now_char >= 97 && now_char <= 122)) //�������ĸ
        {
            now_type[1] = '1';
        }
        else //����Ƿ���
        {
            if (now_char == ' ')
            {
                if (now_type[0] != 's') //��������ַ�����
                {
                    if (n != "" && now_type[0] != '0') //�����ʱ��n���浽����,������һ�����ݲ�Ϊ0
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
                else //������ַ�����,�ո���Ϊ����ĸ
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
        for (int i = last_start; i < len_all_check_table; i++) //ƥ������
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
    if (file_type == 1 && n[0] != '\0') //��Ϊc++��ÿһ�������һ�������� ;,},>��,��pycpp�����в�һ����,������һ�е�ĩβ�ж�,�Ƿ���û������ű�ķ���
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
    vector<pair<string, int>> link;//�ڵ��·��,���intΪ-1,��Ϊ��ֹ״̬
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
_node nodeArr[200];//��Žڵ�
int nowNodeNum;//��¼��ǰ


void deletearr(node a) {
    a->deleteNode();
}

//----------------------------------------------------------------
typedef int (*func)(int);//������func����ָ��
pair<int, int> minmax_;
int visit(int nowIndex)
{
    cout << "���ڷ��ʵĽڵ���" << nowIndex << "   \n����·����Ϣ��" << endl;
    for (int i = 0; i < nodeArr[nowIndex].link.size(); i++)
    {
        cout << " the link value:" << nodeArr[nowIndex].link[i].first << "  next jump:" << nodeArr[nowIndex].link[i].second << endl;
    }
    return 0;
}
int findminmax_(int nowIndex)//������Ϊ��ѡ�Ĳ���,����
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
int copyTree(int fromNodeIdx)//�������ڵ�һ���ڵ㸴�Ƶ��µ��±�,���������ɵ�tree
{
    bfs(fromNodeIdx, &findminmax_);//����minmax_��ֵ,
    cout << minmax_.first << " " << minmax_.second << endl;
    int bias = nowNodeNum - minmax_.first;//�õ���ǰ�ڵ�ĺ͸�������ƫ����
    for (int i = minmax_.first; i <= minmax_.second; i++)//���е�˳��ڵ�
    {
        nodeArr[nowNodeNum] = nodeArr[i];
        for (int j = 0; j < nodeArr[nowNodeNum].link.size(); j++)//�޸ĸýڵ������е�link
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



}*tree;//�ȿ��Ա�ʾnfa��,Ҳ���Ա�ʾdfa��

stack<char> symStack;//����ջ
stack<tree> treeStack;//�ڵ�ջ
//�������ȼ���ִ�й���
//���ջ��ķ������ȼ�С��ջ�����ȼ�,�ӷ���ջ�ó�һ������,
//�ӱ���ջ�ó���������,������Ӧ����,
//���ջ��ķ������ȼ�����ջ�����ȼ�,������Ѹ÷��ŷ������ջ
int isp(char a)//ջ�����ȼ���
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
int icp(char a)//ջ�����ȼ���,���ȼ������˫Ŀ�������,��Ŀ�������ֱ��ִ�е�
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


        nodeArr[nowNodeNum].pushLink("", opetree1->front);//��ͷ��������ͷ
        nodeArr[nowNodeNum].pushLink("", opetree2->front);
        nodeArr[opetree1->end].pushLink("", nowNodeNum + 1);//������β������β
        nodeArr[opetree2->end].pushLink("", nowNodeNum + 1);
        opetree1->front = nowNodeNum; opetree1->end = nowNodeNum + 1;
        delete(opetree2);
        nowNodeNum += 2;

    }
    else if (sym == '.')
    {
        //tree2ָ����,���и��ߵ����ȼ�
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
        operateNode = treeStack.top(); treeStack.pop();//�ѱ����ڵ�ȡ��
    }
    else {
        cout << "treeStack is empty, check out operateBysym_1";
    }

    if (sym == '?')
    {
        nodeArr[nowNodeNum].pushLink("", operateNode->front);//��ԭ�е�ͷ������ͷ
        operateNode->front = nowNodeNum;//����ͷ�ڵ�
        nodeArr[operateNode->end].pushLink("", nowNodeNum + 1);//��ԭ�е�β������β
        operateNode->end = nowNodeNum + 1;//����β�ڵ�
        nodeArr[nowNodeNum].pushLink("", operateNode->end);//��ͷ�ڵ�ֱ��������β�ڵ�
        nowNodeNum += 2;//���ڵ�����Ϊ��ǹ��Ľڵ��±�
    }
    else if (sym == '*')
    {
        nodeArr[operateNode->end].pushLink("", operateNode->front);//��ԭ�е�β����β
        nodeArr[nowNodeNum].pushLink("", operateNode->front);//���µ�ͷ����ԭ�е�ͷ
        operateNode->front = nowNodeNum;//����ͷ�ڵ�
        nodeArr[operateNode->end].pushLink("", nowNodeNum + 1);//��ԭ�е�β������β
        operateNode->end = nowNodeNum + 1;//����β�ڵ�
        nodeArr[nowNodeNum].pushLink("", operateNode->end);//��ͷ�ڵ�ֱ��������β�ڵ�
        nowNodeNum += 2;//���ڵ�����Ϊ��ǹ��Ľڵ��±�
    }
    else if (sym == '+')
    {
        findminmax_(operateNode->front);//�ҵ���tree�ڽڵ�������С�±�
        int bias = copyTree(operateNode->front);
        int newfront = operateNode->front + bias;
        int newend = operateNode->end + bias;

        nodeArr[operateNode->end].pushLink("", operateNode->front);//��ԭ�е�β����ͷ
        nodeArr[nowNodeNum].pushLink("", operateNode->front);//���µ�ͷ����ԭ�е�ͷ
        nodeArr[operateNode->end].pushLink("", nowNodeNum + 1);//��ԭ�е�β������β
        operateNode->end = nowNodeNum + 1;//����β�ڵ�
        nodeArr[nowNodeNum].pushLink("", operateNode->end);//��ͷ�ڵ�ֱ��������β�ڵ�
        nodeArr[newend].pushLink("", nowNodeNum);//�����Ƶ��½ڵ��β���������ڵ�ͷ�ڵ�
        operateNode->front = newfront;//����ͷ�ڵ�
        nowNodeNum += 2;//���ڵ�����Ϊ��ǹ��Ľڵ��±�

    }
    treeStack.push(operateNode);
}


vector<string> wordtable;
tree pushvstrs()//ɨ��vstrs �����ڵ����ݽ��в���
{
    nowNodeNum = 0;//��ʼ�ڵ�����
    char tempChar, symStackTopchar;
    int lenVstrs = vstrs.size();
    for (int i = 0; i < lenVstrs; i++)
    {
        if (vstrs[i].second == '1')//�Ǳ���->����һ����¼�����ڵ��tree,�Ž�����ջ��
        {
            if ( std::find(wordtable.begin(), wordtable.end(), vstrs[i].first) == wordtable.end() )
                wordtable.push_back(vstrs[i].first);
            nodeArr[nowNodeNum].pushLink(vstrs[i].first, nowNodeNum + 1);//����һ���ڵ���������
            tree mytree = new _tree(nowNodeNum, nowNodeNum + 1);//��ͷ��β��������
            nowNodeNum += 2;
            treeStack.push(mytree);
        }
        else {//�������������
            tempChar = vstrs[i].first[0];//�õ���������
            if (!symStack.empty())symStackTopchar = symStack.top();//�õ�����ջ��������
            else cout << "symstack is empty, check on function 'pushvstrs' " << endl;
            if (tempChar == '*' || tempChar == '+' || tempChar == '?')//����ǵ�Ŀ�����,ֱ�ӶԱ���ջ�ڵ�Ԫ�ؽ��в���
            {
                operateBysym_1(tempChar);
                continue;
            }
            else if (tempChar == '^' || tempChar == '-')//����Ƿ�Χ�����
            {
                symStack.push(tempChar);//����ǰ�ķ�Χ�޶�������ջ
                continue;
            }
            else if (icp(tempChar) > isp(symStackTopchar) && tempChar != ')' && tempChar != ']')//�����˫Ŀ�����
            {
                symStack.push(tempChar);//����ǰ�ķ�����ջ
                continue;
            }
            else if (icp(tempChar) < isp(symStackTopchar) && tempChar != ')' && tempChar != ']')
            {
                char aa = symStack.top();
                while (icp(tempChar) < isp(aa))//ispΪջ��,���С��ջ�ڵ�top���ŵ����ȼ�
                {
                    symStack.pop();//����ȵ�ǰջ��Ԫ�����ȼ�С,��ô��pop����
                    operateBysym_2(aa);//�ȶ�ջ��Ԫ�ؽ��в���
                    aa = symStack.top();//ȡ��һ��ջ��Ԫ��,ֱ������ջ�����ȼ�С�ڵ����ڷ��ŵ�
                }
                symStack.push(tempChar);//�ѵ�ǰ�ķ���push������ջ
                continue;
            }
            else if (tempChar == ')')//�����),��ôֱ��(�Ž���
            {
                char aa = symStack.top(); //ȡջ��
                while (aa != '(')
                {
                    symStack.pop();//����(,pop����
                    operateBysym_2(aa);//����
                    aa = symStack.top();//ȡ��һ��,ֱ������(
                }
                symStack.pop();//��(Ҳpop����
            }
            else if (tempChar == ']')//�����],��ôֱ��[�Ž���
            {
                char aa = symStack.top(); symStack.pop();
                int hasSymInIt = 0;//�������޷���������������
                while (aa != '[')
                {
                    hasSymInIt = 1;
                    operateBysym_3(aa);
                    aa = symStack.top(); symStack.pop();
                }
                if (hasSymInIt == 0)//û�з����ڷ�������
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
    
    set<int> numList;//�ڵ���±꼯��
    void pushNode(int index)
    {
        numList.insert(index);
    }

    virtual void deleteNode()
    {
        link.clear();
        numList.clear();
    }
}*nodeDFA;
_nodeDFA nodeDFAArr[100];//��Žڵ�
int nowNodeDFANum;
set<int> firstDFA;
tree frontend;
string definetable[100];//�洢
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
        if (isVisited.count(p) >= 1)//����nfaͼ���Ѿ����ʹ� 
        {
            continue;//�����±���
        }
        nodeDFAArr[fromDFANodeIdex].numList.insert(p);//�ڵ�ǰdfa�²���nfa�ڵ�ֵ
        isVisited.insert(p);//���õ�ǰnfa�ڵ��Ѿ�����
        for (int i = 0; i < nodeArr[p].link.size(); i++)
        {
            tmpLink = nodeArr[p].link[i];
            if (tmpLink.second == frontend->end)//����Ѿ�����β�ڵ�
            {
                nodeDFAArr[fromDFANodeIdex].isTheEnd = 1;
            }

            if (tmpLink.first != "")//�����Ϊ��
            {
                linkset.push_back(tmpLink);
            }
            else//����ǿ�
            {
                que.push(tmpLink.second);
            }
        }
    }

    //�õ����е��м�ֵ��·��
    vector<pair<string, vector<int>>> nextjump;
    int flat = 0;
    for (int i = 0; i < linkset.size(); i++)//����·��
    {
        flat = 0;
        for (int j = 0; j < nextjump.size(); j++)//����ڱ����Ѿ��и�·������Ϣ
        {
            if (linkset[i].first == nextjump[j].first)
            {
                nextjump[j].second.push_back(linkset[i].second);//ֱ�Ӳ������ڵ���һ���ڵ�
                flat = 1;
                break;
            }
        }
        if (flat == 1)continue;
        pair<string, vector<int>> tempjump;
        tempjump.first = linkset[i].first;//û�п�ʼ����
        tempjump.second.push_back(linkset[i].second);
        nextjump.push_back(tempjump);
    }
    for (int j = 0; j < nextjump.size(); j++)
    {
        int flat = 0;
        for (int len = 0; len < firstDFA_.size(); len++)
        {
            if ( firstDFA_[len].second == nextjump[j].second)
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
        cout << "\n��" << p << "��dfa  " <<nodeDFAArr[p].isTheEnd << endl;
        while (it1 != nodeDFAArr[p].numList.rend())
        {
            cout << *it1 << " ";
            ++it1;
        }
        cout << endl;
        cout << "\n";
        for (int i = 0; i < nodeDFAArr[p].link.size(); i++)
        {
            cout << "��" << p << "�� ֵ" << nodeDFAArr[p].link[i].first << "��һ��" << nodeDFAArr[p].link[i].second << endl;
        }
        for (int i = 0; i < nodeDFAArr[p].link.size(); i++)
        {
            que.push(nodeDFAArr[p].link[i].second);
        }
    }
}

int type[100];//��¼ÿ���ڵ����������
int checktype(int idx,string linkvalue)
{
    int pointed=-1;
    for(int i=0;i<nodeDFAArr[idx].link.size();i++)
    {
        if(linkvalue==nodeDFAArr[idx].link[i].first)
        {
            return nodeDFAArr[idx].link[i].second;
        }
    }
    return pointed;
}

class ftd(){
    public:
    int from;
    int to;
    set<int> numlist;
    fld(int a,int b,int c){
        from=a;
        to=b;
        
        numlist.insert(c);
    }
    bool operator==(const ftd ant)
    {
        return self.from==ant.from && self.to==ant.to;
    }
}

void minimiaz()
{
    int maxtype=1;//��¼���ļ��ϸ���
    for(int i=0;i<nowNodeDFANum;i++){
        if(nodeDFAArr[i].isTheEnd==1)type[i]=1;
        else type[i]=2;
    }
    int done=0;//��¼�Ƿ����
    vector<ftd> templink;
    while(done==0)//���δ���
    {
        
        for(int k=0;k<wordtable.size();k++)
        {
            string tstr=wordtable[k];
            for(int i=0;i<nowNodeDFANum;i++)//�������нڵ�
            {
                int type_idx=checktype(i,wordtable[k]);//����ָ���dfa�ڵ�״̬
                ftd tmpftd(type[i],type_idx,i);//�����µ�
                int flat=0;
                for(int tm=0;tm<templink.size();tm++)
                {
                    if(templink[tm]==tmpftd)
                    {
                        flat=1;
                        templink[tm].numlist.insert(i);
                        break;
                    }
                }
                if(flat==0)templink.push_back(tmpftd);//���û���ֹ�������,�½�һ�������Ľڵ�
            }

            //�Ѿ��õ��������ϵĸ������������,ѡȡ��һ�����ӷֿ�,���������Ӷ�ԭ�ز���
            set<int> haslink;       
            for(int idx=0;idx<templink.size();idx++)
            {
                if(haslink.count(templink[idx].from)<1)
                {
                    haslink.insert(templink[idx].from);//��������Ѿ����ʹ�,��
                    set<int>::reverse_iterator it1 = templink[idx].numList.rbegin();
                    while (it1 != templink[idx].numList.rend())
                    {
                        type[*it1]=type[*it1]*2+2;
                        ++it1;
                    }
                }
                else{
                    set<int>::reverse_iterator it1 = templink[idx].numList.rbegin();
                    while (it1 != templink[idx].numList.rend())
                    {
                        type[*it1]=type[*it1]*2+1;
                        ++it1;
                    }
                }
                
            }

            haslink.clear();
            templink.clear();
            cout<<"\n";
            for(int idx=0;idx<nowNodeDFANum;idx++)cout<<type[idx]<<" ";
        }
        
    }
    
}


//--------------------------------------------------------------------------------------------------



int main() {
    string regularExpressions = "digtal(char|digtal)*";//������ʽ
    //string regularExpressions = "digtal*";
    string regularExpressions1 = "(digtal.digtal|char.char)*(digtal|char)(digtal.digtal|char.char)*";
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
    system("pause");
}