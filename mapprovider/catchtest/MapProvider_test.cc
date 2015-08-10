/*
 * MapProvider_test.cc
 *
 *  Created on: 2015-05-13
 *      Author: Jonas Bergman
 */

#include "../MapProvider.h"

#include <string>
#include <map>

#include "../../catchtest/catch.hpp"
#include "../../config/ConfigurationReader.h"
#include "../../config/Configuration.h"
#include "../../config/DatabaseConfig.h"
#include "../postgis/PostGisProvider.h"
#include "../../graph/Edge.h"
#include "../../graph/Vertex.h"


SCENARIO ("MapProvider queries", "[mp-query]")
{
	try
	{
		std::string config_file("catchtest/testsettings/testsettings.json");
		ConfigurationReader config_reader(config_file);

		GIVEN ("a valid database configuration structure and "
				"name to existing topology")
		{
		    Configuration config;
		    config_reader.fillConfiguration(config);
			const DatabaseConfig& db_config = config.getDatabaseConfig();
			std::string topo_name("test");

			MapProvider* p_mp(nullptr);
			if(config.getTopologyConfig().providerName == TopologyConfig::PROVIDER_POSTGIS)
			{
			    p_mp = new PostGisProvider(config);
			}

			REQUIRE (p_mp != nullptr);

			// ...............................................................
			WHEN ("we try to fetch topology")
			{
			    Topology topology;
				p_mp->getTopology(topology);

				THEN ("we should receive topology vertices and edges")
				{
					REQUIRE (topology.nrVertices() > 0);
					REQUIRE (topology.nrEdges() > 0);
				}
			}
			delete p_mp;
		}
	}
	catch (ConfigurationException& e)
	{
		INFO(e.what());
		REQUIRE (false);	// force output of error and failure
	}
	catch (MapProviderException& dbe)
	{
		INFO(dbe.what());
		REQUIRE (false);	// force output of error and failure

	}
}
