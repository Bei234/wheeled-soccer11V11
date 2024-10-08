// BaseAgent.h: interface for the CBaseAgent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEAGENT_H__7FCEA561_5114_11D1_B242_0000C09B5CBE__INCLUDED_)
#define AFX_BASEAGENT_H__7FCEA561_5114_11D1_B242_0000C09B5CBE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Agentsock.h"
class CMicroClientDoc;
class CBaseAgent : public CAgentSocket
{
	public:
		BOOL Connect();

		CBaseAgent();
		CBaseAgent(CMicroClientDoc* m_pDoc);
		virtual ~CBaseAgent();
		BOOL	ConnectAgent(LPCTSTR lpszTeam, LPCTSTR lpszServer, UINT nPort);
		void	DisconnectAgent(void);

		// communication functions
		virtual void OnReceive(int nErrorCode);
		void SendMsg(CString& strText);
	
		// overridables
		virtual void OnCreatedSocket();
		virtual void OnSentMessage(LPCTSTR message) {};
		virtual void OnReceivedMessage(LPCTSTR message) {};
		
		virtual void OnErrorCreatingSocket() {};
		virtual void OnErrorSendingMessage() {};
		virtual void OnErrorReceivingMessage() {};

		virtual void OnChangedView(int mode, int quality) {};

		// the agents actions
		void	Move(int x, int y);
		void	Turn(int iMoment);
		void	Dash(int iPower);
		void	Kick(int iPower, int iDirection);
		void	Catch(int iDir);
		void	Say(CString csMessage);
		void	ChangeView(int iWidth, int iQuality);
		void	SenseBody();

		void  MoveRel(int x);	// moves relative to the current position

		// attribute fuctions
		BOOL	IsConnected() { return m_bConnected; };
		int	GetSide() { return m_iSide; };
		void	SetSide(int side) { m_iSide = side; };
		int	GetNumber() { return m_iNumber; };
		void	SetNumber(int number) { m_iNumber = number; };
	
	protected:

		CString			m_strTeam;	// name of the team the player is in
		int				m_iSide;		// side on which playing
		int				m_iNumber;	// number of the player
		
		int				m_iPositionX;	// position of the agent x coordinate
		int				m_iPositionY;	// position of the agent y coordinate
		int				m_iDirection;	// view direction of the agent

		BOOL				m_bConnected;	// true, if the agent is currently connected
	public:
		void writetofile();
		void SendCommand();
		CString teamname;
		BOOL ParseObject(LPCTSTR str);
		int m_iTime;
		void ParseSee(LPCTSTR str);
		int m_iPlayMode;
		void ParseInit(LPCTSTR str);
		void ParseMessage(LPCTSTR str);
		CMicroClientDoc* m_pDoc;
		send_to_team teaminfo;

};

#endif // !defined(AFX_BASEAGENT_H__7FCEA561_5114_11D1_B242_0000C09B5CBE__INCLUDED_)
