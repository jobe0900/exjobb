/**  Access to Barrier data from the OSM file.
 *
 * #include "OsmBarrier.h"
 *
 * @author Jonas Bergman
 */

#ifndef OSM_OSMBARRIER_H_
#define OSM_OSMBARRIER_H_

// SYSTEM INCLUDES
//
#include <algorithm>
#include <initializer_list>
#include <map>
#include <string>
#include <vector>

// PROJECT INCLUDES
//

// LOCAL INCLUDES
//
#include "OsmException.h"

// FORWARD REFERENCES
//


class OsmBarrier
{
public:
// TYPES
    enum BarrierType
    {
        NONE,
        BLOCK,
        BOLLARD,
        BORDER_CONTROL,
        BUMP_GATE,
        BUS_TRAP,
        CATTLE_GRID,
        CHAIN,
        CYCLE_BARRIER,
        DEBRIS,
        ENTRANCE,
        FULLHEIGHT_TURNSTILE,
        GATE,
        HAMPSHIRE_GATE,
        HEIGHT_RESTRICTOR,
        HORSE_STILE,
        JERSEY_BARRIER,
        KENT_CARRIAGE_GAP,
        KISSING_GATE,
        LIFT_GATE,
        LOG,
        MOTORCYCLE_BARRIER,
        ROPE,
        SALLY_PORT,
        SPIKES,
        STILE,
        SUMP_BUSTER,
        SWING_GATE,
        TOLL_BOOTH,
        TURNSTILE,
        YES,

        NR_BARRIER_TYPES
    };

//    struct BarrierCostType
//    {
//        BarrierCostType(BarrierType barrier, unsigned cost)
//            : barrierType(barrier), cost(cost) {}
//        BarrierType     barrierType {NONE};
//        unsigned        cost {0};
//    };


    /** Barriers which infer restrictions on access.
     */
    struct RestrictionsRule
    {
        RestrictionsRule() = default;
        RestrictionsRule(const RestrictionsRule& from) = default;
        RestrictionsRule(std::initializer_list<BarrierType> restrictionTypes);

        bool    restrictsAccess(BarrierType type) const;

        std::vector<BarrierType> restrictionTypes;
//        {
//            OsmBarrier::BLOCK,
//            OsmBarrier::BOLLARD,
//            OsmBarrier::BUS_TRAP,
//            OsmBarrier::CHAIN,
//            OsmBarrier::CYCLE_BARRIER,
//            OsmBarrier::DEBRIS,
//            OsmBarrier::FULLHEIGHT_TURNSTILE,
//            OsmBarrier::HORSE_STILE,
//            OsmBarrier::JERSEY_BARRIER,
//            OsmBarrier::KENT_CARRIAGE_GAP,
//            OsmBarrier::KISSING_GATE,
//            OsmBarrier::LOG,
//            OsmBarrier::MOTORCYCLE_BARRIER,
//            OsmBarrier::ROPE,
//            OsmBarrier::SALLY_PORT,
//            OsmBarrier::SPIKES,
//            OsmBarrier::STILE,
//            OsmBarrier::SUMP_BUSTER,
//            OsmBarrier::SWING_GATE,
//            OsmBarrier::TURNSTILE,
//            OsmBarrier::YES
//        };
    };

    /** Barriers which infer costs.
     */
    struct CostsRule
    {
        CostsRule() = default;
        CostsRule(const CostsRule& from) = default;
//        CostsRule(std::initializer_list<BarrierCostType> costsTypes);

        bool        costsToPass(BarrierType type) const;
        unsigned    getCost(BarrierType type) const;
        void        addCost(BarrierType type, unsigned cost);

        std::map<BarrierType, unsigned> costs;

//        std::vector<BarrierCostType> costsTypes;
//        {
//            BarrierCostType(OsmBarrier::BORDER_CONTROL,
//            OsmBarrier::BUMP_GATE,
//            OsmBarrier::BUS_TRAP,
//            OsmBarrier::CATTLE_GRID,
//            OsmBarrier::ENTRANCE,
//            OsmBarrier::GATE,
//            OsmBarrier::HAMPSHIRE_GATE,
//            OsmBarrier::HEIGHT_RESTRICTOR,
//            OsmBarrier::JERSEY_BARRIER,
//            OsmBarrier::LIFT_GATE,
//            OsmBarrier::SUMP_BUSTER,
//            OsmBarrier::SWING_GATE,
//            OsmBarrier::TOLL_BOOTH,
//        };
    };

// LIFECYCLE
    OsmBarrier() = delete;
    OsmBarrier(BarrierType type);
    OsmBarrier(const OsmBarrier& from) = default;
    ~OsmBarrier() = default;

// OPERATORS
// OPERATIONS
    /** Attempt to parse a string to a BarrierType
     * @param   rTypeString     String which could contain a Barrier type
     * @return  A valid BarrierType
     * @throw   OsmException if invalid string.
     */
    static BarrierType  parseString(const std::string& rTypeString);

    /** Convert a Barrier Type to a string representation.
     * @param   barrierType     The type to convert.
     * @return  string representation of the type.
     * @throw   OsmException if unknown barrier type (out of bounds).
     */
    static std::string  toString(BarrierType barrierType);

    /** Convert this BarrierType to a string.
     * @return  string representation of this VehicleType.
     */
    std::string         toString() const;

    /** See if this Barrier type permits access according to rule;
     * @param   RestrictionRule
     * @return  true if access is allowed, false if not
     */
    bool                restrictsAccess(RestrictionsRule rule) const;

    /** See if this Barrier type costs to pass according to rule;
     * @param   RestrictionRule
     * @return  true if access is allowed, false if not
     */
    bool                costsToPass(CostsRule rule) const;

// ACCESS
// INQUIRY
protected:
private:
    BarrierType                           mType {NONE};
    static const std::vector<std::string> sTypeStrings;
    static const std::vector<std::string> sDisregardedTypes;
};

// INLINE METHODS
//

// EXTERNAL REFERENCES
//

#endif /* OSM_OSMBARRIER_H_ */
