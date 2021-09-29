#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<algorithm>
using namespace std;


//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5
char translator_table[100][6] = //Ԥ�����﷨�����
{
        "0@@10",
        "@1h10",
        "h1h00",
        "h.h00", //ͷ�ļ������� eg:@iostream

        "01110", //�յ���ĸ�ļ��
        "0_110", //��ʼ��_�ļ��,����ĸ��Ϊ����
        "11100", //����������ĸ������,���Գн�_,����
        "1_100", //��������_������
        "13100", //����������������

        "1((10", //��������(�ļ��
        //"(0010",//���ŵ��ո�ļ��
        "(1110", //���ŵ���ĸ�ļ��
        "(_110", //���ŵ�_�Ľ���

        "0**10", //�յ�*�ļ��
        "1**10", //��������*�ļ��
        "3**10", //���ֵ�*�ļ��
        "*1110", //*����ĸ�ļ��
        "*_110", //*��_�ļ��

        "0++10", //�յ�+�ļ��
        "1++10", //��������+�ļ��
        "3++10", //���ֵ�+�ļ��
        "+1110", //+����ĸ�ļ��
        "+_110", //+��_�ļ��

        //"00010",//�ո񵽿ո�ļ��
        //"*0010",//*���ո�ļ��

        "0,010", //�յ�,�ļ��
        "1,010", //��������,�ļ��
        //",1110",//,����ĸ�ļ��
        //",_110",//,��_�ļ��

        "0==10", //�յ�=�ļ��
        "1==10", //��������=�ļ��
        "=1110", //=���������ļ��
        "=3310", //=�����ֵļ��
        "03310", //�յ����ֵļ��

        "0>>10", //�յ�>�ļ��
        "1>>10", //������>�ļ��
        ">>200", //>>������

        "0<<10", //�յ�<�ļ��
        "1<<10", //������<�ļ��
        "<<200", //<<������
        "21110", //<<�������ļ��
        "2_110", //<<��_�ļ��

        "1+210",

};

char check_table[100][6] = //�﷨�����
    {
        "0##10", //��ʼ��#�ļ��
        "#1i10", //#��include�ļ��
        "i1i00", //include�ڵ���ĸ����
        "i<<10", //include��<�ļ��
        "<1h10", //<��ͷ�ļ��ļ��
        "h1h00", //ͷ�ļ�����ĸ����
        "h.h00", //ͷ�ļ���.������
        "h>>10", //ͷ�ļ���>�ļ��

        "01110", //�յ���ĸ�ļ��
        "0_110", //��ʼ��_�ļ��,����ĸ��Ϊ����
        "11100", //������������,���Գн�_,����
        "1_100", //��������_������
        "13100", //����������������
        "10010", //���������ո�ļ��
        "0;010", //�յ�;�ļ��
        "1;010", //��������;�ļ��
        "3;010", //���ֵ�;�ļ��
        "3.300",//���ֵ�.������
        "33300",//���ֵ����ֵ�����
        "03310", //�յ����ֵļ��

        "1((10", //��������(�ļ��
        //"(0010",//���ŵ��ո�ļ��
        "(1110", //���ŵ���ĸ�ļ��
        "(_110", //���ŵ�_�Ľ���

        "0**10", //�յ�*�ļ��
        "1**10", //��������*�ļ��
        "3**10", //���ֵ�*�ļ��

        "*1110", //*����ĸ�ļ��
        "*_110", //*��_�ļ��

        "0++10", //�յ�+�ļ��
        "1++10", //��������+�ļ��
        "3++10", //���ֵ�+�ļ��
        "+1110", //+����ĸ�ļ��
        "+_110", //+��_�ļ��

        //"00010",//�յ��ո�ļ��
        //"*0010",//*���ո�ļ��

        "0,010", //�յ�,�ļ��
        "1,010", //��������,�ļ��
        //",1110",//,����ĸ�ļ��
        //",_110",//,��_�ļ��

        "0==10", //�յ�=�ļ��
        "1==10", //��������=�ļ��
        "=1110", //=���������ļ��
        "=3310", //=�����ֵļ��
        "03310", //�յ����ֵļ��

        "0{010", //�յ�{�ļ��
        "){010", //)��{�ļ��
        "0}010", //�յ�}�ļ��

        "0>>10", //�յ�>�ļ��
        "1>>10", //������>�ļ��
        ">>200", //>>������

        "0<<10", //�յ�<�ļ��
        "1<<10", //������<�ļ��
        "<<200", //<<������
        "21110", //<<�������ļ��
        "2_110", //<<��_�ļ��

        "1+210", //
        "0\"\"10",//����"�ļ��
        "=\"\"10",//=��"�ļ��
        "2\"\"10",//���ź�"�ļ��
        //"1\"\"10",//��ĸ��"�ļ��
        "\"0s10",//"�Ϳյļ��
        "\"1s10",//"����ĸ�ļ��
        "\"2s10",//"�ͷ��ŵļ��
        "\"3s10",//"�����ֵļ��
        "s\"010",//�ַ�����"�ļ��

        "s0s00",//�ַ���������
        "s1s00",
        "s2s00",
        "s3s00",
        "s\\\\00",//�ַ�����\������
        "\\\"s00",//\��"������
        ">\n010",
        "2\n010",
        "0\n010"
};
//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5
//������:�������0,1����,2�����ռ�,3�����������������,4��������,5���Ŵ���,6������ֵ,7��������

