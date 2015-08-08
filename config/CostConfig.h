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

// FORWARD REFERENCES
//

/**
 * Information about names in the database for topology data.
 */
struct CostConfig
{
// TYPES
    typedef int Speed;

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

        void            addDefaultSpeed(
                            OsmHighway::HighwayType type,
                            HighLowSpeed speed)
        {
            defaultSpeed.erase(type);
            defaultSpeed.insert({type, speed});
        }

        HighLowSpeed    getDefaultSpeed(OsmHighway::HighwayType type) const
        {
            const auto& it = defaultSpeed.find(type);
            if(it != defaultSpeed.end())
            {
                return it->second;
            }
            return HighLowSpeed();
        }

        Speed           getDefaultSpeed(
                            OsmHighway::HighwayType type,
                            HIGH_LOW highOrLow) const
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


//        HighLowSpeed motorway;
//        HighLowSpeed motorway_link;
//        HighLowSpeed trunk;
//        HighLowSpeed trunk_link;
//        HighLowSpeed primary;
//        HighLowSpeed primary_link;
//        HighLowSpeed secondary;
//        HighLowSpeed secondary_link;
//        HighLowSpeed tertiary;
//        HighLowSpeed tertiary_link;
//        HighLowSpeed unclassified;
//        HighLowSpeed residential;
//        HighLowSpeed service;
//        HighLowSpeed living_street;
//        HighLowSpeed bus_guideway;
//        HighLowSpeed road;
    };

    struct SurfaceMaxSpeed
    {

        std::map<OsmHighway::SurfaceType, Speed> surfaceSpeed;

        void    addSurfaceMaxSpeed(OsmHighway::SurfaceType type, Speed speed)
        {
            surfaceSpeed.erase(type);
            surfaceSpeed.insert({type, speed});
        }

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

// ATTRIBUTES
    DefaultSpeed    defaultSpeed;
    SurfaceMaxSpeed surfaceMaxSpeed;

// ACCESS
//    int     getDefaultSpeed(
//                OsmHighway::HighwayType     highwayType,
//                DefaultSpeed::HIGH_LOW      highLow) const
//    {
//        switch(highwayType)
//        {
//            case OsmHighway::MOTORWAY:
//                return getHighOrLow(defaultSpeed.motorway, highLow); break;
//            case OsmHighway::MOTORWAY_LINK:
//                return getHighOrLow(defaultSpeed.motorway_link, highLow); break;
//            case OsmHighway::TRUNK:
//                return getHighOrLow(defaultSpeed.trunk, highLow); break;
//            case OsmHighway::TRUNK_LINK:
//                return getHighOrLow(defaultSpeed.trunk_link, highLow); break;
//            case OsmHighway::PRIMARY:
//                return getHighOrLow(defaultSpeed.primary, highLow); break;
//            case OsmHighway::PRIMARY_LINK:
//                return getHighOrLow(defaultSpeed.primary_link, highLow); break;
//            case OsmHighway::SECONDARY:
//                return getHighOrLow(defaultSpeed.secondary, highLow); break;
//            case OsmHighway::SECONDARY_LINK:
//                return getHighOrLow(defaultSpeed.secondary_link, highLow); break;
//            case OsmHighway::TERTIARY:
//                return getHighOrLow(defaultSpeed.tertiary, highLow); break;
//            case OsmHighway::TERTIARY_LINK:
//                return getHighOrLow(defaultSpeed.tertiary_link, highLow); break;
//            case OsmHighway::UNCLASSIFIED:
//                return getHighOrLow(defaultSpeed.unclassified, highLow); break;
//            case OsmHighway::RESIDENTIAL:
//                return getHighOrLow(defaultSpeed.residential, highLow); break;
//            case OsmHighway::SERVICE:
//                return getHighOrLow(defaultSpeed.service, highLow); break;
//            case OsmHighway::LIVING_STREET:
//                return getHighOrLow(defaultSpeed.living_street, highLow); break;
//            case OsmHighway::BUS_GUIDEWAY:
//                return getHighOrLow(defaultSpeed.bus_guideway, highLow); break;
//            case OsmHighway::ROAD:
//                return getHighOrLow(defaultSpeed.road, highLow); break;
//            case OsmHighway::NR_HIGHWAY_TYPES:
//            default:
//                return 0; break;
//        }
//        return 0;
//    }

// CONSTANTS

private:
//    int     getHighOrLow(
//                DefaultSpeed::HighLowSpeed  speed,
//                DefaultSpeed::HIGH_LOW      highLow) const
//    {
//        if(highLow == DefaultSpeed::HIGH)
//        {
//            return speed.high;
//        }
//        else
//        {
//            return speed.low;
//        }
//    }
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* CONFIG_COSTCONFIG_H_ */
