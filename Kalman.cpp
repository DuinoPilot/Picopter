/* 
 * File:   Kalman.cpp
 * Author: matt
 * 
 * Created on 26 November 2012, 10:04
 */

#include "Kalman.h"

KalmanClass::KalmanClass()
{
    Q_angle_ = 0.000024;
    Q_bias_ = 0.00005;
    R_angle_ = 50;

    P_[0][0] = 0;
    P_[0][1] = 0;
    P_[1][0] = 0;
    P_[1][1] = 0;
    
    bias_ = 0;
}

KalmanClass::KalmanClass(const KalmanClass& orig)
{
}

KalmanClass::~KalmanClass()
{
}

void KalmanClass::predict(double* u, double* x1, float* dt)
{
    //Predicted state estimate
    //x = F.x + B.u
    *x1 += (*u - bias_) * *dt;

    //Predicted estimate covariance
    //P = F.P.F' + Q
    P_[0][0] += -(P_[0][1] + P_[1][0]) * *dt + Q_angle_; // Can ommit P_[1][1]*dt*dt due to very small influence
    P_[0][1] += -P_[1][1] * *dt; //Q[0][1] can be ommited as diagonals are always 0
    P_[1][0] += -P_[1][1] * *dt; //Q[1][0] can be ommited as diagonals are always 0
    P_[1][1] += Q_bias_;
}

void KalmanClass::update(double* z, double* x1)
{
    //Innovation
    //y = z - H.x
    y_ = *z - *x1;

    //Innovation covariance
    //S = H.P.H' + R
    S_ = P_[0][0] + R_angle_;

    //Optimal Kalman gain
    //K = P.H'.S^-1
    K_[0] = P_[0][0] / S_;
    K_[1] = P_[1][0] / S_;

    //Updated state estimate
    //x = x + K.y
    *x1 += K_[0] * y_;
    bias_ += K_[1] * y_;

    //Updated estimate covariance
    //P = (I - K.H).P    
    P_[0][0] -= K_[0] * P_[0][0];
    P_[0][1] -= K_[0] * P_[0][1];
    P_[1][0] -= K_[1] * P_[0][0];
    P_[1][1] -= K_[1] * P_[0][1];
}