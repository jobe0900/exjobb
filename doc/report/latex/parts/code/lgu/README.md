Line Graph Utility
==================

The main class in this utility.

Given a configuration file it picks a `MapProvider` and fetches a `Topology`, which is passed to the `GraphBuilder` along with the `Configuration`. The goal is to fetch a linegraph that is built according to the data found in the database and the configuration settings found in the configuration file.

A requirement for this utility was to be able to update data in the database which means this utility can also be requested to re-read the topology if there has been a change to them, or the restrictions and costs if there has been a change to them. 