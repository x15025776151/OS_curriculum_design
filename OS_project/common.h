#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include <cstdio>
#include <iostream>
#include <cstring>
#include <queue>
#include <cmath>
#include <algorithm>
#include <stack>
#include <map>
#include <string>
#include <set>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#define Clear(x,y) memset (x,y,sizeof(x))
#define Close() ios::sync_with_stdio(0)
#define Open() freopen ("more.in", "r", stdin)
#define get_min(a,b) a = min (a, b)
#define get_max(a,b) a = max (a, b);
#define y0 yzz
#define y1 yzzz
#define fi first
#define se second
#define pii pair<int, int>
#define pli pair<long long, int>
#define pll pair<long long, long long>
#define pdi pair<double, int>
#define pdd pair<double, double>
#define vei vector<int>
#define vel vector<long long>
#define pb push_back
#define pl c<<1
#define pr (c<<1)|1
#define lson l,mid,pl
#define rson mid+1,r,pr
typedef unsigned long long ull;
template <class T> inline T lowbit (T x) {return x&(-x);}
template <class T> inline T sqr (T x) {return x*x;}
template <class T>
inline bool scan (T &ret) {
    char c;
    int sgn;
    if (c = getchar(), c == EOF) return 0; //EOF
    while (c != '-' && (c < '0' || c > '9') ) c = getchar();
    sgn = (c == '-') ? -1 : 1;
    ret = (c == '-') ? 0 : (c - '0');
    while (c = getchar(), c >= '0' && c <= '9') ret = ret * 10 + (c - '0');
    ret *= sgn;
    return 1;
}
const double pi = 3.14159265358979323846264338327950288L;
using namespace std;
#define mod 1000000007
#define INF 1e18
#define maxn 19
#define maxm 2000005
//-----------------morejarphone--------------------//

/*****************
基本类
定义所有用到的数据结构和基本方法
被工程中所有的模块所引用

磁盘块划分：超级节点|用户块|目录块|文件块|索引块
*****************/
//定义四个块的大小
#define USERSIZE 50
#define DIRSIZE 100
#define FILESIZE 100
#define INDEXSIZE 200
//定义四个块的偏移
#define superNodeSegOffset 0
#define userSegOffset sizeof(superNodeBlock)
#define dirSegOffset (userSegOffset+sizeof(userBlock)*USERSIZE)
#define fileSegOffset (dirSegOffset+sizeof(fileBlock)*FILESIZE)
#define indexSegOffset (fileSegOffset+sizeof(indexBlock)*INDEXSIZE)
/*
文件权限规定
以下规约按照优先级别从高到低排序：
	root目录不可删(包括admin)
	1.权限规定：0(-p)：仅admin可见 1(-r)：所有用户(访客)仅读 2(-rw)：所有用户(访客)可读可编辑，不可删 
		3(-a)：所有用户(访客)可以读编辑删（缺省的默认方法）
	2.admin有一切权限(包括修改任何用户的密码，修改任何文件的权限，修改任何用户的权限)
	3.对于权限为2的文件，其上级文件不可能为3

用户权限规定：
	0：admin权限  1：可读编辑删（缺省的默认权限） 2：可读编辑，不可删 3：只可读 
	这么一来当且仅当：
		用户操作满足用户权限 && 用户权限<=文件权限时，用户可以对文件进行用户操作
*/

struct superNodeBlock {
	//超级节点块
	int root;			//根目录位置
	int emptyUserBlock;	//空用户块
	int emptyDirBlock;	//空目录块
	int _emptyDirBlock; //空目录块最后一块
	int emptyFileBlock;	//空文件块
	int _emptyFileBlock;//空文件块最后一块
	int emptyIndexBlock;//空索引块
	int _emptyIndexBlock;//空索引块最后一块
	//int curUser;		//目前的用户
};


struct userBlock {
	//用户块
	char userName[20];	//用户名
	char userPassword[20];//用户密码
	int userMod;		//用户权限
	bool used;			//是否被占用
};

struct dirBlock {
	//目录块 包含目录或文件的信息
	char dirName[20];	//目录名
	char dirOwner[20];	//目录创建者
	int dirSize;		//如果是目录则为0 如果是文件则表示文件大小
	int dirCreateTime;	//目录创建时间
	int dirChangeTime;	//目录上一次修改时间 
	int type; 			//1:目录 2:文件 3:硬链接 4:软链接
	int textLocation;	//如果是文件 文件内容的位置所在的索引
	int faDirID;		//父目录id
	int sonDirID;		//下一级目录id(孩子)
	int nextDirID;		//下一个目录id(兄弟)  如果没有被占用，那么表示下一个空闲快 -1表示最后一个空块
	//int dirMod;			//目录权限信息
	bool used;			//是否被占用
};

struct fileBlock {
	//文件块 保存文件的文本内容
	char text[1000];
	int nextFileID;		//下一个文件块id 如果没有被占用那么表示下一个空闲块
	bool used;			//是否被占用
};

struct indexBlock {
	int nextIndexID;	//下一个索引块id  如果没有被占用那么表示下一个索引块
	int diskOffset;		//索引指向的地址
	int used;			//是否被占用
};

vector <string> curPath;//当前的绝对路径
int curUserID;			//当前用户
int curDirID;			//当前目录块ID
string disk = "disk";	//磁盘块文件名

void showCurPath ();	//命令行输入指令前显示当前路径用户 格式为root>a/b admin$[space]
//0表示命令行输入指令前显示当前路径用户 格式为root>a/b admin$[space]
//1表示输出当前绝对路径位置 格式为root>a/b[enter]
//根据全局变量global直接按照格式输出
vector <string>& pathPrase (string tarPath);	//用自动机解析路径
//参数表示目标路径
//如果目标路径以root开头表示绝对路径  否则是相对路径
//返回路径上各个节点的目录名构成的向量
//  *表示所有文件 向量中用TOT表示  .表示当前路径 向量中用CUR表示













#endif // COMMON_H_INCLUDED
