#include<iostream>
#include<fstream>
#include <string>
#include<vector>
using namespace std;


//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5
char translator_table[100][5] = //Ԥ�����﷨�����
{
        "0@@1",
        "@1h1",
        "h1h0",
        "h.h0", //ͷ�ļ������� eg:@iostream

        "0111", //�յ���ĸ�ļ��
        "0_11", //��ʼ��_�ļ��,����ĸ��Ϊ����
        "1110", //����������ĸ������,���Գн�_,����
        "1_10", //��������_������
        "1310", //����������������

        "1((1", //��������(�ļ��
        //"(001",//���ŵ��ո�ļ��
        "(111", //���ŵ���ĸ�ļ��
        "(_11", //���ŵ�_�Ľ���

        "0**1", //�յ�*�ļ��
        "1**1", //��������*�ļ��
        "3**1", //���ֵ�*�ļ��
        "*111", //*����ĸ�ļ��
        "*_11", //*��_�ļ��

        "0++1", //�յ�+�ļ��
        "1++1", //��������+�ļ��
        "3++1", //���ֵ�+�ļ��
        "+111", //+����ĸ�ļ��
        "+_11", //+��_�ļ��

        //"0001",//�ո񵽿ո�ļ��
        //"*001",//*���ո�ļ��

        "0,01", //�յ�,�ļ��
        "1,01", //��������,�ļ��
        //",111",//,����ĸ�ļ��
        //",_11",//,��_�ļ��

        "0==1", //�յ�=�ļ��
        "1==1", //��������=�ļ��
        "=111", //=���������ļ��
        "=331", //=�����ֵļ��
        "0331", //�յ����ֵļ��

        "0>>1", //�յ�>�ļ��
        "1>>1", //������>�ļ��
        ">>20", //>>������

        "0<<1", //�յ�<�ļ��
        "1<<1", //������<�ļ��
        "<<20", //<<������
        "2111", //<<�������ļ��
        "2_11", //<<��_�ļ��

        "1+21",

};

char check_table[100][5] = //�﷨�����
    {
        "0##1", //��ʼ��#�ļ��
        "#1i1", //#��include�ļ��
        "i1i0", //include�ڵ���ĸ����
        "i<<1", //include��<�ļ��
        "<1h1", //<��ͷ�ļ��ļ��
        "h1h0", //ͷ�ļ�����ĸ����
        "h.h0", //ͷ�ļ���.������
        "h>>1", //ͷ�ļ���>�ļ��

        "0111", //�յ���ĸ�ļ��
        "0_11", //��ʼ��_�ļ��,����ĸ��Ϊ����
        "1110", //������������,���Գн�_,����
        "1_10", //��������_������
        "1310", //����������������
        "1001", //���������ո�ļ��
        "0;01", //�յ�;�ļ��
        "1;01", //��������;�ļ��
        "3;01", //���ֵ�;�ļ��
        "3.30",//���ֵ�.������
        "3330",//���ֵ����ֵ�����
        "0331", //�յ����ֵļ��

        "1((1", //��������(�ļ��
        //"(001",//���ŵ��ո�ļ��
        "(111", //���ŵ���ĸ�ļ��
        "(_11", //���ŵ�_�Ľ���

        "0**1", //�յ�*�ļ��
        "1**1", //��������*�ļ��
        "3**1", //���ֵ�*�ļ��

        "*111", //*����ĸ�ļ��
        "*_11", //*��_�ļ��

        "0++1", //�յ�+�ļ��
        "1++1", //��������+�ļ��
        "3++1", //���ֵ�+�ļ��
        "+111", //+����ĸ�ļ��
        "+_11", //+��_�ļ��

        //"0001",//�յ��ո�ļ��
        //"*001",//*���ո�ļ��

        "0,01", //�յ�,�ļ��
        "1,01", //��������,�ļ��
        //",111",//,����ĸ�ļ��
        //",_11",//,��_�ļ��

        "0==1", //�յ�=�ļ��
        "1==1", //��������=�ļ��
        "=111", //=���������ļ��
        "=331", //=�����ֵļ��
        "0331", //�յ����ֵļ��

        "0{01", //�յ�{�ļ��
        "){01", //)��{�ļ��
        "0}01", //�յ�}�ļ��

        "0>>1", //�յ�>�ļ��
        "1>>1", //������>�ļ��
        ">>20", //>>������

        "0<<1", //�յ�<�ļ��
        "1<<1", //������<�ļ��
        "<<20", //<<������
        "2111", //<<�������ļ��
        "2_11", //<<��_�ļ��

        "1+21", //
        "0\"\"1",//����"�ļ��
        "=\"\"1",//=��"�ļ��
        "2\"\"1",//���ź�"�ļ��
        //"1\"\"1",//��ĸ��"�ļ��
        "\"0s1",//"�Ϳյļ��
        "\"1s1",//"����ĸ�ļ��
        "\"2s1",//"�ͷ��ŵļ��
        "\"3s1",//"�����ֵļ��
        "s\"01",//�ַ�����"�ļ��

        "s0s0",//�ַ���������
        "s1s0",
        "s2s0",
        "s3s0",
        "s\\\\0",//�ַ�����\������
        "\\\"s0",//\��"������
        ">\n01",
        "2\n01",
        "0\n01"
};



