#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;
#include "stdlib.h"

#define Infinity 65535
#define MAX 420

//**********车站类(顶点类）***********//
class Line;
class Station
{
public:
    string station_name;//车站名
    int number;//标记
    Line * link;//邻接路线指针

    Station();//无参构造函数
    //Station(string sn, int n);//含参构造函数
    ~Station() {}; //析构函数
    //void ShowStation();//显示车站信息
    void ShowStation()
    {
        cout << "站点：" << station_name;
    }//显示车站信息
    Line * FindLine(string rn);
    /*新写的部分*/
    Line *FindLineDest(int sta_dest);
};

//**********路线类(边结点类）***********//
class Line
{
public:
    int type;//路线类型
    string line_name;//路线名称
    string to_station;//该路线的终点车站名
    int dest;//终点车站编号
    float distance;//距离
    float time;//时间
    float fee;//费用
    Station * st;//该路线的终点车站指针
    Line * next;//邻接路线

    Line();//构造函数
    //Line(Station* &s, Line* &r){};//含参构造函数
    //bool Initialize(string rn, string ns, int d, int t, float f);//初始化函数                                    ///
    ~Line() {}; //析构函数
    //void ShowLine();//显示路线信息
    void operator = (const Line* &l);
    Line *FindLineDest(int sta_dest)
    {
        Line * p = next;
        while(p!=NULL)
        {
            if( p->dest == sta_dest )
                return p;
            p = p->next;
        }
        return NULL;
    }
};

Station::Station()
{
    link=(Line*) malloc(sizeof(Line));
}
Line::Line()
{
    st = (Station*) malloc(sizeof(Station));
    next=(Line*) malloc(sizeof(Line));
//	st=new Station;
//	next=new Line;
}

Line * Station::FindLine(string rn)
{
    Line * p = link;
    while(p!=NULL)
    {
        if( p->line_name == rn )
            return p;
        p = p->next;
    }
    return NULL;
}

Line* Station::FindLineDest(int sta_dest)
{
    Line * p = link;
    while(p!=NULL)
    {
        if( p->dest == sta_dest )
            return p;
        p = p->next;
    }
    return NULL;
}



void Line::operator = (const Line* &l)
{
    type = l->type ;
    line_name = l->line_name ;
    to_station = l->to_station ;
    dest = l->dest ;
    distance = l->distance ;
    time = l->time ;
    fee = l->fee ;
    st = l->st ;
    next = l->next ;
}

//**********线路类(邻接表类）***********//
//线路+起始站
class Originsta
{
public:
    string route_name;//所在线路名称
    string originsta_name;
    Station * originsta;
    Line * originline;
    Originsta * nextorigin;

    Originsta();
    void ShowRoute()
    {
        string rn = route_name;
        Station *psta = originsta;
        Line *pline = originline;
        cout << "路线：" << rn << "\n途径站点：" ;
        while( psta != NULL )
        {
            cout << psta->station_name;
            psta = pline->st;
            if ( psta != NULL )
            {
                cout << " -> " ;
                pline = psta->FindLine(rn);
            }
            else
                cout << "\n" ;
        }
    }//显示整条路线
};

Originsta::Originsta()
{
    originsta=(Station*) malloc(sizeof(Station));
    originline=(Line*) malloc(sizeof(Line));
//	nextorigin=new Originsta;
    nextorigin = (Originsta* )malloc(sizeof(Originsta));
}

class Originsta_list
{
public:
    Originsta * head;

    //函数
    Originsta_list()
    {
        head = new Originsta;
    }
};

//**********交通网络(图部分）***********//

//dijkstra算法中用到的
struct Queue
{
    int No;
    float Cost;//距离/时间/费用
};
//记录路线
struct WayNode
{
    string cur_sta;//当前站点//from_sta, to_sta;
    int cur;//当前站点编号//from, to;
    string route_name;
    WayNode *next;
};

class Trans_network
{
private:
    Station* station[MAX];
    int  Cur_sta_num;//当前站点总数
    int  Cur_line_num;//当前路线总数
    Originsta_list orilist;

