FastWin: A window aggregation function execution engine for csv file

Window aggregation function is a rich set of analytical functions widely used in database and stream processing systems.

FastWin supports the full functionality of window aggregation query, including partition, sorting and aggregation based on window boundary. Meanhile, it supports inter partition parallel execution at each stage.

In this project, two algorithms: removable cumulative aggregation and segment tree algorithm is implemented and benchmarked agaist different quries.

To compile:
make

To execute:
./test.sh [csv file] [preceding] [following]

The result indicate that if the number of predicate rows and following rows is determined, it is better to use removable cumulative aggegation, otherwise, it is better to use segment tree algorithm.
