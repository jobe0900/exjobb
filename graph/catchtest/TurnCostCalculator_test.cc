/* Tests for TurnCostCalclulator class.
 *
 * To run these tests one needs to comment the
 * `private` label in the TurnCostCalculator.
 *
 * TurnCostCalculator_test.cc
 *
 *  @author  Jonas Bergman
 */

#include "../../catchtest/catch.hpp"

#include "../TurnCostCalculator.h"
#include "../Edge.h"
#include "../../config/ConfigurationReader.h"
#include "../../config/Configuration.h"

SCENARIO ("Keeping track of costs for Turn", "[turncost]")
{
    Edge source(1,1,1,1);
    Edge target(2,2,2,2);

    Edge::GeomData source_geom;
    Edge::GeomData target_geom;

    Edge::RoadData primary;
    Edge::RoadData secondary;

    primary.roadType = OsmHighway::HighwayType::PRIMARY;
    secondary.roadType = OsmHighway::HighwayType::SECONDARY;


    std::string config_file(
        "catchtest/testsettings/mikh_restr_0617-testsettings.json");
    ConfigurationReader config_reader(config_file);
    Configuration config;
    config_reader.fillConfiguration(config);

    GIVEN ("two edges and a configuration")
    {
        WHEN ("asking for turn cost for turn between bearing 80 and 350")
        {
            source_geom.targetBearing = 80;
            source.setGeomData(source_geom);
            source.setSpeed(90);

            target_geom.sourceBearing = 350;
            target.setGeomData(target_geom);
            target.setSpeed(60);

            THEN ("we should get a cost")
            {
                double cost =
                    TurnCostCalculator::getTurnCost(
                        source, target, config);
                INFO ("Turn cost " << cost);
                REQUIRE (cost > 0);
            }
        }

        /*********************************************************************
         * Must comment the `private` part of the TurnCostCalculator class
         * to run the following tests.
         ********************************************************************/
//        WHEN ("getting cost for lower priority source turning into"
//            " higher priority target")
//        {
//            source.setRoadData(secondary);
//            target.setRoadData(primary);
//
//            THEN ("we should get a cost")
//            {
//                double cost =
//                    TurnCostCalculator::giveWayToHigherRoadCategoryCost(
//                    source, target);
//                REQUIRE (cost > 0);
//            }
//        }
//
//        WHEN ("getting cost for higher priority source turning into"
//            " lower priority target")
//        {
//            source.setRoadData(primary);
//            target.setRoadData(secondary);
//
//            THEN ("we should not get a cost")
//            {
//                double cost =
//                    TurnCostCalculator::giveWayToHigherRoadCategoryCost(
//                    source, target);
//                REQUIRE (cost == Approx(0.0));
//            }
//        }
//
//        WHEN ("getting cost for source turning into"
//            " equal priority target")
//        {
//            source.setRoadData(primary);
//            target.setRoadData(primary);
//
//            THEN ("we should not get a cost")
//            {
//                double cost =
//                    TurnCostCalculator::giveWayToHigherRoadCategoryCost(
//                    source, target);
//                REQUIRE (cost == Approx(0.0));
//            }
//        }
//
//        WHEN ("asking for angle between bearing 80 and bearing 20")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//
//            target_geom.sourceBearing = 20;
//            target.setGeomData(target_geom);
//
//            THEN ("we should get an angle of 60")
//            {
//                int angle = TurnCostCalculator::getTurnAngle(source, target);
//                INFO ("turn angle in 80, out 20 = " << angle);
//                REQUIRE (angle == 60);
//            }
//        }
//
//        WHEN ("asking for angle between bearing 80 and bearing 350")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//
//            target_geom.sourceBearing = 350;
//            target.setGeomData(target_geom);
//
//            THEN ("we should get an angle of 90")
//            {
//                int angle = TurnCostCalculator::getTurnAngle(source, target);
//                INFO ("turn angle in 80, out 350 = " << angle);
//                REQUIRE (angle == 90);
//            }
//        }
//
//        WHEN ("asking for angle between bearing 80 and bearing 125")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//
//            target_geom.sourceBearing = 125;
//            target.setGeomData(target_geom);
//
//            THEN ("we should get an angle of -45")
//            {
//                int angle = TurnCostCalculator::getTurnAngle(source, target);
//                INFO ("turn angle in 80, out 125 = " << angle);
//                REQUIRE (angle == -45);
//            }
//        }
//
//        WHEN ("asking for angle between bearing 80 and bearing 260")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//
//            target_geom.sourceBearing = 260;
//            target.setGeomData(target_geom);
//
//            THEN ("we should get an angle of -180")
//            {
//                int angle = TurnCostCalculator::getTurnAngle(source, target);
//                INFO ("turn angle in 80, out 260 = " << angle);
//                REQUIRE (angle == -180);
//            }
//        }
//
//        WHEN ("asking for length penalty factor for length 4.5 at angle 35 degrees")
//        {
//            int angle = 35;
//
//            THEN ("we should get a factor of 1")
//            {
//                double len = 4.5;
//                double factor =
//                    TurnCostCalculator::calculateLengthPenaltyFactor(angle, len);
//                REQUIRE (factor == Approx(1.0));
//            }
//        }
//
//        WHEN ("asking for length penalty factor for length 6.0 at angle 35 degrees")
//        {
//            int angle = 35;
//
//            THEN ("we should get a factor less than 1")
//            {
//                double len = 6.0;
//                double factor =
//                    TurnCostCalculator::calculateLengthPenaltyFactor(angle, len);
//                REQUIRE (factor < 1.0);
//            }
//        }
//
//        WHEN ("asking for length penalty factor for length 6.0 at angle -130 degrees")
//        {
//            int angle = -130;
//
//            THEN ("we should get a factor equal to 1")
//            {
//                double len = 6.0;
//                double factor =
//                    TurnCostCalculator::calculateLengthPenaltyFactor(angle, len);
//                REQUIRE (factor == Approx(1.0));
//            }
//        }
//
//        WHEN ("asking for smallest speed of 20, 40, 60, 80")
//        {
//            THEN ("we should get 20")
//            {
//                Speed smallest = TurnCostCalculator::getSmallestSpeed({80,40,20,60});
//                REQUIRE (smallest == 20);
//            }
//        }
//
//        WHEN ("asking for angle speed between bearing 80 and 350")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//            source.setSpeed(90);
//
//            target_geom.sourceBearing = 350;
//            target.setGeomData(target_geom);
//            target.setSpeed(60);
//
//            THEN ("we should get a speed")
//            {
//                Speed speed = TurnCostCalculator::getAngleSpeed(source, target);
//                INFO ("Angle speed " << speed);
//                REQUIRE ((speed > 0 && speed < 60));
//            }
//        }
//
//        WHEN ("asking for angle speed between bearing 80 and 260")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//            source.setSpeed(90);
//
//            target_geom.sourceBearing = 260;
//            target.setGeomData(target_geom);
//            target.setSpeed(60);
//
//            THEN ("we should get a speed")
//            {
//                Speed speed = TurnCostCalculator::getAngleSpeed(source, target);
//                INFO ("Angle speed " << speed);
//                REQUIRE ((speed > 0 && speed < 60));
//            }
//        }
//
//        WHEN ("asking for angle speed between bearing 80 and 20")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//            source.setSpeed(90);
//
//            target_geom.sourceBearing = 20;
//            target.setGeomData(target_geom);
//            target.setSpeed(60);
//
//            THEN ("we should get a speed")
//            {
//                Speed speed = TurnCostCalculator::getAngleSpeed(source, target);
//                INFO ("Angle speed " << speed);
//                REQUIRE ((speed > 0 && speed < 60));
//            }
//        }
//
//        WHEN ("asking for vehicle size speed between bearing 80 and 20, vehicle_length 4.5")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//            source.setSpeed(90);
//
//            target_geom.sourceBearing = 20;
//            target.setGeomData(target_geom);
//            target.setSpeed(60);
//
//            double length {4.5};
//            int angle_speed {40};
//
//
//            THEN ("we should not get a speed reduction from angle speed")
//            {
//                Speed speed =
//                    TurnCostCalculator::getVehicleSizeSpeed(
//                        source, target, length, angle_speed);
//                INFO ("Vehicle size speed " << speed);
//                REQUIRE (speed == angle_speed);
//            }
//        }
//
//        WHEN ("asking for vehicle size speed between bearing 80 and 20, vehicle_length 8.5")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//            source.setSpeed(90);
//
//            target_geom.sourceBearing = 20;
//            target.setGeomData(target_geom);
//            target.setSpeed(60);
//
//            double length {8.5};
//            int angle_speed {40};
//
//
//            THEN ("we should get a speed reduction from angle speed")
//            {
//                Speed speed =
//                    TurnCostCalculator::getVehicleSizeSpeed(
//                        source, target, length, angle_speed);
//                INFO ("Vehicle size speed " << speed);
//                REQUIRE (speed < angle_speed);
//            }
//        }
//
//        WHEN ("asking for turn cost for turn between bearing 80 and 20")
//        {
//            source_geom.targetBearing = 80;
//            source.setGeomData(source_geom);
//            source.setSpeed(90);
//
//            target_geom.sourceBearing = 20;
//            target.setGeomData(target_geom);
//            target.setSpeed(60);
//
//            THEN ("we should get a cost")
//            {
//                double cost =
//                    TurnCostCalculator::getTurnCost(
//                        source, target, config);
//                INFO ("Turn cost " << cost);
//                REQUIRE (cost > 0);
//            }
//        }
    }
}


