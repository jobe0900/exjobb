CATCH
=====
This project uses [Catch](https://github.com/philsquared/Catch) for testing. It allows for writing tests BDD-style.

It doesn't play really nicely with Eclipse, as Eclipse's editor marks `REQUIRE` as errors, so the project has a  lot of error markers throughout, without any real errors. But the Catch way of testing is nice, so it is worth is. And Eclipse flags a lot of errors for standard c++11 features as well...

When writing `SCENARIO`s or `TESTCASE`s one can tag those, which makes it easy to test small parts of the code. After building you can modify the Eclipse `Run Configuration` (or write on the command line) to only run those tests.

Example:

```cpp
SCENARIO ("Testing this module but not other", "[moduletag]")
{
    GIVEN ("a")
    {
        WHEN ("b")
        {
            THEN ("c")
            {
                REQUIRE (c)
            }
        }
    }
}
```

To specify which test to run, go to `Run` > `Run Configurations...`, select the `Arguments` tab and in `Program arguments` write the tag, e.g. `[moduletag]`, click `Apply` and `Run`.

A useful flag to add to the program arguments when running tests is `-s` to have print out of every step, else you only get the final report of how many scenarios have run.