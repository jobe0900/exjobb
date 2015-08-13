/*
 * ConfigurationReader_test.cc
 *
 *  Created on: 2015-05-07
 *      Author: Jonas Bergman
 */

#include "../../catchtest/catch.hpp"
#include "../Configuration.h"
#include "../ConfigurationReader.h"
#include "../ConfigurationException.h"

SCENARIO ("Use ConfigurationReader to read configuration from json file",
		"[json],[config]")
{
	//------------------------------------------------------------------------
	GIVEN ("a filename to a valid configuration file")
	{
		std::string filename("catchtest/testsettings/testsettings.json");

		WHEN ("asking for database configuration")
		{
			ConfigurationReader config_reader(filename);
			Configuration config;
			config_reader.fillConfiguration(config);
			const DatabaseConfig& r_db_config = config.getDatabaseConfig();

			THEN ("we get a  database configuration filled out")
			{
				REQUIRE (r_db_config.hostname == "127.0.0.1");
				REQUIRE (r_db_config.port == 5432);
				REQUIRE (r_db_config.username == "tester");
				REQUIRE (r_db_config.password == "tester");
				REQUIRE (r_db_config.database == "mikh_0530");
			}
		}
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a configuration file with missing information")
	{
		std::string filename("catchtest/testsettings/testsettings-missing-name.json");

		WHEN ("asking for database configuration")
		{
			ConfigurationReader config_reader(filename);

			THEN ("we get an exception")
			{
			    Configuration config;
				REQUIRE_THROWS_AS (config_reader.fillConfiguration(config),
						ConfigurationException&);
			}
		}
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a non-existing file")
	{
		std::string filename("config/catchtest/foo.json");

		WHEN ("asking for database configuration")
		{
			THEN ("we get an exception")
			{
				REQUIRE_THROWS_AS (ConfigurationReader config_reader(filename),
						ConfigurationException&);
			}
		}
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a valid configuration file")
	{
		std::string filename("catchtest/testsettings/testsettings.json");
		ConfigurationReader config_reader(filename);
		Configuration config;
		config_reader.fillConfiguration(config);

		//....................................................................
		WHEN ("asking for topology configuration")
		{
			const TopologyConfig& r_topo_config = config.getTopologyConfig();

			THEN ("we get a topology configuration filled out")
			{
				REQUIRE (r_topo_config.providerName == "postgis");
				REQUIRE (r_topo_config.topoName == "lgu");
				REQUIRE (r_topo_config.roadsPrefix == "highways");
				REQUIRE (r_topo_config.topologySchemaPrefix == "topo");
				REQUIRE (r_topo_config.tempTopoName == "");
				REQUIRE (r_topo_config.srid == 900913);
				REQUIRE (r_topo_config.tolerance == Approx(1.0));

				REQUIRE (r_topo_config.edgeTableName == "edge_data");
				REQUIRE (r_topo_config.edgeIdColumnName == "edge_id");
				REQUIRE (r_topo_config.sourceColumnName == "start_node");
				REQUIRE (r_topo_config.targetColumnName == "end_node");
				REQUIRE (r_topo_config.edgeGeomColumnName == "geom");
				REQUIRE (r_topo_config.vertexTableName == "node");
				REQUIRE (r_topo_config.vertexIdColumnName == "node_id");
				REQUIRE (r_topo_config.vertexGeomColumnName == "geom");
			}
		}

		//....................................................................
		WHEN ("asking for vehicle configuration")
		{
			const VehicleConfig& r_vehicle_config = config.getVehicleConfig();

			THEN ("we get a vehicle configuration filled out")
			{
				REQUIRE (r_vehicle_config.category == OsmVehicle::MOTORCAR);
				REQUIRE (r_vehicle_config.height == Approx(1.6));
				REQUIRE (r_vehicle_config.length == Approx(4.5));
				REQUIRE (r_vehicle_config.weight == Approx(2.0));
				REQUIRE (r_vehicle_config.width == Approx(1.9));
				REQUIRE (r_vehicle_config.maxspeed == 200);
				REQUIRE (r_vehicle_config.acceleration == 10);
				REQUIRE (r_vehicle_config.deceleration == 7);
			}
		}

		//....................................................................
		WHEN ("asking for cost configuration")
		{
			const CostConfig& r_cost_config = config.getCostConfig();

			THEN ("we get a cost configuration filled out")
			{
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::MOTORWAY, CostConfig::DefaultSpeed::HIGH) == 110);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::MOTORWAY, CostConfig::DefaultSpeed::LOW) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::MOTORWAY_LINK, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::MOTORWAY_LINK, CostConfig::DefaultSpeed::LOW) == 90);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TRUNK, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TRUNK, CostConfig::DefaultSpeed::LOW) == 60);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TRUNK_LINK, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TRUNK_LINK, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::PRIMARY, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::PRIMARY, CostConfig::DefaultSpeed::LOW) == 60);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::PRIMARY_LINK, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::PRIMARY_LINK, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::SECONDARY, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::SECONDARY, CostConfig::DefaultSpeed::LOW) == 60);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::SECONDARY_LINK, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::SECONDARY_LINK, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TERTIARY, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TERTIARY, CostConfig::DefaultSpeed::LOW) == 60);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TERTIARY_LINK, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::TERTIARY_LINK, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::UNCLASSIFIED, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::UNCLASSIFIED, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::RESIDENTIAL, CostConfig::DefaultSpeed::HIGH) == 90);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::RESIDENTIAL, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::SERVICE, CostConfig::DefaultSpeed::HIGH) == 40);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::SERVICE, CostConfig::DefaultSpeed::LOW) == 20);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::LIVING_STREET, CostConfig::DefaultSpeed::HIGH) == 20);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::LIVING_STREET, CostConfig::DefaultSpeed::LOW) == 20);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::BUS_GUIDEWAY, CostConfig::DefaultSpeed::HIGH) == 80);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::BUS_GUIDEWAY, CostConfig::DefaultSpeed::LOW) == 60);

			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::ROAD, CostConfig::DefaultSpeed::HIGH) == 80);
			    REQUIRE (r_cost_config.defaultSpeed.getDefaultSpeed(
			        OsmHighway::ROAD, CostConfig::DefaultSpeed::LOW) == 50);

			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::PAVED) == 1000);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::ASPHALT) == 1000);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::COBBLESTONE) == 20);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::COBBLESTONE_FLATTENED) == 40);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::SETT) == 40);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::CONCRETE) == 1000);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::CONCRETE_LANES) == 40);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::CONCRETE_PLATES) == 100);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::PAVING_STONES) == 40);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::METAL) == 60);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::WOOD) == 30);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::UNPAVED) == 60);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::COMPACTED) == 70);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::DIRT) == 40);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::EARTH) == 40);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::FINE_GRAVEL) == 50);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::GRASS) == 10);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::GRASS_PAVER) == 20);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::GRAVEL) == 60);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::GROUND) == 20);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::ICE) == 70);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::MUD) == 5);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::PEBBLESTONE) == 50);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::SALT) == 70);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::SAND) == 70);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::SNOW) == 50);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::WOODCHIPS) == 5);
			    REQUIRE (r_cost_config.surfaceMaxSpeed.getSurfaceMaxSpeed(
			        OsmHighway::METAL_GRID) == 40);

			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "highway=bus_stop") == 5);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "highway=crossing") == 5);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "highway=give_way") == 20);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "highway=mini_roundabout") == 20);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "highway=stop") == 30);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "highway=traffic_signals") == 30);

			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "railway=level_crossing") == 20);

			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "public_transport=stop_position") == 5);

			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=yes") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=bump") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=table") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=cushion") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=rumble_strip") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=chicane") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=choker") == 10);
			    REQUIRE(r_cost_config.otherEdgeCosts.getOtherCost(
			        "traffic_calming=island") == 5);
			}
		}

		WHEN ("asking for access rules")
		{
		    const OsmAccess::AccessRule r_access_rule = config.getAccessRule();

		    THEN ("we get an AccessRule filled out")
		    {
		        std::vector<OsmAccess::AccessType> types = r_access_rule.allowAccessToTypes;
		        REQUIRE (types.size() == 3);

		        auto it = std::find(types.begin(), types.end(),
		            OsmAccess::AccessType::YES);
		        INFO ("Allows access:" + OsmAccess::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmAccess::AccessType::PERMISSIVE);
		        INFO ("Allows access:" + OsmAccess::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmAccess::AccessType::DESIGNATED);
		        INFO ("Allows access:" + OsmAccess::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmAccess::AccessType::NO);
		        INFO ("Denies access: no");
		        REQUIRE (it == types.end());
		    }
		}

		WHEN ("asking for restrictions rules")
		{
		    const OsmBarrier::RestrictionsRule
		        r_restrict_rule = config.getBarrierRestrictionsRule();

		    THEN ("we get RestrictionsRule filled out")
		    {
		        std::vector<OsmBarrier::BarrierType> types =
		            r_restrict_rule.restrictionTypes;
		        REQUIRE (types.size() == 21);

		        auto it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::BLOCK);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::BOLLARD);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::BUS_TRAP);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::CHAIN);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::CYCLE_BARRIER);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::DEBRIS);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::FULLHEIGHT_TURNSTILE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::HORSE_STILE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::JERSEY_BARRIER);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::KENT_CARRIAGE_GAP);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::KISSING_GATE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::LOG);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::MOTORCYCLE_BARRIER);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::ROPE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::SALLY_PORT);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::SPIKES);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::STILE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::SUMP_BUSTER);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::SWING_GATE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::TURNSTILE);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::YES);
		        INFO ("Restrict: " + OsmBarrier::toString(*it));
		        REQUIRE (it != types.end());

		        it = std::find(types.begin(), types.end(),
		            OsmBarrier::BarrierType::GATE);
		        INFO ("Allow: gate");
		        REQUIRE (it == types.end());
		    }
		}

		WHEN ("asking for costs rules")
		{
		    const OsmBarrier::CostsRule r_costs_rule = config.getBarrierCostsRule();

		    THEN ("we get CostssRule filled out")
		    {
//		        std::vector<OsmBarrier::BarrierType> types =
//		            r_costs_rule.costsTypes;
		        REQUIRE (r_costs_rule.costs.size() == 13);

		        INFO("Costs: border control");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::BORDER_CONTROL) == 120);

		        INFO("Costs: bump gate");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::BUMP_GATE) == 30);

		        INFO("Costs: bus trap");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::BUS_TRAP) == 30);

		        INFO("Costs: cattle grid");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::CATTLE_GRID) == 20);

		        INFO("Costs: entrance");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::ENTRANCE) == 10);

		        INFO("Costs: gate");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::GATE) == 30);

		        INFO("Costs: hampshire gate");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::HAMPSHIRE_GATE) == 60);

		        INFO("Costs: height restrictor");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::HEIGHT_RESTRICTOR) == 5);

		        INFO("Costs: jersey barrier");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::JERSEY_BARRIER) == 10);

		        INFO("Costs: lift gate");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::LIFT_GATE) == 60);

		        INFO("Costs: sump buster");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::SUMP_BUSTER) == 30);

		        INFO("Costs: swing gate");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::SWING_GATE) == 60);

		        INFO("Costs: toll both");
		        REQUIRE (r_costs_rule.getCost(
		            OsmBarrier::BarrierType::TOLL_BOOTH) == 40);

		        INFO("No cost: yes");
		        REQUIRE (r_costs_rule.costsToPass(OsmBarrier::BarrierType::YES) == false);
		    }
		}
	}
}



