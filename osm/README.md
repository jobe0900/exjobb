OSM
===

OpenStreetMap related classes and constants are placed in this package.


Could benefit of using configurations instead of hard coding values, e.g. which barriers restricts traffic and which costs to pass, but that must be a later step.

Relations
---------

There is no really easy way to get to relations if data has been imported with osm2pgsql. Best chance is to import in "slim mode" (with flag `-s`) and look through table `planet_osm_rel` and search the column `tags` for `restriction`. Then parse the `members` column for members of the relation and their roles.

The TurnRestriction class could be smarter with handling turn either via `nodes` or `ways` but let's keep it simple, there are only a few turn restrictions to handle.