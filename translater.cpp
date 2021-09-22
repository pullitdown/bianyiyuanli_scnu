#include<iostream>
#include<fstream>
#include <string>
#include<vector>
using namespace std;

//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5
char check_table[100][5]=//�﷨�����
{
"0##1",//��ʼ��#�ļ��
"#1i1",//#��include�ļ��
"i1i0",//include�ڵ���ĸ����
"i<<1",//include��<�ļ��
"<1h1",//<��ͷ�ļ��ļ�� 
"h1h0",//ͷ�ļ�����ĸ����
"h.h0",//ͷ�ļ���.������
"h>>1",//ͷ�ļ���>�ļ��

"0111",//�ո���ĸ�ļ��
"0_11",//��ʼ��_�ļ��,����ĸ��Ϊ����
"1110",//������������,���Գн�_,����
"1_10",//��������_������
"1310",//����������������
"1001",//���������ո�ļ��
"0;01",//�յ�;�ļ��
"1;01",//��������;�ļ��
"3;01",//���ֵ�;�ļ��
"0331",//�ո����ֵļ��

"1((1",//��������(�ļ��
//"(001",//���ŵ��ո�ļ��
"(111",//���ŵ���ĸ�ļ��
"(_11",//���ŵ�_�Ľ���

"0**1",//�յ�*�ļ��
"1**1",//��������*�ļ��
"3**1",//���ֵ�*�ļ��
"*111",//*����ĸ�ļ��
"*_11",//*��_�ļ��

"0++1",//�յ�+�ļ��
"1++1",//��������+�ļ��
"3++1",//���ֵ�+�ļ��
"+111",//+����ĸ�ļ��
"+_11",//+��_�ļ��

//"0001",//�ո񵽿ո�ļ��
//"*001",//*���ո�ļ��

"0,01",//�ո�,�ļ��
"1,01",//��������,�ļ��
//",111",//,����ĸ�ļ��
//",_11",//,��_�ļ��

"0==1",//�յ�=�ļ��
"1==1",//��������=�ļ��
"=111",//=���������ļ��
"=331",//=�����ֵļ��

"0{01",//�յ�{�ļ��
"){01",//)��{�ļ��
"0}01",//�յ�}�ļ��

"0>>1",//�յ�>�ļ��
"1>>1",//������>�ļ��
">>20",//>>������

"0<<1",//�յ�<�ļ��
"1<<1",//������<�ļ��
"<<20",//<<������
"2111",//<<�������ļ��
"2_11",//<<��_�ļ��

"1+21",//
};
int len_check_table=sizeof(check_table)/sizeof(check_table[0]);
string keyword[100] ={"include","iostream.h","int","main","return","cout","endl"}; 
string translation_keyword[100]={"import","iostream.h","int8",""};
string type_of_str[5]={"������","�������","����","�ַ���","�ؼ���"};
vector<pair<string,char>> vstrs;//װ�����е�����
int pointin(string str,char now_char)
{
    if(now_char>=48&&now_char<=57)//���������
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
        else if((now_char>=65&&now_char<=90)||(now_char>=97&&now_char<=122))//�������ĸ
        {
            return 5;
        }
        else//����Ƿ���
        {
            return 2;
        }
}
void scanner(string str)//ɨ��ÿһ��
{
    char now_char=str[0];//��ǰָ����ַ�
    int lenofeachline=str.size();//�����еĳ���
    int index=0;//��ʾ��ǰָ����ַ��±�
    string n="";//���õ���token
    char now_type[5]="0000";//��ʼ����ǰ�ַ�״̬
    while(index<=lenofeachline)
    {
        now_char=str[index];
        now_type[0]=now_type[2];
        if(now_char>=48&&now_char<=57)//���������
        {
            now_type[1]='3';
        }
        else if((now_char>=65&&now_char<=90)||(now_char>=97&&now_char<=122))//�������ĸ
        {
            now_type[1]='1';
        }
        else//����Ƿ���
        {
            if(now_char==' '){
                if(now_type[0]!='s')//��������ַ�����
                {
                    if(n!=""&&now_type[0]!='0')//�����ʱ��n���浽����,������һ�����ݲ�Ϊ0
                    {
                        pair<string,char> *last_str_type=new pair<string,char>(n,now_type[0]);
                        vstrs.push_back(*last_str_type);
                        n="";
                    }
                    now_type[2]='0';
                    index++;
                    continue;
                }
                else//������ַ�����,�ո���Ϊ����ĸ
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
        for(int i=0;i<len_check_table;i++)//ƥ������
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