
\\ Count down from x

: cnt_down ( x -- )
    DUP 0 > 
            IF 1- DUP . RECURSE 
   THEN
;

/*
    noop drop 2drop dup 2dup swap pick 2swap over 2over rot 2rot nip tuck
    >r r> 2>r 2r> r@ 2r@ ! @
    and or xor ~ << >>
    1+ 1- + - * / * / mod /mod negate abs min max
    f1+ f+ f- f* f/ f*/
    pc sp rp
    flush
    stack .
    ." Spam And Eggs"
*/

