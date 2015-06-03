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
		std::string filename("config/catchtest/testsettings.json");

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
		std::string filename("config/catchtest/testsettings-missing-name.json");

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
		std::string filename("config/catchtest/testsettings.json");

		WHEN ("asking for topology configuration")
		{
			ConfigurationReader config_reader(filename);
			Configuration config;
			config_reader.fillConfiguration(config);
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
	}

	//------------------------------------------------------------------------
	GIVEN ("a filename to a valid configuration file")
	{
		std::string filename("config/catchtest/testsettings.json");

		WHEN ("asking for vehicle configuration")
		{
			ConfigurationReader config_reader(filename);
			Configuration config;
			config_reader.fillConfiguration(config);
			const VehicleConfig& r_vehicle_config = config.getVehicleConfig();

			THEN ("we get a vehicle configuration filled out")
			{
				REQUIRE (r_vehicle_config.category == OsmVehicle::MOTORCAR);
				REQUIRE (r_vehicle_config.height == Approx(1.6));
				REQUIRE (r_vehicle_config.length == Approx(4.5));
				REQUIRE (r_vehicle_config.weight == Approx(2.0));
				REQUIRE (r_vehicle_config.width == Approx(1.9));
				REQUIRE (r_vehicle_config.maxspeed == 200);
			}
		}
	}
}