    //Dijkstra算法表
    //int Dist[MAX];//指定源点到节点i的最短距离/最短时间
    float Dist[MAX];//指定源点到节点i的最少费用
    int Path[MAX];//指定源点到节点i路径中，节点i的前驱节点序号
    Queue DQue[MAX];
    WayNode *way;

public:
    Trans_network();//构造函数                                                                   //
    //Trans_network(int sta_num, int rou_num);//含参构造函数
    ~Trans_network();//析构函数
    void Initialize();//初始化交通网络                                                                            ///
    void AddStation();//增加一个车站
    void AddLine();//增加一条路线
    void Delete();//删除操作
    void Create();//创建orilist
    void DeleteStation(string sn);//删除一个车站
    bool DeleteLine(string sn1, string sn2);//删除一条路线
    Station * FindStation(string sn);//寻找站点
    int FindNumber(string sn);//寻找站点编号，sn为车站名
    void Check();
    void CheckStation();
    void CheckRoute();
    bool Save();
    /*新写的部分*/
    int  GetDistance(string s1, string s2);//获取两相邻站点之间的距离，如果不相邻，返回无穷
    int  GetTime(string s1, string s2);//获取两相邻站点之间的时间，如果不相邻，返回无穷
    float  GetFee(string s1, string s2);//获取两相邻站点之间的费用，如果不相邻，返回无穷

    void Shortest(string s1, string s2);//最短距离，s1为始发站，s2为终点站
    void Fastest(string s1, string s2);//最短时间，s1为始发站，s2为终点站
    void Cheapest(string s1, string s2);//最少费用，s1为始发站，s2为终点站
    void MinChange(string s1, string s2);//最小换乘，s1为始发站，s2为终点站
    //dijkstra算法
    void QueInsert(int &num, int no, float cost);//插入容器
    Queue QueGetMin(int &num);
    void QueKeepMin(int &num,const int k);
    void CreateWay(int v);//创建dijkstra算法求出的路线
    void PrintWay(int v);
};

Trans_network::Trans_network()
{
    int i;
    for (i=0; i<MAX; i++)
    {
//		station[i]=new Station;                                                                      //
    }
    Cur_sta_num=0;
    Cur_line_num=0;
}

Trans_network::~Trans_network()
{
    for( int i=0 ; i < MAX ; i++ )
    {
        Line *p = station[i]->link;
        while( p!=NULL )
        {
            station[i]->link = p->next;
            delete p;
            p = station[i]->link;
        }
    }
    delete station[MAX];
}

void Trans_network::Create()
{
    string r,s;
    int k;
    fstream f("transportation4.txt", ios::in);
    Originsta* p=new Originsta;
    orilist.head=p;
    f>>r;
    while (r!="end")
    {
        f>>s;
        while (s!="#")
        {
            p=new Originsta;
            p->route_name = r;
            p->originsta_name = s;
            p->originsta = FindStation(s);
            Station * s1=p->originsta ;
            Line *l=s1->link;
            k=0;
            while (l!=NULL)
            {
                if (l->line_name == r&&k==1)
                {
                    break;
                }
                else if (l->line_name ==r)
                {
                    k++;
                }
                l->next ;
            }
            p->originline = l;
            Originsta* pp=new Originsta;
            pp=p->nextorigin ;
            p=pp;
            f>>s;
        }
        f>>r;
    }
}


