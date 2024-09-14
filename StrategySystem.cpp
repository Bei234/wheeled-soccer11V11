#include "StdAfx.h"
#include "StrategySystem.h"
#include "iostream"

IMPLEMENT_DYNAMIC(CStrategySystem, CObject)

CStrategySystem::CStrategySystem(int id)
{
	m_OurTeam=id;

	boundRect.SetRect(65,95,965,723);//场地四角

	if(id)
		m_nGameArea=GAME_LEFT;
	else
		m_nGameArea=GAME_RIGHT;
	for(int i=0;i<35;i++)//将机器的左右转速 赋值为零
		command[i]=0;
}



CStrategySystem::~CStrategySystem()
{

}

void CStrategySystem::ReceiveData(Robot1 bal,Robot2 *ho,Robot3 opp)//bal球；ho自家球员；opp对方球员
{
	if(m_nGameArea==GAME_RIGHT)	//我方在右半区，直接接收
	{
		Ball.position=bal.position;
		
		for(int i=0;i<11;i++)//（在右半时给的赋值）
		{
			Home[i].angle=ho[i].angle;//将角度赋值给自家球员
			Home[i].position=ho[i].position;//位置赋值给自家球员
		}	
	}
	else  //我方在左半区，球、球员及对手信息位置关于场地中心对称
	{
		CPoint R(boundRect.right+boundRect.left,boundRect.bottom+boundRect.top);
		Ball.position=R-bal.position;
		
		for(int i=0;i<11;i++)
		{
			Home[i].angle=ho[i].angle+180;//（自家球员的角度转180°）
			Home[i].position=R-ho[i].position;//R-自家球员地址（左右场的一个转换）
		}
	}
}

void CStrategySystem::Action()
{
	for (int i = 0; i < 10; i++)
	{
		Position(i, CPoint(514, 723));
	}
	//int cx=(boundRect.right+boundRect.left)/2;
	//int cy=(boundRect.bottom+boundRect.top)/2;
	////（赛场的中心）
 //   
	////三个前锋
	//Position(0,CPoint(100,100));
	//Position(1,CPoint(100,110));
	//Position(2,CPoint(100,120));

	////三个中锋
	//Position(3,CPoint(100,130));
	//Position(4,CPoint(100,140));
	//Position(5,CPoint(100,150));

	////四个后卫
	//Position(6,CPoint(100,160));
	//Position(7,CPoint(100,170));
	//Position(8,CPoint(100,180));
	//Position(9,CPoint(100,190));

	////守门员
	//CPoint p(boundRect.right-10,Ball.position.y);
	//if(p.y<cy-100)//（p.y为球的纵坐标）当球的纵坐标小于守门员的纵坐标时向上跑
	//	p.y=cy-100;
	//if(p.y>cy+100)//当球的纵坐标大于守门员的纵坐标时对进球方向向下跑
	//	p.y=cy+100;
	////Angle(10,PI/2);
	//Position(10,p);

	////选出踢球者，踢球
	////int kicker=WhichKick();
	//Kick(0);
	//Kick(1);
	//Kick(2);
	//Kick(3);
	//Kick(4);
	//Kick(5);

}

void CStrategySystem::Position(int which, CPoint point)
{
	double desired_angle,theta_e;                 
	int dx, dy, v;

	dx = point.x - Home[which].position.x;
	dy = point.y - Home[which].position.y;


    if(dx==0 && dy==0)
	{
		Velocity(which, 0, 0);
		return;
	}
	else
		desired_angle = atan2(dy,dx);
	
	theta_e = desired_angle - Home[which].angle*PI/180;

	while(theta_e > PI)
		theta_e -= PI*2;
	while(theta_e < -PI)
		theta_e += PI*2;                                      

	//若夹角在（90，180）或（-180，-90）之间，旋转其补角
	//倒退计算出的距离
	if(theta_e < -PI/2){  
		theta_e += PI; 
		v = -127;		
	}
	else if(theta_e > PI/2){  
		theta_e -= PI;		
		v = -127;
	}
	else
		v=127;

	Velocity(which, (int)(v+theta_e*200), int(v-theta_e*200));
}

int CStrategySystem::WhichKick()
{
	int minn=0;
	double minl=pleng(Home[0].position,Ball.position);
	for(int i=1;i<10;i++)
	{
		double t=pleng(Home[i].position,Ball.position);
		if(t<minl)
		{
			minn=i;
			minl=t;
		}
	}

	return minn;
}

void CStrategySystem::Kick(int which)
{
	CPoint tag(boundRect.left,(boundRect.bottom+boundRect.top)/2);

	double distance_e=pleng(Ball.position,Home[which].position);	//足球-小车 距离

	double angle_a=atp(Ball.position,Home[which].position);	//足球-小车 角度
	double angle_b=atp(tag,Ball.position);	//目标-足球 角度
	double angle_t=angle_b-angle_a;	//目标-足球 足球-小车 夹角

	while(angle_t > PI)
		angle_t -= PI*2;
	while(angle_t < -PI)
		angle_t += PI*2;                                      

	double l;
	if(angle_t>PI/2)
	{
		angle_b+=PI/2;
		l=20;
	}
	else if(angle_t<-PI/2)
	{
		angle_b-=PI/2;
		l=20;
	}
	else
	{
		angle_b+=PI;
		l=distance_e/cos(angle_t)/2;	//（中位线 交 目标反向延长线）的焦点 距足球距离
	}
	
	tag.x=(long)(Ball.position.x+cos(angle_b)*l);
	tag.y=(long)(Ball.position.y+sin(angle_b)*l);
	Position(which,tag);
}

void CStrategySystem::Velocity(int which, int vL, int vR)
{
	//检查变量范围
	if(vL < -127)	vL = -127;
	if(vL > 127)	vL = 127;

	if(vR < -127)	vR = -127;
	if(vR > 127)	vR = 127;

	command[which*3+2] = vL;
	command[which*3+3] = vR;
	command[which*3+4] = C_GO;
}

void CStrategySystem::Angle(int which, double desired_angle)
{
	Robot2 tr;
	double theta_e;
	
	tr=Home[which];

	//计算目标角与机器人方向角的差
	theta_e = desired_angle - tr.angle*PI/180;;
	FrontRad(theta_e);

	int va=((long)((theta_e/rad_speed/2)+.5));

	Velocity(which, va, -va);
}

bool FrontRad(double &a)	//得到正方向（true:正向 false:反向）以及所需转动角度
{
	a=AbsRad(a);
	if(a>PI/2)
	{
		a-=PI;
		return false;
	}
	if(a<-PI/2)
	{
		a+=PI;
		return false;
	}
	return true;
}

double AbsRad(double a)	//弧度制角度规范化
{
	if(a>100 || a<-100)
		return 0;
	while(a>PI)a-=2*PI;
	while(a<-PI)a+=2*PI;
	return a;
}                                                                                    