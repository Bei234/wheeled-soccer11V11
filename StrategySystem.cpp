#include "StdAfx.h"
#include "StrategySystem.h"
#include "iostream"

IMPLEMENT_DYNAMIC(CStrategySystem, CObject)

CStrategySystem::CStrategySystem(int id)
{
	m_OurTeam=id;

	boundRect.SetRect(65,95,965,723);//�����Ľ�

	if(id)
		m_nGameArea=GAME_LEFT;
	else
		m_nGameArea=GAME_RIGHT;
	for(int i=0;i<35;i++)//������������ת�� ��ֵΪ��
		command[i]=0;
}



CStrategySystem::~CStrategySystem()
{

}

void CStrategySystem::ReceiveData(Robot1 bal,Robot2 *ho,Robot3 opp)//bal��ho�Լ���Ա��opp�Է���Ա
{
	if(m_nGameArea==GAME_RIGHT)	//�ҷ����Ұ�����ֱ�ӽ���
	{
		Ball.position=bal.position;
		
		for(int i=0;i<11;i++)//�����Ұ�ʱ���ĸ�ֵ��
		{
			Home[i].angle=ho[i].angle;//���Ƕȸ�ֵ���Լ���Ա
			Home[i].position=ho[i].position;//λ�ø�ֵ���Լ���Ա
		}	
	}
	else  //�ҷ��������������Ա��������Ϣλ�ù��ڳ������ĶԳ�
	{
		CPoint R(boundRect.right+boundRect.left,boundRect.bottom+boundRect.top);
		Ball.position=R-bal.position;
		
		for(int i=0;i<11;i++)
		{
			Home[i].angle=ho[i].angle+180;//���Լ���Ա�ĽǶ�ת180�㣩
			Home[i].position=R-ho[i].position;//R-�Լ���Ա��ַ�����ҳ���һ��ת����
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
	////�����������ģ�
 //   
	////����ǰ��
	//Position(0,CPoint(100,100));
	//Position(1,CPoint(100,110));
	//Position(2,CPoint(100,120));

	////�����з�
	//Position(3,CPoint(100,130));
	//Position(4,CPoint(100,140));
	//Position(5,CPoint(100,150));

	////�ĸ�����
	//Position(6,CPoint(100,160));
	//Position(7,CPoint(100,170));
	//Position(8,CPoint(100,180));
	//Position(9,CPoint(100,190));

	////����Ա
	//CPoint p(boundRect.right-10,Ball.position.y);
	//if(p.y<cy-100)//��p.yΪ��������꣩�����������С������Ա��������ʱ������
	//	p.y=cy-100;
	//if(p.y>cy+100)//������������������Ա��������ʱ�Խ�����������
	//	p.y=cy+100;
	////Angle(10,PI/2);
	//Position(10,p);

	////ѡ�������ߣ�����
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

	//���н��ڣ�90��180����-180��-90��֮�䣬��ת�䲹��
	//���˼�����ľ���
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

	double distance_e=pleng(Ball.position,Home[which].position);	//����-С�� ����

	double angle_a=atp(Ball.position,Home[which].position);	//����-С�� �Ƕ�
	double angle_b=atp(tag,Ball.position);	//Ŀ��-���� �Ƕ�
	double angle_t=angle_b-angle_a;	//Ŀ��-���� ����-С�� �н�

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
		l=distance_e/cos(angle_t)/2;	//����λ�� �� Ŀ�귴���ӳ��ߣ��Ľ��� ���������
	}
	
	tag.x=(long)(Ball.position.x+cos(angle_b)*l);
	tag.y=(long)(Ball.position.y+sin(angle_b)*l);
	Position(which,tag);
}

void CStrategySystem::Velocity(int which, int vL, int vR)
{
	//��������Χ
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

	//����Ŀ���������˷���ǵĲ�
	theta_e = desired_angle - tr.angle*PI/180;;
	FrontRad(theta_e);

	int va=((long)((theta_e/rad_speed/2)+.5));

	Velocity(which, va, -va);
}

bool FrontRad(double &a)	//�õ�������true:���� false:�����Լ�����ת���Ƕ�
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

double AbsRad(double a)	//�����ƽǶȹ淶��
{
	if(a>100 || a<-100)
		return 0;
	while(a>PI)a-=2*PI;
	while(a<-PI)a+=2*PI;
	return a;
}                                                                                    