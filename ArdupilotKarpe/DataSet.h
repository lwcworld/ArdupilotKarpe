#pragma once

#include "Resource.h"
#include "ArdupilotKarpeDlg.h"
#include "mavlink\common\mavlink.h"



class DataSet
{
public:
	DataSet();
	~DataSet();
	static double Mpos[NQ][9];
	static double x_err[NQ];
	static double y_err[NQ];
	static double z_err[NQ];
	static double vx[NQ];
	static double vy[NQ];
	static double vz[NQ];
	static double x_int_err[NQ];
	static double y_int_err[NQ];
	static double z_int_err[NQ];
	static double theta_ref[NQ]; // 클래스 밖에서 초기화해줘야 한다!
	static double theta_err[NQ];
	static double dtheta[NQ];
	static double theta_int_err[NQ];
	static double theta[NQ];
	static double Pos_err[NQ];

	static double GoalPos[NQ][3];
	static double q[NQ][6]; // quadrotor state: pos(012) / angle(345)
	static double q_prev[NQ][6]; // quadrotor previous state: pos(012) / angle(345)

	static int Mode[NQ]; // 0: takeoff / 1: moving / 2: landing / 3: failsafe

	static int CTRL_command[NQ][4]; // MAVLINK Control input
	
	void StateCalc(int QuadNum);
	void SetGoal(int QuadNum, double xgoal, double ygoal, double zgoal);
	void CtrlCalc(int QuadNum);
};
