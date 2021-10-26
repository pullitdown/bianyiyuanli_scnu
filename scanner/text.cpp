#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include<algorithm>

using namespace std;


//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5
char translator_table[200][6] = //Ԥ�����﷨�����
{
        "0@@10",
        "@1h10",
        "h1h00",
        "h.h00", //ͷ�ļ������� eg:@iostream
        "h\n010",

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

char check_table[200][6] = //�﷨�����
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
    "3))10",//���ֵ�)�ļ��
    "1))10",//�ַ���)�ļ��
    ");010",//)��;�ļ��

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
    "0\n010",
    "1[[10",//�������������ŵļ��
    "[3310",//�����ŵ����ֵļ��
    "3]]10",//��ֵ�������ŵļ��
    "{3310",//���ŵ����ֵļ��
    "3}}10",//���ֵ�}�ļ��
    "};010",//}��;�ļ��
    "]==10",//]��=�ļ��
    "={{10",//=��{�ļ��
    "3,010",//���ֵ�,�ļ��
    "=\'\'10",//�Ⱥŵ�'��j
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
int nextTypeIndex0[200];//������¼ ĳ�������ڴʷ������״γ��ֵ��±�
int nextTypeIndex1[200];
int compar_words(const void* p, const void* q)
{
    return strcmp((char*)p, (char*)q);
}
void init_table(int file_type = 0)//���﷨������������,���һ�ò�ͬ�﷨����ĳнӹ�ϵ,�Ա���������ʱ���и�����ٶ�
{
    int* nextTypeIndex = file_type == 1 ? nextTypeIndex1 : nextTypeIndex0;

    char(*all_check_table)[6] = (file_type == 0 ? check_table : translator_table);
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
//---------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------
int len_check_table = sizeof(check_table) / sizeof(check_table[0]); //��¼c++�﷨��ĳ���




string type_of_str[7] = { "������", "�������", "����", "�ַ���", "�ؼ���" ,"�ַ�","ע��"};

int len_translator_table = sizeof(translator_table) / sizeof(translator_table[0]); //��¼�Զ��������﷨��ĳ���


vector<pair<string, char>> vstrs;                     //װ�����е�����
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
    else if ((now_char >= 65 && now_char <= 90) || (now_char >= 97 && now_char <= 122)) //�������ĸ
    {
        if (now_char == 's')return 4;
        return -5;
    }
    else //����Ƿ���
    {
        return now_char;
    }
}

int pointin(string str, char now_char, int file_type = 0) //����(�ַ���,�ַ������м�����,ɨ�����������0Ϊc++,1Ϊ�Զ�������),�����ַ�������������
{
    string* keyword_ = (file_type == 0 ? keyword : pykeyword);
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
        if (now_char == 's')return 4;
        return 5;
    }
    else //����Ƿ���
    {
        return 2;
    }
}
//char����:�ո�:0 ��ĸ:1 ����:2 ��:3
//string����:�ջ���ĩ:0,������:1 �������:2 ����:3 �ַ���:4 �ؼ���:5

void scanner(string str, int file_type = 0) //ɨ��ÿһ��,file_type������ɨ��c++�ļ�(file_type=0) ���� �Զ��������ļ�(file_type=1)
{
    int* nextTypeIndex = file_type == 0 ? nextTypeIndex0 : nextTypeIndex1;
    unsigned int last_start = 0;//��¼���ε�ƥ���﷨����,��нӵ���һ�ι�����ʼ��
    char(*all_check_table)[6] = (file_type == 0 ? check_table : translator_table);
    int len_all_check_table = (file_type == 0 ? len_check_table : len_translator_table);
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
                int lenTemp = 0;//��¼temp����
                int indexTemp = 0;//��¼temp��Ŀǰ�±�
                while (sys[step] != 1)
                {
                    if (sys[step] == -1)
                    {
                        i++;//����һ���ַ���
                        temp[lenTemp++] = vstrs[i].first;
                    }
                    step++;
                }
                step++;//����1�ָ�֮�������
                while (step < level[j + 1])
                {
                    if (sys[step] > 0)goal += (char)sys[step];
                    else if (sys[step]<0 && sys[step]>-99 && indexTemp < lenTemp)goal += temp[indexTemp++];
                    else goal += keyword[-sys[step] - 100];
                    step++;
                }
                break;//����������,�˳�
            }
        }
        if (flag == 0)//���û��ת�����еõ�����
        {
            if (i <= vstrs.size())
                goal += vstrs[i].first;
            cout << "ֱ�Ӽ���" << endl;
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
            scanner(str);//�����ǵ������� ,Ĭ��Ϊɨ��cpp�ļ�
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
    //         scanner(str,1);//�����ǵ������� ,Ĭ��Ϊɨ��cpp�ļ�
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
