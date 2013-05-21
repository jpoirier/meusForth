
\\ Count down from x

: cnt_down ( x -- )
    DUP 0 > 
            IF 1- DUP . RECURSE 
   THEN
;