void Trans_network::AddStation()
{
    Station * s=new Station;
    int k;
    cout << "您将要建立一个新的站点。请按照指示输入新车站！"<<endl;
    cout << "请输入车站的名称：";
    cin>>s->station_name;
    Cur_sta_num++;
    station[Cur_sta_num]=s;
    cout<< "车站创建成功！"<<endl;
    cout<< "是否保存修改？1：是，0：否"<<endl;
    cin>>k;
    if (k)
    {
        if (Save())
        {
            cout<< "保存成功！"<<endl;
        }
        else
        {
            cout<< "保存失败！"<<endl;
        }
    }
}
Station * Trans_network::FindStation(string sn)
{
    int i=1;
    while (station[i]!=NULL)
    {
        if (station[i]->station_name==sn)
        {
            return station[i];
        }
        else if (i>Cur_sta_num)
        {
            return NULL;
        }
        i++;
    }
}
int Trans_network::FindNumber(string sn)
{
    int i=1;
    while (station[i]!=NULL)
    {
        if (station[i]->station_name==sn)
        {
            return station[i]->number;
        }
        else if (i>Cur_sta_num)
        {
            return 0;
        }
        i++;
    }
}
/*Station * Trans_network::FindLine(string ln)
{
	Station * p;
	Line * t;
	int i=1;
	while (station[i]!=NULL)
	{
		p=station[i];
		t = p->link;
		while (t!=NULL)
		{
			if (t->line_name==ln)
			{
				return
			}
		}
	}
}*/

void Trans_network::Check()
{
    int c;
    string s1,s2;
    while (1)
    {
        system("cls");
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "     		       欢迎使用北京交通路线查询系统\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "		■■           请选择您要执行的操作         ■■\n";
        cout << "		■■              1.查询最短距离            ■■\n";
        cout << "		■■              2.查询最短时间            ■■\n";
        cout << "		■■              3.查询最少费用            ■■\n";
        cout << "		■■              4.查询最少换乘            ■■\n";
        cout << "		■■              5.返回上一级              ■■\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "            		选项：";
        cin >> c;
        system("cls");
        cout << "请输入起点车站和终点车站！\n" ;
        cout << "起点车站：" ;
        cin >> s1;
        cout << "终点车站：" ;
        cin >> s2;
        if (c==1)
        {
            Shortest(s1,s2);
            system("pause");
        }
        else if (c==2)
        {
            Fastest(s1,s2);
            system("pause");
        }
        else if (c==3)
        {
            Cheapest(s1,s2);
            system("pause");
        }
        else if (c==4)
        {
            MinChange(s1,s2);
            system("pause");
        }
        else if (c==5)
            break;
        else
        {
            cout << "您没有选择任何一个选项！\n";
            system("pause");
        }
    }
    system("cls");
}

void Trans_network::CheckStation()
{
    cout << "请输入您需要查询的站点名：" ;
    string sn;
    cin >> sn;
    int i=0;
    Station * psta = FindStation(sn);
    if( psta != NULL)
        psta->ShowStation();
    else
        cout << "没有找到该站点。\n" ;
}

void Trans_network::CheckRoute()
{
    cout << "请输入您需要查询的线路名：" ;
    string rn;
    cin >> rn;
    int k=0;
    Originsta *p = orilist.head;
    Station *psta = p->originsta;
    while( p != NULL)
    {
        p = p->nextorigin;
        if( p->route_name == rn )
        {
            k = 1;
            p->ShowRoute();//show route
            break;
        }
    }
    if( k==0 )
        cout << "没有找到该路线。\n" ;
}

void Trans_network::Delete()
{
    int c;
    string s,s1,s2;
    system("cls");
    cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
    cout << "     		       欢迎使用北京交通路线删除系统\n";
    cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
    cout << "		■■           请选择您要执行的操作         ■■\n";
    cout << "		■■              1.删除一个站点            ■■\n";
    cout << "		■■              2.删除一条路线            ■■\n";
    cout << "		■■              3.返回上一级              ■■\n";
    cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
    cout << "            		选项：";
    cin >> c;
    system("cls");
    if (c==1)
    {
        cout << "请输入将要删除的车站名称：";
        cin >> s;
        DeleteStation(s);
        system("cls");
    }
    else if (c==2)
    {
        cout << "请输入将要删除的路线所连接的车站名称"<<endl;
        cout << "车站1：";
        cin >> s1;
        cout << "车站2：";
        cin >> s2;
        DeleteLine(s1,s2);
        system("cls");
    }
    else if (c==3)
    {
        system("cls");
    }
    else
    {
        cout << "您没有选择任何一个选项！\n";
        system("pause");
    }
}

