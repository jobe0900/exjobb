/**  Data structure for configuration of costs.
 *
 * #include "CostConfig.h"
 *
 * @author Jonas Bergman
 */

#ifndef CONFIG_COSTCONFIG_H_
#define CONFIG_COSTCONFIG_H_

// SYSTEM INCLUDES
//
#include <map>
#include <string>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "../osm/OsmHighway.h"
#include "../graph/Cost.h"
#include "../graph/Speed.h"

// FORWARD REFERENCES
//

/**
 * Information about names in the database for topology data.
 */
struct CostConfig
{
// TYPES

    /** Keep track of default speeds for different categories of roads.
     * The values are a high and a low value, depending of if we are inside or
     * outside of an urban area.
     */
    struct DefaultSpeed
    {
        enum HIGH_LOW
        {
            HIGH,
            LOW
        };

        struct HighLowSpeed
        {
            Speed high {0};
            Speed low {0};
        };

        std::map<OsmHighway::HighwayType, HighLowSpeed> defaultSpeed;

        /** Add a speed far a specific road category (highway type).
         * @param   type    The highway type
         * @param   speed   The high and low speed limits.
         */
        void            addDefaultSpeed(
                            OsmHighway::HighwayType type,
                            HighLowSpeed speed)
        {
            defaultSpeed.erase(type);
            defaultSpeed.insert({type, speed});
        }

        /**
         * @param   type    The highway type
         * @return  The high/low speed for this type of highway.
         */
        HighLowSpeed    getDefaultSpeed(OsmHighway::HighwayType type) const
        {
            const auto& it = defaultSpeed.find(type);
            if(it != defaultSpeed.end())
            {
                return it->second;
            }
            return HighLowSpeed();
        }

        /** Get a high or low speed limit for a highway type.
         * @param   type        The Type of highway.
         * @param   highOrLow   Either HIGH or LOW speed
         * @return  Either the high or low speed for a highway type.
         */
        Speed           getDefaultSpeed(
                            OsmHighway::HighwayType type,
                            HIGH_LOW                highOrLow) const
        {
            HighLowSpeed hl = getDefaultSpeed(type);
            if(highOrLow == HIGH)
            {
                return hl.high;
            }
            else
            {
                return hl.low;
            }
        }
    };

    /** Keep track of max speed that are suitable for different kind of
     * surfaces.
     */
    struct SurfaceMaxSpeed
    {
        std::map<OsmHighway::SurfaceType, Speed> surfaceSpeed;

        /** Add a surface type and the max speed.
         * @param   type    The type of surface.
         * @param   speed   The max suitable speed for the surface type.
         */
        void    addSurfaceMaxSpeed(OsmHighway::SurfaceType type, Speed speed)
        {
            surfaceSpeed.erase(type);
            surfaceSpeed.insert({type, speed});
        }

        /**
         * @return  The suitable max speed for a surface type.
         */
        Speed   getSurfaceMaxSpeed(OsmHighway::SurfaceType type) const
        {
            const auto& it = surfaceSpeed.find(type);
            if(it != surfaceSpeed.end())
            {
                return it->second;
            }
            return 0;
        }
    };

    /** Other edge costs are kept track of simply by strings as keys and
     * Costs as values. The costs are "penalties" added to the travel time.
     * The string that make up the keys are simply constructed as "tag=value",
     * e.g. "highway=give_way".
     */
    struct OtherEdgeCosts
    {
        std::map<std::string, Cost> otherCostValues;

        /** Add a 'penalty' for another kind of EdgeCost.
         * @param   key     String of "tag=value" that makes up the cost.
         * @param   cost    The cost for this kind of hindrance.
         */
        void    addOtherCost(std::string key, Cost cost)
        {
            otherCostValues.erase(key);
            otherCostValues.insert({key, cost});
        }

        /** Get other costs associated with the key.
         * @param   key
         * @return  The cost for this key.
         */
        Cost    getOtherCost(std::string key) const
        {
            const auto& it = otherCostValues.find(key);
            if(it !=otherCostValues.end())
            {
                return it->second;
            }
            return 0;
        }
    };

// ATTRIBUTES
    DefaultSpeed    defaultSpeed;
    SurfaceMaxSpeed surfaceMaxSpeed;
    OtherEdgeCosts  otherEdgeCosts;

// ACCESS
// CONSTANTS

private:
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_COSTCONFIG_H_ */
