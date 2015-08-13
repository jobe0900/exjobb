/*
 * TurnCostCalculator.cc
 */

#include "TurnCostCalculator.h" // class implemented

/////////////////////////////// PUBLIC ///////////////////////////////////////
//============================= LIFECYCLE ====================================
//============================= OPERATORS ====================================
//============================= OPERATIONS ===================================
//============================= ACESS      ===================================
/* static */
double
TurnCostCalculator::getTurnCost(
    const Edge& rSource,
    const Edge& rTarget,
    const Configuration& rConfig)
{
    Speed angle_speed = getAngleSpeed(rSource, rTarget);
    Speed size_speed = getVehicleSizeSpeed(rSource, rTarget, rConfig, angle_speed);
    Speed turn_speed = getSmallestSpeed({angle_speed, size_speed});

    double decel_factor = rConfig.getVehicleConfig().acceleration / 100.0;
    double accel_factor = rConfig.getVehicleConfig().deceleration / 100.0;

    Cost deceleration_cost = decel_factor * (rSource.speed() - turn_speed);
    Cost acceleration_cost = accel_factor * (rTarget.speed() - turn_speed);
    Cost additional_cost = 0;

    Cost turn_cost = deceleration_cost + acceleration_cost + additional_cost;

    return static_cast<double>(turn_cost);
}
//============================= INQUIRY    ===================================
/////////////////////////////// PROTECTED  ///////////////////////////////////

/////////////////////////////// PRIVATE    ///////////////////////////////////
/* static */
Speed
TurnCostCalculator::getAngleSpeed(const Edge& rSource, const Edge& rTarget)
{
    int turn_angle = getTurnAngle(rSource, rTarget);
    double reduction_factor(1 - (abs(turn_angle)/180.0));
    double speed =
        reduction_factor * getSmallestSpeed({rSource.speed(), rTarget.speed()});
    return static_cast<Speed>(speed);
}

/* static */
Speed
TurnCostCalculator::getVehicleSizeSpeed(
    const Edge& rSource,
    const Edge& rTarget,
    const Configuration& rConfig,
    Speed angleSpeed)
{
    double vehicle_length = rConfig.getVehicleConfig().length;
    int turn_angle (getTurnAngle(rSource, rTarget));
    double length_penalty_factor = calculateLengthPenaltyFactor(turn_angle, vehicle_length);

    Speed speed = angleSpeed
                    * (VEHICLE_PENALTY_LENGTH / vehicle_length)
                    * length_penalty_factor;
    return speed;
}

/* static */
Speed
TurnCostCalculator::getSmallestSpeed(std::initializer_list<Speed> speeds)
{
    Speed min {0};

    if(speeds.size() > 0)
    {
        for(const auto& s : speeds)
        {
            if(s < min)
            {
                min = s;
            }
        }
    }
    return min;
}

/* static */
int
TurnCostCalculator::getTurnAngle(const Edge& rSource, const Edge& rTarget)
{
    int angle =
        rSource.geomData().targetBearing - rTarget.geomData().sourceBearing;
    if(angle < -180)
    {
        angle += 360;
    }
    if(angle > 180)
    {
        angle -= 360;
    }
    return angle;
}

/* static */
double
TurnCostCalculator::calculateLengthPenaltyFactor(
    int turnAngle,
    double vehicleLength)
{
    double factor(1.0);

    if(vehicleLength > VEHICLE_PENALTY_LENGTH)
    {
        if(turnAngle > 0)
        {
            factor = 1 - ((2/3) * (turnAngle/180));
        }
    }
    return factor;
}

