#pragma once

#include "Resource.h"
#include "ArdupilotKarpeDlg.h"



class DataSet
{
public:
	DataSet();
	~DataSet();
	double Mpos[NQ][9];
	double x_err[NQ];
	double y_err[NQ];
	double z_err[NQ];
	double vx[NQ];
	double vy[NQ];
	double vz[NQ];
	double x_int_err[NQ];
	double y_int_err[NQ];
	double z_int_err[NQ];
	double theta_ref[NQ]; // 클래스 밖에서 초기화해줘야 한다!
	double theta_err[NQ];
	double dtheta[NQ];
	double theta_int_err[NQ];
	double theta[NQ];
	double Pos_err[NQ];

	double GoalPos[NQ][3];
	double q[NQ][6]; // quadrotor state: pos(012) / angle(345)
	double q_prev[NQ][6]; // quadrotor previous state: pos(012) / angle(345)

	int Mode[NQ]; // 0: takeoff / 1: moving / 2: landing / 3: failsafe

	int CTRL_command[4]; // MAVLINK Control input
	
	void StateCalc(int QuadNum);
	void SetGoal(int QuadNum, double xgoal, double ygoal, double zgoal);
	void CtrlCalc(int QuadNum);
};
