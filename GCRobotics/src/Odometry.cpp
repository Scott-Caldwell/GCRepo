#include "Odometry.h"
#include "geometry_msgs/String.h"
void Odometry::init(int argc, char **argv);
{
	pub = n.advertise<geometry_msgs::PoseStamped>("CurrentPose",100);
	
	sub = n.subscribe("EncoderData", 100, &Odometry::odometryCallback, this);
	return;
}
void Odometry::odometryCallback(const geometry_msgs::Pose::ConstPtr& msg)
{
	en1 = msg->encoder1;
	en2 = msg->encoder2;
	en3 = msg->encoder3;
	en4 = msg->encoder4;
	
	direction1 = msg->direction1;
	direction2 = msg->direction2;	
	direction3 = msg->direction3;	
	direction4 = msg->direction4;		
	
	processMotion();
	
	return;				
}
		
void Odometry::processMotion()
{	
	if(!direction1 && !direction2 && !direction3 && !direction4) // back
	{
		x += moveStraight(en1, en2, en3, en4) * cos(heading+180);
		y += moveStraight(en1, en2, en3, en4) * sin(heading+180);
	}
	else if(!direction1 && !direction2 && direction3 && direction4) // strafe right
	{
		x += moveStrafe(en1, en2, en3, en4) * cos(heading+270);
		y += moveStrafe(en1, en2, en3, en4) * sin(heading+270);
	}
	else if(!direction1 && direction2 && !direction3 && direction4) // ccw
	{
		heading += moveRotate(en1, en2, en3,en4);
	}
	else if(direction1 && !direction2 && direction3 && !direction4) // cw
	{
		heading -= moveRotate(en1, en2, en3,en4);
	}
	else if(direction1 && direction2 && !direction3 && !direction4) // strafe left
	{
		x += moveStrafe(en1, en2, en3, en4) * cos(heading+90);
		y += moveStrafe(en1, en2, en3, en4) * sin(heading+90);
	}
	else if(direction1 && direction2 && direction3 && direction4) // forward
	{
		x += moveStraight(en1, en2, en3, en4) * cos(heading);
		y += moveStraight(en1, en2, en3, en4) * sin(heading);
	}
	
	
	// Publish position data
	
}

double Odometry::moveStraight (EC1, EC2, EC3, EC4) { // Pass in 4 integers
	long double count = (EC1 + EC2 + EC3 + EC4) / 4; // Average the inputs

	double distanceTraveled = (count * wheelCircumference / CPR); // Kstraight
	return distanceTraveled;
}

double Odometry::moveStrafe (EC1, EC2, EC3, EC4) { // Pass in 4 integers
	double distanceTraveled = moveStraight(EC1, EC2, EC3, EC4) * sin(45); // Kstrafe = Kstraight * sin(45)
	return distanceTraveled;
}

double Odometry::moveRotate (EC1, EC2, EC3, EC4) {
	long double count = (EC1 + EC2 + EC3 + EC4) / 4; // Average the inputs	
	double degreesTurned = degreesPerCount * count;
	return degreesTurned;
}
