# vEB_tree_optimizations

Van Emde Boas trees are a dictionary data structure that have less than logarithmic asymptotic running time queries. Although the running time is asymptoticly fast, at what universe size does a simple data structure outperform a van Emde Boas tree? Determining the universe size at which performance is improved and the performance characteristics of a data structure gives justification for the usage of that data structure. A van Emde Boas tree was implemented and insertion, predecessor, and delete tests were run, tests were also run when the tree was truncated at varying depths replacing that section of the tree with an array. It was found that truncating at small universe sizes was always superior to a standard approach and a higher-level truncation was advantageous, dependent on the density of the tree. These results can make an existing implementation of a van Emde Boas tree faster and allow the data structure to be used in contexts previously unfavorable.

A simple array of the universe size was implemented as an alternative base case. 

# Sample of testing results:
time values are insertion, predecessor, search.
|     Iterations, Universe    |     2^27, 2^26                      |     2^26, 2^30                     |     2^25, 2^30                       |     2^24, 2^30                       |     2^23, 2^30                       |     2^22, 2^30                   |     2^21, 2^30                  |     2^20, 2^30                   |     2^19, 2^30                  |
|-----------------------------|-------------------------------------|------------------------------------|--------------------------------------|--------------------------------------|--------------------------------------|----------------------------------|---------------------------------|----------------------------------|---------------------------------|
|     Regular                 |     142.3046, 139.2610, 124.6279    |     Null                           |     Null                             |     15.1992, 14.911, 8.2731          |     7.9235, 7.392, 3.7575            |     4.0297, 3.7786, 1.5995       |     2.0953, 1.8315, 0.6152      |     1.0225, 0.8230, 0.2418       |     0.4981, 0.3669, 0.1016      |
|     Base case size 4        |     91.8196, 103.5149, 88.6726      |      54.0415, 61.3509, 46.3924     |     26.0478, 29.6145, 18.7344        |     12.7108, 13.9839, 8.3110,        |      6.4465, 6.9891, 3.6178          |       3.2865, 3.3994, 1.5030     |     1.5969, 1.6379, 0.5815      |     0.7756, 0.7235, 0.2396       |     0.3868, 0.3267, 0.1037      |
|     Base case size 16       |     54.9842, 65.7456, 48.6141       |     44.1428, 54.5812,   40.3103    |     23.3990, 27.6029,   19.1118      |     11.6813, 12.7197,   8.4602       |     5.6235, 6.0635,   3.7688         |     2.9157, 2.9675,   1.5907     |     1.4205, 1.4442,   0.6105    |     0.6712, 0.3297,   0.2367     |     0.3198, 0.1645,   0.1306    |
|     Base case size 256      |     29.3181, 31.798, 24.3064        |     34.9425, 37.5395,   28.8529    |     19.441, 19.9976,   15.0439       |     11.4913, 10.9539,   7.7677       |     6.6728, 5.444,   3.5007          |     4.0069, 2.6478,   1.5124     |     2.2162, 1.1998,   0.5851    |     1.0384, 0.5264,   0.2219     |     0.5487, 0.2339,   0.0953    |
|     Base case size 65536    |     30.0506, 33.0001, 26.2492       |     34.8454, 37.4003,   29.3329    |     19.2288, 19.7541,   15.3377,     |     11.5969, 11.0866, 7.56231        |     6.8062, 5.5097,   3.5313         |     4.0387, 2.5913,   1.4909     |     2.2424, 1.2945,   0.5963    |     1.1680, 0.5382, 0.2335       |     0.5826, 0.1655,   0.1348    |

# Highlights
- A base case of 16 is always faster
- When there is less than an average of 128 between keys the base case size of 256 is faster than 16
- Large memory imporvements where seen, of up to 29x
- The fastest base case also used the least memory