void Trans_network::DeleteStation(string sn)
{
    Station* p;
    Line *t,*tt;
    int i;
    while (station[i]!=NULL)
    {
        p=station[i];
        if (p->station_name == sn)
        {
            t=p->link ;
            while (t!=NULL)
            {
                tt=t;
                t=t->next ;
                delete tt;
            }
            delete p;
            break;
        }
        i++;
    }
}

void Trans_network::AddLine()
{
    Line* ll1=new Line;
    Line* ll2=new Line;
    Line *l,*l1,*l2;
    Station *p1,*p2;
    int k;
    string s1,s2;
    cout << "您将要建立一条新的路线。请按照指示输入新路线！"<<endl;
    cout << "请输入路线的名称：";
    cin >> l->line_name;
    cout << "请设置该路线的类型：1公交线 2地铁线";
flag:
    cin >> k;
    if (k==1||k==2)
    {
        l->type=k;
        cout << "请输入该路线连通的车站：" << endl;
        cout << "车站1：" ;
        cin>>s1;
        cout << "车站2：" ;
        cin>>s2;
        cout << "请依次输入该路线的距离、耗时和费用：";
        cin>>l->distance>>l->time>>l->fee;
        p1=FindStation(s1);
        l1=p1->link;
        while (l1!=NULL)
        {
            if (l1->next==NULL)
            {
                ll1=l;
                ll1->to_station = s2;
                ll1->dest = FindNumber(s2);
                l1->next=ll1;
                ll1->next=NULL;
                break;
            }
            l1=l1->next;
        }
        p2=FindStation(s2);
        l2=p2->link;
        while (l2!=NULL)
        {
            if (l2->next==NULL)
            {
                ll2=l;
                ll2->to_station = s1;
                ll2->dest = FindNumber(s1);
                l2->next=ll2;
                ll2->next=NULL;
                break;
            }
            l2=l2->next;
        }
        cout << "建立成功！" <<endl;
        cout<< "是否保存修改？1：是，0：否"<<endl;
        cin>>k;
        if (k)
        {
            if (Save())
            {
                cout<< "保存成功！"<<endl;
            }
            else
            {
                cout<< "保存失败！"<<endl;
            }
        }
    }
    else
    {
        cout << "输入错误！请重新输入。";
        goto flag;
    }
}

bool Trans_network::DeleteLine(string sn1, string sn2)
{
    int i=1;
    Station* p;
    Line *t,*tt;
    while(station[i]!=NULL)
    {
        p=station[i];
        if (p->station_name == sn1)
        {
            t=p->link ;
            if (t->to_station == sn2)
            {
                p->link = t->next ;
                delete t;
                return true;
            }
            else
            {
                while (t->next!=NULL)
                {
                    tt=t->next ;
                    if (tt->to_station == sn2)
                    {
                        t->next = tt->next ;
                        delete tt;
                        return true;
                    }
                    t=t->next ;
                    tt=tt->next ;
                }
                return false;
            }
        }
        else if (p->station_name == sn2)
        {
            t=p->link ;
            if (t->to_station == sn1)
            {
                p->link = t->next ;
                delete t;
                return true;
            }
            else
            {
                while (t->next!=NULL)
                {
                    tt=t->next ;
                    if (tt->to_station == sn1)
                    {
                        t->next = tt->next ;
                        delete tt;
                        return true;
                    }
                    t=t->next ;
                    tt=tt->next ;
                }
                return false;
            }
        }
        return false;
    }
}

void Trans_network::Initialize()
{
    int i=0,num=1;
    float sn,de,di,ti,fe,ty;
    string s,l,ts;
    fstream f("transportation2.txt", ios::in);
    f>>s;
    while (s!="end")
    {
        Station *sta=new Station;
        sta->station_name = s;
        f>>sta->number;
        num=sta->number;
        Line * p1=new Line;
        f>>p1->line_name;
        f>>p1->type;
        f>>p1->dest;
        f>>p1->to_station;
        f>>p1->distance;
        f>>p1->time;
        f>>p1->fee;
        sta->link = p1;
        Line * p=sta->link;
        f>>l;
        while (l!="#")
        {
            Line *line1=new Line;
            line1->line_name=l;
            f>>line1->type;
            f>>line1->dest;
            f>>line1->to_station;
            f>>line1->distance;
            f>>line1->time;
            f>>line1->fee;
            line1->st=FindStation(line1->to_station);
            p->next=line1;
            p=p->next;
            Cur_line_num++;
            f>>l;
        }
        station[num]=new Station;
        station[num]=sta;
        Cur_sta_num++;
        f>>s;
    }
}

