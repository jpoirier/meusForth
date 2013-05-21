\\ Calculate the Fib for x, the result is printed to stdio.
\\ Be careful not to make x too large; it'll 
\\ take a long time to calculate. Try 9 or 10...

: fib (x -- )
    dup 1 > if
            dup
            1 - recurse
            swap 2 - recurse
            +
    then . 
;
