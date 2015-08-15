Line Graph Utility
==================

The main class in this utility. It calls other classes:

- `ConfigReader` to read in configurations.
- `MapProvider` to get topology and other data for the graph. 
- `Graph` is created from the data obtained.

The goal is to fetch a linegraph that is built according to the data found in the database and the configuration settings found in the configuration file.

A requirement for this utility was to be able to update data in the database which means this utility can also be requested to re-read the topology if there has been a change to them, or the restrictions and costs if there has been a change to them. 