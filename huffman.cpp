#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED
#include <map>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <algorithm>
#define MAX 100000
using namespace std;


struct Huffman
{

	Huffman(string c, int w) :weight(w), ch(c), lchild(NULL), rchild(NULL) {}
	Huffman() :ch(""), lchild(NULL), rchild(NULL) {}
    string ch = "";
	int weight;
	Huffman *lchild, *rchild;
};
typedef Huffman* Node;



class compare
{
public:
	bool  operator()(const Node&ch1, const Node&ch2)const
	{
		return (*ch1).weight > (*ch2).weight;
	}
};
priority_queue<Node, vector<Node>, compare> pq;
void CalWeight(string fin);
void iniQueue();
void CreateHTree(Node &node);
void Encode(string code, Node node);
void Compress(string fin, string fout);
void DeCompre(string fin, string fout);
void conduct();
string turnchar(unsigned char c);



map<string, int>map1;
map<string, string>map2;
map<string, int>::iterator it;


void CalWeight(string fin)
{
    char ch;
	string s;
	ifstream infile(fin.c_str());
	if (!infile.good())
	{
		cout << "Can Not Open the File1" << endl;
		return;
	}
	while (!infile.eof())
	{
		infile.get(ch);
		if (!infile.eof())
		{
			s = ch;
			map1[s] = map1[s] + 1;
		}
	}
	infile.close();
}
void iniQueue()
{
	for (it = map1.begin(); it != map1.end(); it++)
	{
		Node n = new Huffman(it->first, it->second);
		pq.push(n);
	}
}


void CreateHTree(Node &node)
{

	iniQueue();
	unsigned int i=0;
	while (pq.size() > 1)
	{
		Node n1 = pq.top();
		pq.pop();
		Node n2 = pq.top();
		pq.pop();
		string nch = n1->ch + n2->ch;
		int weight = n1->weight + n2->weight;
		node= new Huffman(nch, weight);
		node->lchild = n1;
		node->rchild = n2;
		pq.push(node);
		i++;


	}

}
void Encode(string code, Node node)
{
	if (!node || node->ch == "")
		return;
	if (node->ch.length()==1)
		map2[node->ch] = code;
	if (node->rchild)
	{
		Node right = node->rchild;
		Encode(code + '1', right);
	}
	if (node->lchild)
	{
		Node left = node->lchild;
		Encode(code + '0', left);
	}

}

void Compress(string fin,string fout)
{

	ofstream outfile(fout.c_str(), ios::out | ios::binary);

	if (!outfile.good())
	{
		cout << "Can Not Open the File2" << endl;
		return;
	}
	ifstream infile(fin.c_str());
	if (!infile.good())
	{
		cout << "Can Not Open the File3" << endl;
		return;
	}
	map<string, string>::iterator it2;
	outfile << map2.size() << " ";
	for (it2 = map2.begin(); it2 != map2.end(); it2++)
	{

		 if(it2->first.c_str()[0]=='\n')
            {
                outfile<<endl;
             }
        outfile << it2->first << it2->second << " ";
	}
	char ch;
	string s;
	string code = "";
	string str = "";
	unsigned long temp;
	unsigned char byte;
	int length = code.length();
	while (!infile.eof())
	{
		while (length < MAX && !infile.eof())
		{
			infile.get(ch);
			if (!infile.eof())
			{
				s = ch;
				code += map2[s];
				length = code.length();
			}
		}
		while (length >= 8)
		{
			str = code.substr(0, 8);
			bitset<8> bits(str);
			temp = bits.to_ulong();
			byte = temp;
			outfile << byte;
			code = code.substr(8, length - 8);
			length = code.length();

		}
		if (length != 0)
		{
			str = code.substr(0, length);
			int i = 0;
			while (i < 8- length)
			{
				str += '0';
				i++;
			}
			bitset<8> bits(str);
			temp = bits.to_ulong();
			byte = temp;
			outfile << byte;

		}
		char n = (char)(8 - length) % 8;
		outfile << n;
		infile.close();
		outfile.close();
	}
	cout<<"压缩成功"<<endl;

}
void test()
{
    map<string, string>::iterator it2;
	cout<<map2.size() << " ";
	for (it2 = map2.begin(); it2 != map2.end(); it2++)
	{
		cout << it2->first <<" "<< it2->second << " ";
	}
}
string turnchar(unsigned char c)
{
	string k = "";
	int j = 128;
	for (int i = 0; i < 8; i++)
	{
		k += (unsigned char)(bool)(c&j) + '0';
		j >>= 1;
	}
	return k;

}
string result = "";
void DeCompre(string fin,string  fout)
{
	ifstream in(fin.c_str(), ios::binary);
	if (in.fail()) {
		cout << "Fail to open the file1 !" << endl;
		return;
	}

	ofstream out(fout.c_str(), ios_base::trunc);
	if (out.fail()) {
		cerr << "Fail to open the file2 !" << endl;
		return;
	}
	int size;
	in >> size;
	char code;
	string ch;
	char space;
	map<string, char>map3;
	in.get(space);
	while (size > 0)
	{
		in.get(code);
		in >> ch;
		map3[ch] = code;
		in.get(space);
		size--;
	}

	string cs = "";
	int length = cs.length();
	char c;
	int num;
	unsigned char s;
	while (!in.fail())
	{
		cs.clear();
		result.clear();
		length = cs.length();
		while (length < MAX && !in.eof())
		{
			in.get(c);
			if (!in.eof())
			{
				s = (unsigned char)c;
				cs += turnchar(s);
				length = cs.length();
			}
		}

		if (in.fail())
		{
			num = (int)c;
			cs = cs.substr(0, cs.length() -8 - num);
		}

		string strrrrr = "";
		bool flag = false;
		int i = 1;
		while (cs.length() > 0)
		{
			strrrrr = cs.substr(0, i);
			while (map3.find(strrrrr) == map3.end())
			{
				i++;
				length = cs.length();
				if (i > length)
				{
					flag = true;
					break;
				}
				strrrrr = cs.substr(0, i);
			}
			cs = cs.substr(i, length - i);
			i = 1;
			if (flag == true)
			{
				break;
			}
			result += map3[strrrrr];
		}
		out << result;
	}
	out.close();
	in.close();
    cout<<"解压缩成功！"<<endl;
}
void conduct()
{

       // cout<<"*****"<<pq.top()->ch<<" "<<pq.top()->weight<<endl;
		string code = "";
		Node node=NULL;
	    string original1;
	    string original2;
	    string aftcom;
	    string aftdecom;
        cout << "请选择您的操作：" << endl << "1:压缩" << endl << "2:解压缩" << endl << "3:退出" << endl<<"4:返回上一级"<<endl;
		int opt;
		cin >> opt;
		switch(opt)
		{
        case 1:
            cout << "请输入文件名称：";
			fflush(stdin);
			cin >> original1;
			cout << "请输入压缩文件文件名称: ";
			fflush(stdin);
			cin >> aftcom;
			CalWeight(original1);
			CreateHTree(node);
			Encode(code, node);
			Compress(original1, aftcom);
			conduct();
			break;
        case 2:
            cout << "请输入需要解压的文件名称：";
			fflush(stdin);
			cin >> original2;
			cout << "请输入解压缩后文件文件名称: ";
			fflush(stdin);
			cin >> aftdecom;
			DeCompre(original2, aftdecom);
			conduct();
			break;
        case 3:
            break;
        default:
            cout << "输入错误，请重新输入" << endl;conduct();break;
		}


}
int main()
{
    conduct();
return 0;
}
#endif // HUFFMAN_H_INCLUDED
