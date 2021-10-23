#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<stack>
#include<algorithm>
using namespace std;

//---------------------������ʽ�ķִʲ���------------------------------
//�ִʹ����
char syntaxTable_RegularExpressions[100][6]=
{

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
    int* nextTypeIndex = = nextTypeIndex0;
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
            cout << last_start << endl;
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
    vector<int> numlist;//�ڵ���±�
    vector<pair<string,int>> link;//�ڵ��·��
    int lenLink;
    _node(int index)
    {
        lenLink=0;
        numlist.push_back(index);
    }
}*node;
struct _node tree[200];
stack<char> symStack;//����ջ
stack<node> nodeStack;//�ڵ�ջ

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
            node nodeptr=new _node(nodeIndex);//ͨ��nodeIndex��ʼ���ڵ�ֵ
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
    string regularExpressions="";//������ʽ
    cin>>regularExpressions;
    int len_regularExpressions=regularExpressions.size();
    for(int i=0;i<len_regularExpressions;i++)
    {
        
    }
}