bool Trans_network::Save()
{
    int i=1;
    Station * p;
    Line * l;
    fstream f("transportation2.txt",ios::out);
    if(!f)
        return false;
    while (station[i]!=NULL)
    {
        p=station[i];
        f<<p->station_name<<" "<<p->number<<" ";
        l=p->link;
        while (l!=NULL)
        {
            f<<l->line_name<<" "<<l->type<<" "<<l->dest<<" "<<l->to_station<<" "<<l->distance<<" "<<l->time<<" "<<l->fee<<" ";
            l=l->next;
        }
        f<<"#/n";
        i++;
    }
    f<<"end";
    f.close();
    return true;
}

int Trans_network::GetDistance(string s1, string s2)
{
    Station *psta;
    Line *pline;
    psta = FindStation(s1);
    pline = psta->FindLine(s2);
    if(pline==NULL)
        return pline->distance;
    else
        return Infinity;
}

int Trans_network::GetTime(string s1, string s2)
{
    Station *psta;
    Line *pline;
    psta = FindStation(s1);
    pline = psta->FindLine(s2);
    if(pline==NULL)
        return pline->time;
    else
        return Infinity;
}

float Trans_network::GetFee(string s1, string s2)
{
    Station *psta;
    Line *pline;
    psta = FindStation(s1);
    pline = psta->FindLine(s2);
    if(pline==NULL)
        return pline->fee;
    else
        return Infinity;
}

void Trans_network::QueInsert(int &num,int no,float cost)
{
    num += 1;
    DQue[num].No = no;
    DQue[num].Cost = cost;
    int i=num;
    while( i>1 && DQue[i/2].Cost>DQue[i].Cost )
    {
        Queue temp = DQue[i];
        DQue[i] = DQue[i/2];
        DQue[i/2] = temp;
        i = i/2;
    }
}

Queue Trans_network::QueGetMin(int &num)
{
    if( num<1 )
        return DQue[0];
    Queue min = DQue[1];
    DQue[1] = DQue[num];
    num -= 1;
    QueKeepMin(num,1);
    return min;
}

void Trans_network::QueKeepMin(int &num,const int k)
{
    int l=2*k, r=2*k+1;
    int smallest = k;
    if( l<=num && DQue[l].Cost<DQue[k].Cost )
        smallest = l;
    if( r<=num && DQue[r].Cost<DQue[smallest].Cost )
        smallest = r;
    if( smallest != k )
    {
        Queue temp = DQue[smallest];
        DQue[smallest] = DQue[k];
        DQue[k] = temp;
        QueKeepMin(num,smallest);
    }
}

void Trans_network::CreateWay(int v)
{
    if( Path[v]==v )
    {
        WayNode *p = new WayNode;
        p->cur_sta = station[v]->station_name;
        way->next = p;
        way = p;
    }
    else
    {
        CreateWay(Path[v]);
        WayNode *p = new WayNode;
        p->cur_sta = station[v]->station_name;
        way->next = p;
        way = p;
    }
}

