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

char syntaxTable_RegularExpressions[100][6]=
{
    "01110", //�յ���ĸ�ļ��
    "0_110", //��ʼ��_�ļ��,����ĸ��Ϊ����
    "11100", //������������,���Գн�_,����
    "1_100", //��������_������
    "13100", //����������������
    "10010", //���������ո�ļ��
    "1((10", //��������(�ļ��
    "(1110", //(����ĸ�ļ��
    "1||10", //��ĸ��|�ļ��
    "1))10", //��ĸ��)�ļ��
    "|1110", //|����ĸ�ļ��
    ")**10", //)��*�ļ��
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
    int* nextTypeIndex = nextTypeIndex0 ;

    char(*all_check_table)[6] = syntaxTable_RegularExpressions ;
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
    int len_all_check_table =  len_check_table ;
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

typedef class _node{
public:
    vector<pair<string,int>> link;//�ڵ��·��,���intΪ-1,��Ϊ��ֹ״̬
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
_node nodeArr[200];//��Žڵ�
int nowNodeNum;//��¼��ǰ

typedef class _nodeDFA :public _node
{
public:
    int isTheEnd;

    set<int> numList;//�ڵ���±꼯��
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
_nodeDFA nodeDFAArr[100];//��Žڵ�
int nowNodeDFANum;
void deletearr(node a){
    a->deleteNode();
}

typedef class _tree{
public:
    int front;
    int end;
    _tree(int f,int e):front(f),end(e){}
    void copyTree(int fromNodeIdx,int toNodeIdx)//�������ڵ�һ���ڵ㸴�Ƶ��µ��±�
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

	case '+':
	case '-':return 3;//һ����������ͬ���Ŷ�һ��,�����ջ�ڵķ��Ž��м���
	case '*':
	case '/':
	case '%':return 5;
	case '(':return 1;//��ջ��(Ӧ�þ���С

	case ')':return 6;//��ʵ���Բ������,��Ϊ')'������ջ,����ֱ��ִ�е���һ��
	default:return 0;
	}
}
int icp(char a)//ջ�����ȼ���,���ȼ������˫Ŀ�������,��Ŀ�������ֱ��ִ�е�
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
    tree operateNode=treeStack.top();treeStack.pop();//�ѽڵ�ȡ��
    if(sym=='?')
    {
        nodeArr[nowNodeNum].pushLink("",operateNode->front);//��ԭ�е�ͷ������ͷ
        operateNode->front=nowNodeNum;//����ͷ�ڵ�
        nodeArr[operateNode->end].pushLink("",nowNodeNum+1);//��ԭ�е�β������β
        operateNode->end=nowNodeNum+1;//����β�ڵ�
        nodeArr[nowNodeNum].pushLink("",operateNode->end);//��ͷ�ڵ�ֱ��������β�ڵ�
        nowNodeNum+=2;//���ڵ�����Ϊ��ǹ��Ľڵ��±�
    }
    if(sym=='*')
    {
        nodeArr[operateNode->end].pushLink("",operateNode->front);//��ԭ�е�β����β
        nodeArr[nowNodeNum].pushLink("",operateNode->front);//���µ�ͷ����ԭ�е�ͷ
        operateNode->front=nowNodeNum;//����ͷ�ڵ�
        nodeArr[operateNode->end].pushLink("",nowNodeNum+1);//��ԭ�е�β������β
        operateNode->end=nowNodeNum+1;//����β�ڵ�
        nodeArr[nowNodeNum].pushLink("",operateNode->end);//��ͷ�ڵ�ֱ��������β�ڵ�
        nowNodeNum+=2;//���ڵ�����Ϊ��ǹ��Ľڵ��±�
    }
    if(sym=='+')
    {
        
        nodeArr[operateNode->end].pushLink("",operateNode->front);//��ԭ�е�β����β
        nodeArr[nowNodeNum].pushLink("",operateNode->front);//���µ�ͷ����ԭ�е�ͷ
        operateNode->front=nowNodeNum;//����ͷ�ڵ�
        nodeArr[operateNode->end].pushLink("",nowNodeNum+1);//��ԭ�е�β������β
        operateNode->end=nowNodeNum+1;//����β�ڵ�
        nodeArr[nowNodeNum].pushLink("",operateNode->end);//��ͷ�ڵ�ֱ��������β�ڵ�
        nowNodeNum+=2;//���ڵ�����Ϊ��ǹ��Ľڵ��±�
        nodeArr[nowNodeNum]
    }
    treeStack.push(operateNode);
    
}

void pushvstrs()
{
    nowNodeNum=0;//��ʼ�ڵ�����
    char tempChar,symStackTopchar;
    int lenVstrs=vstrs.size();
    for(int i=0;i<lenVstrs;i++)
    {
        if(vstrs[i].second=='1')
        {
            nodeArr[nowNodeNum].pushLink(vstrs[i].first,nowNodeNum+1);//����һ���ڵ���������
            tree mytree=new _tree(nowNodeNum,++nowNodeNum);//��ͷ��β��������
            nowNodeNum++;
            treeStack.push(mytree);
        }
        else{
            tempChar=vstrs[i].first[0];
            symStackTopchar=symStack.top();
            if(tempChar=='*'||tempChar=='+'||tempChar=='?')//����ǵ�Ŀ�����
            {
                operateBysym_1(tempChar);
            }
            else if(tempChar=='^'||tempChar=='-')//����Ƿ�Χ�����
            {
                symStack.push(tempChar);//����ǰ�ķ�Χ�޶�������ջ
                continue;
            }
            else if(icp(tempChar)>isp(symStackTopchar))//�����˫Ŀ�����
            {
                symStack.push(tempChar);//����ǰ�ķ�����ջ
                continue;
            }
            else if(icp(tempChar)<isp(symStackTopchar) && tempChar != ')' && tempChar != ']')
            {
                while(icp(tempChar)<isp(symStackTopchar))
                {
                    operateBysym_2(tempChar);
                }
            }
            else if(tempChar != ')')//�����),��ôֱ��(�Ž���
            {
                char aa=symStack.top();symStack.pop();
                while(aa!='(')
                {
                    operateBysym_1(aa);
                    aa=symStack.top();symStack.pop();
                }
            }
            else if(tempChar != ']')//�����],��ôֱ��[�Ž���
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
    string regularExpressions="digtal(char|digtal)*";//������ʽ
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