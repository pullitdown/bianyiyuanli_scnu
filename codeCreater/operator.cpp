
#pragma once
#include <bits\stdc++.h>
using namespace std;

template <typename T>
class Stack {
private:
	T* element;
	T* top;
	int size;
	int maxsize;
public:
	Stack();
	//Stack(const Stack& a);
	~Stack();
	Stack(int a);
	T pop();
	void push(T a);

	T gettop()const;
	T gettop();
	bool isempty();
	bool isfull();
	void clear();
	void show() {
		for (int i = 0; i < size; i++)
		{
			cout << i + 1 << " " << element[i] << endl;
		}
	}
	int getmaxsize()const {
		return maxsize;
	}
	int getcurrsize()const {
		return size;
	}
};

template<typename T>
inline Stack<T>::Stack()
{
	this->maxsize = 100;
	this->size = 0;
	element = new T[101];
	top = element;
}

//template<typename T>
//inline Stack<T>::Stack(const Stack& a)
//{
//	int aa = a.getmaxsize();
//	this->maxsize = aa;
//	this->size = 0;
//	element = new T[aa + 1];
//
//	top = element;
//}

template<typename T>
inline Stack<T>::~Stack()
{
	delete[]element;
	size = 0;
	element = nullptr;
	top = NULL;
	maxsize = 0;
}

template<typename T>
inline Stack<T>::Stack(int a)//传入栈的maxsize初始化
{
	this->maxsize = a;
	this->size = 0;
	element = new T[a + 1];
	*element = NULL;
	top = element;

}

template<typename T>
inline T Stack<T>::pop()
{
	if (!isempty())
	{

		size--;

		top--;
		T temp = *top;
		*top = 0;

		return temp;
	}
	else
	{
		cout << "stack is empty,fail_pop,exit(0)" << endl;
		exit(0);
	}
}

template<typename T>
inline void Stack<T>::push(T a)
{
	if (!isfull())
	{

		size++;
		*top = a;
		top++;
	}
	else
	{
		cout << "stack is full,fail_push,exit(0)" << endl;
		exit(0);
	}
}

template<typename T>
inline T Stack<T>::gettop()const
{

	return *(top - 1);
}
template<typename T>
inline T Stack<T>::gettop()
{

	return *(top - 1);
}

template<typename T>
inline bool Stack<T>::isempty()
{
	if (size == 0)
	{
		return true;
	}
	return false;
}

template<typename T>
inline bool Stack<T>::isfull()
{
	if (size == maxsize)
	{
		return true;
	}
	return false;
}

template<typename T>
inline void Stack<T>::clear()
{
	for (int i = 0; i < size; i++)
	{
		element[i] = 0;
	}
	size = 0;
	top = element;
}

using namespace std;
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
double func(double a, double b, char type)
{
	switch (type)
	{
	case '+':return a + b;
	case '-':return a - b;
	case '*':return a * b;
	case '/':return a / b;
		//case '%':return a % b;
	default:cout << "fulse" << endl; return 0;
	}
}
double final_num[10000];
int main()
{
	cout << "作业5:计数器,数据类型支持浮点数,支持+,-,*,/,(,)等运算符\n数据类型为float\n测试数据:\n2 \n2*(1.5+0.3*2)+1.0\n6.3+2*8/4" << endl;

	Stack<double> num(100);
	Stack<char> cchar(100);
	char fo = '#';
	cchar.push(fo);
	int n, index = 0;
	cout << "Please input the number of formulas" << endl;
	cin >> n;//输入算式的个数
	string goal;

	for (int i = 0; i < n; i++)
	{

		cin >> goal;//输入算式
		goal += '#';//末尾符
		int len = goal.length();
		int e = 1;//判断浮点数小数点前有几位数
		int dio = 0;//最终的数值要乘的阶数
		double temp = 0;//输入数值的中转值
		int flat = 0, flat1 = 0;//flat记录运算符前一个是否为数值,处理连续的运算符,flat1记录是否为上一个数字浮点数,
		for (int p = 0; p < len; p++)
		{
			if ((goal[p] >= '0' && goal[p] <= '9') || goal[p] == '.')//数值处理部分
			{
				if (goal[p] == '.')//小数点处理入口
				{

					dio = e - 1;
					flat1 = 1;
					continue;
				}
				temp += (goal[p] - '0') * pow(10, -e);//每一位乘一个越来越小的倍率
				flat = 1;
				e++;
			}

			if (goal[p] == '/' || goal[p] == '-' || goal[p] == '%' || goal[p] == '+' || goal[p] == '*' || goal[p] == '(' || goal[p] == ')' || goal[p] == '#')//操作符入口
			{
				if (flat)
				{
					if (!flat1)
					{
						dio = e - 1;//处理temp的阶数

					}
					temp *= pow(10, dio);
					num.push(temp);
					//cout << "temp:" << temp << endl;

					temp = 0;//初始化前面的数据值
					e = 1;
					dio = 0;
					flat = 0;
					flat1 = 0;
				}

				if (icp(goal[p]) > isp(cchar.gettop()))//判断优先级
				{
					cchar.push(goal[p]);//如果栈外的符号优先于栈顶符,则入栈
					continue;
				}

				else if (icp(goal[p]) < isp(cchar.gettop()) && goal[p] != ')')//栈内操作符优先于外部,并且不为右括号时
				{


					while (icp(goal[p]) < isp(cchar.gettop()))//按数值栈出二,符号栈出二的规律,作相应运算
					{

						double a = num.pop();


						double b = num.pop();

						double c = func(b, a, cchar.pop());

						num.push(c);
					}
					cchar.push(goal[p]);//把没次运算得到的数压回栈内
				}

				if (goal[p] == ')')
				{
					char aa = cchar.pop();//直接pop出符号栈内的操作符
					while (aa != '(')//等于(时直接忽略,此时也说明两个括号内只有一个数
					{
						//否则就对括号内的全部符号直接运算,符合运算顺序
						double a = num.pop();
						double b = num.pop();
						double c = func(b, a, aa);
						num.push(c);
						aa = cchar.pop();
					}
				}
				//num.show();
				//cchar.show();
			}
		}
		final_num[index++] = num.pop();//开数组存最后的结果
		num.clear();//清空两个栈的数值
		cchar.clear();
	}
	for (int i = 0; i < n; i++)//输出
	{
		cout << "no " << i + 1 << ". answer:" << final_num[i] << endl;
	}

	system("pause");
	return 0;

}

