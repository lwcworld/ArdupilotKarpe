#include "stdafx.h"
#include "DataSet.h"

double DataSet::Mpos[NQ][9];
double DataSet::x_err[NQ];
double DataSet::y_err[NQ];
double DataSet::z_err[NQ];
double DataSet::vx[NQ];
double DataSet::vy[NQ];
double DataSet::vz[NQ];
double DataSet::x_int_err[NQ];
double DataSet::y_int_err[NQ];
double DataSet::z_int_err[NQ];
double DataSet::theta_ref[NQ]; // 클래스 밖에서 초기화해줘야 한다!
double DataSet::theta_err[NQ];
double DataSet::dtheta[NQ];
double DataSet::theta_int_err[NQ];
double DataSet::theta[NQ];
double DataSet::Pos_err[NQ];

double DataSet::GoalPos[NQ][3];
double DataSet::q[NQ][6]; // quadrotor state: pos(012) / angle(345)
double DataSet::q_prev[NQ][6]; // quadrotor previous state: pos(012) / angle(345)

int DataSet::Mode[NQ]; // 0: takeoff / 1: moving / 2: landing / 3: failsafe

int DataSet::CTRL_command[NQ][4]; // MAVLINK Control input

DataSet::DataSet()
{
}


DataSet::~DataSet()
{
}

// DataSet::Mpos[x][i] : M1 pos(012) / M2 pos(345) / M3 pos(678) / M1 angle (9 10 11)
void DataSet::StateCalc(int QuadNum)
{
	DataSet::q[QuadNum][0] = (DataSet::Mpos[QuadNum][0] + DataSet::Mpos[QuadNum][3]) / 2000;
	DataSet::q[QuadNum][1] = (DataSet::Mpos[QuadNum][1] + DataSet::Mpos[QuadNum][4])  / 2000;
	DataSet::q[QuadNum][2] = (DataSet::Mpos[QuadNum][2] + DataSet::Mpos[QuadNum][5]) / 2000; // quadrotor position 1000/2

	DataSet::q[QuadNum][3] = DataSet::Mpos[QuadNum][9] / 1000;
	DataSet::q[QuadNum][4] = DataSet::Mpos[QuadNum][10] / 1000;
	DataSet::q[QuadNum][5] = DataSet::Mpos[QuadNum][11] / 1000; // quadrotor angle

	while (DataSet::q[QuadNum][5] > 180 || DataSet::q[QuadNum][5] < -180){
		if (DataSet::q[QuadNum][5] < -180) DataSet::q[QuadNum][5] = DataSet::q[QuadNum][5] + 360;
		if (DataSet::q[QuadNum][5] > 180) DataSet::q[QuadNum][5] = DataSet::q[QuadNum][5] - 360;
	} // z축 angle이 -180~180도 범위에 들도록 함.
}

void DataSet::SetGoal(int QuadNum, double xgoal, double ygoal, double zgoal)
{
	// 1: hovering / 2: landing / 3: moving / 4: failsafe
	//switch (DataSet::Mode[QuadNum])
	//{
	//case 0: // 0: takeoff
	//	DataSet::GoalPos[QuadNum][0] = DataSet::Mpos[QuadNum][0] / 1000;
	//	DataSet::GoalPos[QuadNum][1] = DataSet::Mpos[QuadNum][1] / 1000;
	//	DataSet::GoalPos[QuadNum][2] = 600;
	//	break;

	//case 1: // 1: moving
		DataSet::GoalPos[QuadNum][0] = xgoal;
		DataSet::GoalPos[QuadNum][1] = ygoal;
		DataSet::GoalPos[QuadNum][2] = zgoal;
	//	break;

	//case 2: // 2: landing
	//	DataSet::GoalPos[QuadNum][0] = DataSet::Mpos[QuadNum][0] / 1000;
	//	DataSet::GoalPos[QuadNum][1] = DataSet::Mpos[QuadNum][1] / 1000;
	//	DataSet::GoalPos[QuadNum][2] = 250;
	//	break;

	//case 3: // 3: failsafe
	//	break;
	//}
}