//����﷨��
//����һ������������,����һ�������¼ÿһ��item����ʼλ��,
int synx_table[10000]
{
    //ÿ�ĸ���Ϊһ���﷨����
    //��һ�����ִ�����һ��������,�ڶ������ִ����string������
    //���������ִ�����ϸ�strin��,���е�������,���ĸ����ִ����Ƿ�Ҫ�ָ�
    //��ôҪȷ���ľ��ǵڶ���Ĳ�������
    //����ǹؼ���,��ôȡ��ֵ��(�ؼ��������±�+100)
    //����Ƿ���,ֱ��ȡ���ű���
    //���������,ȡ-3,������ȡ-1,�ַ���ȡ-4,
    0,'@',1,1,//�յ�@�ļ��
    1,'h',1,0,//���뵽ͷ�ļ�����
    1,'@',1,1,//���뵽@�ļ��
    1,-162,2,1,//���뵽use�ļ��
    2,1,2,0,//use������(�����ռ�)������
    2,-133,3,1,//�����ռ䵽�����������͵�����
    2,-134,3,1,//int16
    2,-102,3,1,//bool
    2,-126,3,1,//float
    3,-168,3,0,//����������������������
    3,'(',4,0,//����������(������
    4,-133,4,0,//(�������������͵�����
    4,-134,4,0,//int16
    4,-102,4,0,//bool
    4,-126,4,0,//float
    4,1,4,0,//����������������������
    4,',',',',0,//����������������������
    
    3,1,3,0,//�����������͵�������������

    3,'(',3,0,


};

// int translator_table[10000]=
// {
//     //��ͬ���͵�ת������ͬ


// };

bool cmp(char arr1[6],char arr2[6])
{
    return strcmp(arr1,arr2);
}
void init_table()//���﷨������������,���һ�ò�ͬ�﷨����ĳнӹ�ϵ,�Ա���������ʱ���и�����ٶ�
{
    sort(check_table,check_table+20,cmp);
    for(int i=0;i<90;i++)
    {
        cout<<check_table[i]<<endl;
    }
}


int len_check_table = sizeof(check_table) / sizeof(check_table[0]); //��¼c++�﷨��ĳ���
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

string type_of_str[5] = {"������", "�������", "����", "�ַ���", "�ؼ���"};

int len_translator_table = sizeof(translator_table) / sizeof(translator_table[0]); //��¼�Զ��������﷨��ĳ���
string translator_keyword[100] = {"iostream", "int8", "__main__", "pyout", "pyend", "begin", "end", "use", "std"};

vector<pair<string, char>> vstrs;                     //װ�����е�����



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
    int last_start=0;//��¼���ε�ƥ���﷨����,��нӵ���һ�ι�����ʼ��
    char (*all_check_table)[6]=(file_type==0?check_table:translator_table);
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
        for (int i = last_start; i < len_all_check_table; i++) //ƥ������
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


//
