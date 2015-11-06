LineGraphUtility (lgu)
======================
This software module uses OpenStreetMap data to fetch topology, restrictions and costs, and uses them to build a Graph, which is converted to a LineGraph.

## State of the software
The software module does not fulfill the specification yet.

#### Working features
- Building graph and linegraph respecting some **edge** restrictions:
    - Turning restrictions.
    - General access restrictions.
    - Vehicle type specific restrictions.
    - Vehicle property restrictions (weight, height...).
- Some restrictions on edges.
- Turning restrictions via a node, not via other roads.
- Costs.

#### NOT implemented features
- Inclination, different speed uphill or down hill.
- Conditional restrictions.
- Turning restrictions that are not one-to-one, but one-to-many.
- Turning restrictions via ways (not via nodes).
- Parsing units, i.e. assuming all dimensions are meters and weight in metric tons and speed in km/h.

### Organization
The code is organized in folders (kind of "packages") to keep it modularized. The packages are:

- **`catchtest`**: The main for the testing framework.
- **`config`**: For configuration related code.
- **`doc`**: For documentation, report and uml diagrams.
- **`graph`**: For code that is related to Graphs.
- **`lgu`**: The main entry point into this software.
- **`mapprovider`**: The package for code providing access to map data.
- **`osm`**: Classes representing some concepts in OpenStreetMap data.
- **`preparation`**: osm-files and sql-files and instructions on how to set up database.
- **`util`**: A few utility classes.

Each folder should have its own `README.md` that describes what the contents and the purpose of that package is. Each package should also have their own tests in a `catchtest` folder, and preferably an *exception class*.

### Building
Right now  all development has been in Eclipse, so it is just a standard Eclipse project with the makefiles that Eclipse has set up in the `Debug` folder. The file `catchtest/catchmain.cc` provides the entry point for the software module during testing.

#### Libraries
There was only need for linking with `-lpqxx` and `-lpq` (for connecting to the database) until *Boost logging* was included, at which point it also became necessary to link with `-lboost_log -lboost_log_setup -lboost_thread -lboost_system -lpthread`.

#### Testing
As mentioned, testing is done with [Catch](https://github.com/philsquared/Catch). Tests can be written BDD-style, and it is header only. A few quirks: some of the macro-keywords, most notably `REQUIRE`, is reported as an error in the Eclipse editor, but one can ignore that.

### Style
I have tried to follow the style given in [C++ Coding Standard](http://www.possibility.com/Cpp/CppCodingStandard.html).

### Design
I have deliberately tried to avoid passing pointers around, and rather pass in references as IN-OUT parameters. The idea is that the central LGU class has stack variables of `Graph`, `Topology` that gets filled in, rather than obtained as pointers to objects on the heap. This is to try to reduce risks of complicated memory handling, while not have too much copying of large objects.

### Logging
Boost logging was the last feature added, and is so far only used in the `Graph` class. It needs to be compiled and linked with a lot of libraries:
    
    -lboost_log -lboost_log_setup -lboost_thread -lboost_system -lpthread
    
The log produced is `lgu.log` in the top level of the project.
