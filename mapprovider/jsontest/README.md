JsonTest
========

This package exists for the sole purpose of testing in the initial stages of development, so that we easily could read in a small set of well-known edges and vertices.

But the test is now commented out, as it has not been update to keep up with advances in the development.

Format of test file in json is:

```json
    {
        "vertices": [
            [1,2,0],
            [2,2,1]
        ],
        "edges": [
            [1,1,2,0]
        ]
    }
```
    
 where each row in `vertices` are `[id,x,y]` and each row in `edges` are `[id, source vertex id, target vertex id, direction]`. Values for `direction` is `0 = BOTH`, `1 = FROM_TO`, `2 = TO_FROM`.
 
