/** Calculate the turn cost for making a turn between to edges (roads).
 *
 * #include "TurnCostCalculator.h"
 *
 * @author Jonas Bergman
 */

#ifndef GRAPH_TURNCOSTCALCULATOR_H_
#define GRAPH_TURNCOSTCALCULATOR_H_

// SYSTEM INCLUDES
//
#include <initializer_list>
#include <cstdlib> // abs()

#include <iostream>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "Cost.h"
#include "Edge.h"
#include "Speed.h"
#include "../config/Configuration.h"

// FORWARD REFERENCES
//

// TYPES
//

/**
 * Calculate the cost for making turns.
 * Based on "Route Planning in Road Networks with Turn Costs"
 * by Lars Volker. Universität Karslruhe 2008.
 * http://algo2.iti.kit.edu/documents/routeplanning/volker_sa.pdf
 *
 * The cost is dependent on
 * - angle between roads
 * - size of vehicle
 * - road category precedence
 */
class TurnCostCalculator
{
public:
// LIFECYCLE
    TurnCostCalculator() = delete;
    virtual ~TurnCostCalculator() = delete;

// OPERATORS
// OPERATIONS
// ACCESS
    /**
     * @return  The cost of the turn.
     */
    static double   getTurnCost(
                        const Edge& rSource,
                        const Edge& rTarget,
                        const Configuration& rConfig);
// INQUIRY
private:
// HELPERS
    /**
     * @param   rSource     Source edge.
     * @param   rTarget     Target edge.
     * @return  The speed dependent on the angle between edges.
     */
    static Speed    getAngleSpeed(
                        const Edge& rSource,
                        const Edge& rTarget);

    /**
     * @param   rSource         Source edge.
     * @param   rTarget         Target edge.
     * @param   vehicle_length  Length of the vehicle.
     * @param   angleSpeed      The angle dependent speed
     * @return  The speed dependent on the size of the routed vehicle.
     */
    static Speed    getVehicleSizeSpeed(
                        const Edge& rSource,
                        const Edge& rTarget,
                        double      vehicle_length,
                        Speed       angleSpeed);

    /**
//     * @param   rSource     Source edge.
//     * @param   rTarget     Target edge.
//     * @return  The angle between edges.
//     */
//    static int      getAngle(
//                        const Edge& rSource,
//                        const Edge& rTarget);

    /**
     * @param   speeds      A set of speeds.
     * @return  The smallest speed
     */
    static Speed    getSmallestSpeed(std::initializer_list<Speed> speeds);

    /** Get the angle between source and target as
     * -180 < angle < 180
     * That means that 0 is straight ahead, > 0 to the right
     * and < 0 to the left.
     */
    static int      getTurnAngle(
                        const Edge& rSource,
                        const Edge& rTarget);

    /** Calculate a penalty for making sharp right turns with long vehicles.
     * @param   turnAngle       The turning angle in degrees (-180 < a < 180).
     * @param   vehicleLength   The length of the vehicle.
     * @return  A factor 0.33 - 1.0
     */
    static double   calculateLengthPenaltyFactor(
                        int     turnAngle,
                        double  vehicleLength);

// ATTRIBUTES
// CONSTANTS
    static constexpr double VEHICLE_PENALTY_LENGTH = 4.5;

};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* GRAPH_TURNCOSTCALCULATOR_H_ */