void Trans_network::PrintWay(int v)
{
    way = new WayNode;
    WayNode *way_start = way;
    CreateWay(v);
    int change = 0, time=0;
    float money = 0;
    WayNode *p = way_start->next;
    string rn, sn;

    //添加路线名称
    while( p!=NULL )
    {
        sn = p->cur_sta;
        rn = FindStation(sn)->FindLineDest(p->next->cur)->line_name;
        p->route_name = rn;
        p = p->next;
    }
    //回到起点
    p = way_start->next;

    //开始输出
    cout << "在" << p->cur_sta << "乘坐" << p->route_name ;
    while( p->next->next != NULL )
    {
        if( p->route_name == p->next->route_name )
        {
            //打印路线
            p = p->next;
            cout << "经过" << p->cur_sta ;
            //记录信息
            money += GetFee(p->cur_sta,p->next->cur_sta);
            time += GetTime(p->cur_sta,p->next->cur_sta);
        }
        else if( p->route_name != p->next->route_name )
        {
            //打印路线
            p = p->next;
            cout << "至" << p->cur_sta << "\n在" << p->cur_sta << "换乘" << p->route_name;
            //记录信息
            change++;
            money += GetFee(p->cur_sta,p->next->cur_sta);
            time += GetTime(p->cur_sta,p->next->cur_sta);
        }
    }
    p = p->next;
    cout << "到达目的地" << p->cur_sta << endl ;
    cout << "总费用：" << money << "元\n大约用时：" << time << "分钟\n换乘次数：" << change << "次\n";
}

void Trans_network::Shortest(string s1, string s2)
{
    int vstart, vend, i=0, v, num=0;
    Line *p;
    Queue q;
    vstart = FindNumber(s1);
    vend = FindNumber(s2);
    v = vstart;

    //表初始化
    for( i=0; i<Cur_sta_num; i++ )
    {
        Dist[i] = Infinity;
        Path[i] = 0;
    }
    Dist[vstart] = 0;
    Path[vstart] = 0;

    //dijkstra算法
    for( i=0 ; i<Cur_sta_num ; i++ )
    {
        p = station[v]->link;
        while( p != NULL )
        {
            if( Dist[v]+p->distance < Dist[p->dest] )
            {
                Dist[p->dest] = Dist[v]+p->distance;
                Path[p->dest] = v;
                //存入队列
                QueInsert(num,p->dest,Dist[p->dest]);
            }
            p = p->next;
        }
        //找出min
        q = QueGetMin(num);
        v = q.No;
    }
    cout << "从 " << s1 << " 开始到 " << s2 << " 的距离最短路径为：" << endl;
    PrintWay(vend);
}

void Trans_network::Fastest(string s1, string s2)
{
    int vstart, vend, i=0, v, num=0;
    Line *p;
    Queue q;
    vstart = FindNumber(s1);
    vend = FindNumber(s2);
    v = vstart;

    //表初始化
    for( i=0; i<Cur_sta_num; i++ )
    {
        Dist[i] = Infinity;
        Path[i] = 0;
    }
    Dist[vstart] = 0;
    Path[vstart] = 0;

    //dijkstra算法
    for( i=0 ; i<Cur_sta_num ; i++ )
    {
        p = station[v]->link;
        while( p != NULL )
        {
            if( Dist[v]+p->time < Dist[p->dest] )
            {
                Dist[p->dest] = Dist[v]+p->time;
                Path[p->dest] = v;
                //存入队列
                QueInsert(num,p->dest,Dist[p->dest]);
            }
            p = p->next;
        }
        //找出min
        q = QueGetMin(num);
        v = q.No;
    }
    cout << "从 " << s1 << " 开始到 " << s2 << " 的用时最少的路径为：" << endl;
    PrintWay(vend);
}

void Trans_network::Cheapest(string s1, string s2)
{
    int vstart, vend, i=0, v, num=0;
    Line *p;
    Queue q;
    vstart = FindNumber(s1);
    vend = FindNumber(s2);
    v = vstart;

    //表初始化
    for( i=0; i<Cur_sta_num; i++ )
    {
        Dist[i] = Infinity;
        Path[i] = 0;
    }
    Dist[vstart] = 0;
    Path[vstart] = 0;

    //dijkstra算法
    for( i=0 ; i<Cur_sta_num ; i++ )
    {
        p = station[v]->link;
        while( p != NULL )
        {
            if( Dist[v]+p->fee < Dist[p->dest] )
            {
                Dist[p->dest] = Dist[v]+p->fee;
                Path[p->dest] = v;
                //存入队列
                QueInsert(num,p->dest,Dist[p->dest]);
            }
            p = p->next;
        }
        //找出min
        q = QueGetMin(num);
        v = q.No;
    }
    cout << "从 " << s1 << " 开始到 " << s2 << " 的费用最少的路径为：" << endl;
    PrintWay(vend);
}

