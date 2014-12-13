#include <iostream>
#include <cmath>

using namespace std;

float cGravity = 9.81f;

typedef float float32;

typedef struct
{
float32 Fgravity;
float32 Frolling;
float32 Fdrag;
} FResistStruct;

// Calculates and returns the force components needed to achieve the
// given velocity. <params> is a dictionary containing the rider and
// environmental parameters, as returned by ScrapeParameters(), i.e.,
// all in metric units. 'velocity' is in km/h.
FResistStruct calculateForces(
const float32 &iVelocity,
const float32 &iMass,
const float32 &iRollCoef,
const float32 &iRefArea,
const float32 &iAirDensity,
const float32 &iDragCoef)
{
const float32 cEp_g = 0.0; //lets assume 0 grade
// calculate Fgravity
float32 wFgravity = cGravity *
iMass *
1;// sin(atan(cEp_g / 100.0));

// calculate Frolling
float32 wFrolling = cGravity *
iMass *
1 * //cos(atan(cEp_g / 100.0)) *
iRollCoef;

// calculate Fdrag
float32 wFdrag = 0.5 *
iRefArea *
iDragCoef *
iAirDensity *
iVelocity *
iVelocity;

// cons up and return the force components
FResistStruct ret;
ret.Fgravity = wFgravity;
ret.Frolling = wFrolling;
ret.Fdrag = wFdrag;
return ret;
}

// Calculates and returns the power needed to achieve the given
// velocity. <params> is a dictionary containing the vehicle and
// environmenetal parameters, as returned by ScrapeParameters(), i.e.,
// all in metric units. 'velocity' is in km/h. NO! it is in m/s Returns power in
// watts.
float32 calculatePower(
const float32 &iVelocity,
const float32 &iMass,
const float32 &iRollCoef,
const float32 &iRefArea,
const float32 &iAirDensity,
const float32 &iDragCoef)
{
// calculate the forces on the vehicle.
FResistStruct wForces = calculateForces(
iVelocity,
iMass,
iRollCoef,
iRefArea,
iAirDensity,
iDragCoef);

float32 wTotalForce = wForces.Fgravity + wForces.Frolling + wForces.Fdrag;
// calculate necessary wheelpower
float32 wWheelPower = wTotalForce * iVelocity;//nokmph * 1000.0 / 3600.0);

return wWheelPower;
}

// Calculates the velocity obtained from a given power. <params> is a
// dictionary containing the rider and model parameters, all in
// metric units.
//
// Runs a simple midpoint search, using CalculatePower().
//
// Returns velocity, in km/h.
float32 calculateVelocity(
const float32 &iPower,
const float32 &iMass,
const float32 &iRollCoef,
const float32 &iRefArea,
const float32 &iAirDensity,
const float32 &iDragCoef)
{
// How close to get before finishing.
float32 wEpsilon = 0.000001;

// Set some reasonable upper / lower starting points.
float32 wLowerVel = -1000.0;
float32 wUpperVel = 1000.0;
float32 wMidVel = 0.0;

float32 wMidPow = calculatePower(
wMidVel,
iMass,
iRollCoef,
iRefArea,
iAirDensity,
iDragCoef);
// Iterate until completion.
int wItCount = 0;
do {
if(abs(wMidPow - iPower) < wEpsilon)
break;
if(wMidPow > iPower)
wUpperVel = wMidVel;
else
wLowerVel = wMidVel;

wMidVel = (wUpperVel + wLowerVel) / 2.0;
wMidPow = calculatePower(
wMidVel,
iMass,
iRollCoef,
iRefArea,
iAirDensity,
iDragCoef);
} while (wItCount++ < 100);
return wMidVel;
}


int main()
{
   cout << "Hello World" << endl; 
   float valami = calculateVelocity(
       104000.0,
       5500.0,
       0.005,
       7.0,
       1.2,
       0.4);
   cout << valami << endl;
   return 0;
}

