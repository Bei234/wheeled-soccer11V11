// StrategySystem.h
#include "general.h"

#ifndef _INSIDE_VISUAL_CPP_STRATEGYSYSTEM
#define _INSIDE_VISUAL_CPP_STRATEGYSYSTEM
//////////////////////////////////////////////////NEW/////////////////////////////////////////////////////

class CStrategySystem : public CObject
{
    DECLARE_DYNAMIC(CStrategySystem)
public:
	void Action();
    ~CStrategySystem();
	CStrategySystem(int id);
    void ReceiveData(Robot1 bal,Robot2 *ho,Robot3 opp);
private:
	int WhichKick();
	void Kick(int which);
	void Position(int which, CPoint point);
	void Velocity(int which, int vL, int vR);
	void Angle(int which, double desired_angle);
	CRect	boundRect;     
	int	m_nGameArea;
	int	m_OurTeam;  

	Robot1 Ball;
	Robot2 Home[11];
	Robot3 opponent[11];
public:
	int command[35]; 
};
bool FrontRad(double &a);
double AbsRad(double a);
const double rad_speed=2.1837*0.02/11;
#endif // _INSIDE_VISUAL_CPP_STRATEGYSYSTEM