void Trans_network::MinChange(string s1, string s2)
{
    //未完待续。。。
}

//**********用户部分***********//
void Manager(Trans_network &tmap)
{
    int c;
    while(1)
    {
        system("cls");
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "		                     管理员\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "		■■      请选择选项对交通网络进行操作      ■■\n";
        cout << "		■■              1.建立新站点              ■■\n";
        cout << "		■■              2.建立新路线              ■■\n";
        cout << "		■■              3.删除站点或路线          ■■\n";
        cout << "		■■              4.查询路线                ■■\n";
        cout << "		■■              5.查询站点                ■■\n";
        cout << "		■■              6.保存当前网络            ■■\n";
        cout << "		■■              7.返回上一级              ■■\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "            选项：";
        cin >> c;
        system("cls");
        if(c == 1)
        {
            tmap.AddStation();
            system("pause");
        }
        else if(c == 2)
        {
            tmap.AddLine();
            system("pause");
        }
        else if(c == 3)
        {
            tmap.Delete();
            system("pause");
        }
        else if(c == 4)
        {
            tmap.CheckRoute() ;
            system("pause");
        }
        else if(c == 5)
        {
            tmap.CheckStation() ;
            system("pause");
        }
        else if(c == 6)
        {
            if (tmap.Save())
            {
                cout << "保存成功！"<<endl;
                system("pause");
            }
            else
            {
                cout << "保存失败..."<<endl;
                system("pause");
            }
        }
        else if(c == 7)
        {
            break;
        }
        else
        {
            cout << "您没有选择任何一个选项！\n";
            system("pause");
        }
    }
    system("cls");
}

void User(Trans_network &tmap)
{
    int c1,c2;
    while(1)
    {
        system("cls");
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "     		       欢迎使用北京公交路线查询系统\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "		■■             请选择您需要的服务         ■■\n";
        cout << "		■■              1.查询路线                ■■\n";
        cout << "		■■              2.查询站点                ■■\n";
        cout << "		■■              3.查询最优方案            ■■\n";
        cout << "		■■              4.返回上一级              ■■\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "            		选项：";
        cin >> c1;
        system("cls");
        if(c1 == 1)
        {
            tmap.CheckRoute();
            system("pause");
        }
        else if(c1 == 2)
        {
            tmap.CheckStation();
            system("pause");
        }
        else if(c1 == 3)
        {
            tmap.Check();
            system("pause");
        }
        else if(c1 == 4)
            break;
        else
        {
            cout << "您没有选择任何一个选项！\n";
            system("pause");
        }
    }
    system("cls");
}

//***********主函数************//
int main()
{
    string pw,PASSWORD="123456";
    Trans_network tmap;
    int i,c;
    tmap.Initialize();
    tmap.Create();
    while(1)
    {
        system("cls");
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "          			欢迎使用北京公交路线查询系统\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "		■■             请选择您的身份:            ■■\n";
        cout << "		■■                1.公交管理系统          ■■\n";
        cout << "		■■                2.公交乘客系统          ■■\n";
        cout << "		■■■■■■■■■■■■■■■■■■■■■■■■\n";
        cout << "          				选项：";
        cin >> c;
        if(c == 1)
        {
            cout << "\n\n";
            cout << "请输入管理员密码：";
            cin >> pw;
            if(pw != PASSWORD)
            {
                cout << "密码错误，返回主界面……\n";
                system("pause");
            }
            else
            {
                system("cls");
                Manager(tmap);
            }
        }
        else if(c == 2)
        {
            system("cls");
            User(tmap);
        }
        else
        {
            cout << "您没有选择任何一个选项！\n";
            system("pause");
        }
    }
    return 0;
}

