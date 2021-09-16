// mgc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//敏感词类
class sensitivewords
{
public:
    int bh;  // 编号
    string word;
    int type;  // 英文0，汉字1
    int Leng;  // 敏感词长度
}words[500];
int word_sum = 0;

struct line
{
    int leng;
    int LINE;
    string content;  // 内容
}l[1001];

struct answer
{
    int num;  // 存放敏感词在哪一行出现
    int bh;
    string ans;  // 存放测试文本中的含敏感词的字符串
    int anstart;  // 头位置
    int ansend;  // 尾位置
}ansout[2000];

int sum = 0;  // 总数
int line_sum = 0;
void dj()
{
    string address;
    cin >> address;  // 输入文件地址
    ifstream in(address);  // 打开敏感词文件
    string sensi;
    int i = 0;

    if (in) // 有该文件
    {
        while (getline(in, sensi)) // 行中不包括每行的换行符,sensi每次存放一个敏感词
        {
            words[i].word = sensi;
            words[i].bh = i;
            words[i].Leng = sensi.size();
            if (words[i].word[0] & 0x80)
                words[i].type = 1;  // 汉字
            else words[i].type = 0;  // 英文
            i++;
            word_sum++;
        }
    }
    else // 没有该文件
    {
        cout << "no such file" << endl;
        return;
    }
    int x = i;
    in.close();

    //读入测试文本
    cin >> address;
    ifstream f(address);  // 打开测试文本
    string sentence;
    int j = 0;
    if (f)  // 有该文件
    {
        while (getline(f, sentence))
        {
            l[j].content = sentence;  //测试文本的每一行存入line结构体数组l中的content
            l[j].LINE = j + 1;
            l[j].leng = sentence.size();
            j++;
            line_sum++;
        }
    }
    else return;
    int y = j;
    f.close();
}
void search(sensitivewords& p, line& q)
{
    int max = 0;

    int i, j;
    int start;  // 答案的头位置
    if (p.type == 0)  //英文敏感词
    {
        i = 0;
        for (j = 0; j < q.leng; j++)
        {
            //大写转换为小写
            if (q.content[j] >= 'A' && q.content[j] <= 'Z')
                q.content[j] += 'a' - 'A';
            if (p.word[i] == q.content[j])
            {
                max++;
                if (max > 20) break;  // 敏感词中间穿插了超过20个字符就不检测
                if (i == 0) start = j;
                i++;
                if (i == p.Leng)  // 匹配成功
                {
                    ansout[sum].anstart = start;
                    ansout[sum].ansend = j;
                    ansout[sum].num = q.LINE;
                    ansout[sum].bh = p.bh;
                    sum++;
                    i = 0;
                    max = 0;
                }
            }
        }
    }
    if (p.type == 1)  // 汉字敏感词
    {
        i = 0;
        //如果是汉语，在UTF-8中三个字节才能代表一个字符。
        for (j = 0; j < q.leng; j++)
        {
            if (p.word[i] == q.content[j] && p.word[i + 1] == q.content[j + 1] && p.word[i + 2] == q.content[j + 2])  //三个字节相等=一个汉字匹配
            {
                max++;
                if (max > 60) break;  // 敏感词中间穿插了超过60个字符就不检测
                if (i == 0) start = j;
                i += 3;
                j += 2;
                if (i == p.Leng)  //匹配成功
                {
                    ansout[sum].anstart = start;
                    ansout[sum].ansend = j;
                    ansout[sum].num = q.LINE;
                    ansout[sum].bh = p.bh;
                    sum++;
                    i = 0;
                    max = 0;
                }
            }
        }
    }

}

// 输出结果到文件中
void print()
{
    string address;
    cin >> address;
    ofstream out;
    out.open(address);
    int i, j;
    out << "Total: " << sum << endl;
    for (i = 0; i < sum; i++)
    {
        out << "Line" << ansout[i].num << ": <" << words[ansout[i].bh].word << "> ";
        for (j = ansout[i].anstart; j <= ansout[i].ansend; j++)
        {
            out << l[ansout[i].num - 1].content[j];
        }
        out << endl;
    }
}

int main()
{
    system("chcp 65001");
    string language;
    cin >> language;
    int i = 0, j = 0;
    dj();
    for (i = 0; i < line_sum; i++)
        for (j = 0; j < word_sum; j++)
            search(words[j], l[i]);
    print();
    return 0;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
