Line Graph Utility
==================

The main class in this utility. It calls other classes:

- `ConfigReader` to read in configurations.
- `MapProvider` to get topology and other data for the graph. 
- `Graph` is created from the data obtained.

All variables are kept as local variable in the class and passed by reference and not created dynamically and passed as pointers to ease memory handling. The only exception is the final linegraph representation that are the final product of this component. That linegraph is handed out as a pointer and the responsibility for its destruction as well.

This class is not really done yet.