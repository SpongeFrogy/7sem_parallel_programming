# time

MAXSIZE: 8192
linear time: 19 [µs]
Sse time: 9 [µs]
Cascade time: 42 [µs]
Combination1 time: 364 [µs]

MAXSIZE: 16384
linear time: 48 [µs]
Sse time: 19 [µs]
Cascade time: 82 [µs]
Combination1 time: 381 [µs]

MAXSIZE: 32768
linear time: 77 [µs]
Sse time: 38 [µs]
Cascade time: 183 [µs]
Combination1 time: 391 [µs]

| method/MAXSIZE      |   8192   |   16384   |   32768   | S                           |
|---------------------|----------|-----------|-----------|-----------------------------|
| linear              | 19 [µs]  | 48 [µs]   | 77 [µs]   | 1                           |
| Sse                 | 9 [µs]   | 19 [µs]   | 38 [µs]   | 4                           |
| Cascade             | 42 [µs]  | 82 [µs]   | 183 [µs]  | $log_2 (n) * n/2$           |
| Combination         | 364 [µs] | 381 [µs]  | 391 [µs]  | $n/(n/4 + log_2 (n) * n/2)$ |
| Combination Section | 364 [µs] | 387 [µs]  | 386 [µs]  | $2$                         |