int pointout(string str, char now_char, int file_type)
{

}

int len_check_table = sizeof(check_table) / sizeof(check_table[0]); //��¼c++�﷨��ĳ���
string keyword[100] = {"asm","auto","bool","break","case","catch","char","cin","class","const",
"const_cast","continue","cout","default","define","delete","do","double","dynamic_cast","else",
"endl","enum","explicit","export","extern","false","float","for","friend","goto","if","inline",
"int","long","mutable","namespace","new","operator","private","protected","public","register",
"reinterpret_cast","return","short","signed","sizeof","static","static_cast","struct","switch",
"template","this","throw","true","try","typedef","typeid","typename","union","unsigned","using"
,"virtual","void","volatile","wchar_t","while"};

string type_of_str[5] = {"������", "�������", "����", "�ַ���", "�ؼ���"};

int len_translator_table = sizeof(translator_table) / sizeof(translator_table[0]); //��¼�Զ��������﷨��ĳ���
string translator_keyword[100] = {"iostream", "int8", "__main__", "pyout", "pyend", "begin", "end", "use", "std"};

vector<pair<string, char>> vstrs;                     //װ�����е�����
string synxax_table[100]={};

// void translator(vector<pair<string, char>> vstrs)
// {

//     int len=vstrs.size();
//     for(int i=0;i<len;i++)
//     {
//         vstrs[i].second
//     }
// }

int pointin(string str, char now_char, int file_type=0) //����(�ַ���,�ַ������м�����,ɨ�����������0Ϊc++,1Ϊ�Զ�������),�����ַ�������������
{
    string *keyword_ = (file_type == 0 ? keyword : translator_keyword);
    if (now_char >= 48 && now_char <= 57) //���������
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
    else if ((now_char >= 65 && now_char <= 90) || (now_char >= 97 && now_char <= 122)) //�������ĸ
    {
        if(now_char=='s')return 4;
        return 5;
    }
    else //����Ƿ���
    {
        return 2;
    }
}



void scanner(string str, int file_type=0) //ɨ��ÿһ��,file_type������ɨ��c++�ļ�(file_type=0) ���� �Զ��������ļ�(file_type=1)
{
    char (*all_check_table)[5]=(file_type==0?check_table:translator_table);
    int len_all_check_table=(file_type==0?len_check_table:len_translator_table);
    char now_char = str[0];         //��ǰָ����ַ�
    int lenofeachline = str.size(); //�����еĳ���
    int index = 0;                  //��ʾ��ǰָ����ַ��±�
    string n = "";                  //���õ���token
    char now_type[5] = "0000";      //��ʼ����ǰ�ַ�״̬
    while (index <= lenofeachline)
    {
        now_char = str[index];
        now_type[0] = now_type[2];
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
                        pair<string, char> *last_str_type = new pair<string, char>(n, now_type[0]);
                        vstrs.push_back(*last_str_type);
                        n = "";
                    }
                    now_type[2] = '0';
                    index++;
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
        for (int i = 0; i < len_all_check_table; i++) //ƥ������
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
    if (file_type == 1 && n[0] != '\0') //��Ϊc++��ÿһ�������һ�������� ;,},>��,��pycpp�����в�һ����,������һ�е�ĩβ�ж�,�Ƿ���û������ű�ķ���
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
            scanner(str);//�����ǵ������� ,Ĭ��Ϊɨ��cpp�ļ�
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
            scanner(str,1);//��������������,���ҵڶ�������Ϊ1,��ôɨ������Զ����ļ�
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

