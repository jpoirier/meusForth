\\ Prints out the first 20 Fib numbers

: fib_disp ( -- ) 
    0 1 20 0 do 
               dup . tuck + 
             loop 
     2drop 
;
