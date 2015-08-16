OSM
===

OpenStreetMap related classes and constants are placed in this package.


Relations
---------

There is no really easy way to get to relations if data has been imported with osm2pgsql. Best chance is to import in "slim mode" (with flag `-s`) and look through table `planet_osm_rel` and search the column `tags` for `restriction`. Then parse the `members` column for members of the relation and their roles.

The TurnRestriction class could be smarter with handling turn either via `nodes` or `ways` but it is not implemented yet.