void DataSet::CtrlCalc(int QuadNum)
{
	DataSet::theta_ref[QuadNum] = 0;  // 항상 x축 방향 바라봄.
	DataSet::theta[QuadNum] = q[QuadNum][5];
	DataSet::x_err[QuadNum] = DataSet::GoalPos[QuadNum][0] - DataSet::q[QuadNum][0];
	DataSet::y_err[QuadNum] = DataSet::GoalPos[QuadNum][1] - DataSet::q[QuadNum][1];
	DataSet::z_err[QuadNum] = DataSet::GoalPos[QuadNum][2] - DataSet::q[QuadNum][2];

	DataSet::theta_err[QuadNum] = DataSet::theta_ref[QuadNum] - DataSet::theta[QuadNum];
	DataSet::vx[QuadNum] = (DataSet::q[QuadNum][0] - DataSet::q_prev[QuadNum][0]) / time_gap;
	DataSet::vy[QuadNum] = (DataSet::q[QuadNum][1] - DataSet::q_prev[QuadNum][1]) / time_gap;
	DataSet::vz[QuadNum] = (DataSet::q[QuadNum][2] - DataSet::q_prev[QuadNum][2]) / time_gap;
	DataSet::dtheta[QuadNum] = (DataSet::theta[QuadNum] - q_prev[QuadNum][5]) / time_gap;
	DataSet::x_int_err[QuadNum] = DataSet::x_int_err[QuadNum] + DataSet::x_err[QuadNum] * time_gap;
	DataSet::y_int_err[QuadNum] = DataSet::y_int_err[QuadNum] + DataSet::y_err[QuadNum] * time_gap;
	DataSet::z_int_err[QuadNum] = DataSet::z_int_err[QuadNum] + DataSet::z_err[QuadNum] * time_gap;
	DataSet::theta_int_err[QuadNum] = DataSet::theta_int_err[QuadNum] + DataSet::theta_err[QuadNum] * time_gap;

	DataSet::Pos_err[QuadNum] = int(sqrt(double(DataSet::x_err[QuadNum]) * double(DataSet::x_err[QuadNum]) + double(DataSet::y_err[QuadNum]) * double(DataSet::y_err[QuadNum]) + double(DataSet::z_err[QuadNum] * DataSet::z_err[QuadNum])));

	for (int j = 0; j < 6; j++){
		DataSet::q_prev[QuadNum][j] = DataSet::q[QuadNum][j];
	}

	double Kp_r = 0.1;
	double Kd_r = 0.08;
	double Ki_r = 0.1;

	double Kp_p = 0.1;
	double Kd_p = 0.08;
	double Ki_p = 0.1;

	double Kp_y = 2.2;
	double Kd_y = 0;
	double Ki_y = 1;

	//roll
	DataSet::CTRL_command[QuadNum][0] = 1500 + (Kp_r*DataSet::x_err[QuadNum] - Kd_r*DataSet::vx[QuadNum] + Ki_r*DataSet::x_int_err[QuadNum])*sin(DataSet::theta[QuadNum] * D2R)
		+ (-Kp_r*DataSet::y_err[QuadNum] + Kd_r*DataSet::vy[QuadNum] - Ki_r * DataSet::y_int_err[QuadNum])*cos(DataSet::theta[QuadNum] * D2R);

	//pitch
	DataSet::CTRL_command[QuadNum][1] = 1500 + (-Kp_p*DataSet::x_err[QuadNum] + Kd_p*DataSet::vx[QuadNum] - Ki_p*DataSet::x_int_err[QuadNum])*cos(DataSet::theta[QuadNum] * D2R)
		+ (-Kp_p*DataSet::y_err[QuadNum] + Kd_p*DataSet::vy[QuadNum] - Ki_p*DataSet::y_int_err[QuadNum])*sin(DataSet::theta[QuadNum] * D2R);

	// yaw
	CTRL_command[QuadNum][2] = 1500 + ((-Kp_y*DataSet::theta_err[QuadNum]) + (-Kd_y*DataSet::dtheta[QuadNum]) + (-Ki_y*DataSet::theta_int_err[QuadNum]));

	// thrust
	DataSet::CTRL_command[QuadNum][3] = 1480 + 1 * ((0.23 * DataSet::z_err[QuadNum]) - (0.04*DataSet::vz[QuadNum]) + (0.12 * DataSet::z_int_err[QuadNum]));

	// Control input limitation
	if (DataSet::CTRL_command[QuadNum][0] > 1850)	DataSet::CTRL_command[QuadNum][0] = 1850;
	if (DataSet::CTRL_command[QuadNum][0] < 1150)	DataSet::CTRL_command[QuadNum][0] = 1150; //roll

	if (DataSet::CTRL_command[QuadNum][1] > 1850)	DataSet::CTRL_command[QuadNum][1] = 1850;
	if (DataSet::CTRL_command[QuadNum][1] < 1150)	DataSet::CTRL_command[QuadNum][1] = 1150; //pitch

	if (DataSet::CTRL_command[QuadNum][2] > 1850)	DataSet::CTRL_command[QuadNum][2] = 1850;
	if (DataSet::CTRL_command[QuadNum][2] < 1150)	DataSet::CTRL_command[QuadNum][2] = 1150; //yaw

	if (DataSet::CTRL_command[QuadNum][3] > 1850)	DataSet::CTRL_command[QuadNum][3] = 1850;
	if (DataSet::CTRL_command[QuadNum][3] < 1150)	DataSet::CTRL_command[QuadNum][3] = 1150; //thrusts
}