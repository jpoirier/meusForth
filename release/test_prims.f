VARIABLE pass_cnt
VARIABLE fail_cnt

: passed ( -- )
    pass_cnt @ 1+ pass_cnt !
;

: failed ( -- )
    fail_cnt @ 1+ fail_cnt !
;

: test_cond ( -- )
    \\ ----------------------------------
    \\ passed and failed counter init
    0 pass_cnt !
    0 fail_cnt !
    \\ test 1: equal
    1 1 =
        IF ." equal test 1 passed" LF passed
        ELSE ." equal test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: equal
    1 2 =
        IF ." equal test 2 failed" LF failed
        ELSE ." equal test 2 passed" LF passed
        THEN FLUSH
    \\ test 3: equal
    2 1 =
        IF ." equal test 3 failed" LF failed
        ELSE ." equal test 3 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: greater-than
    2 1 >
        IF ." greater-than test 1 passed" LF passed
        ELSE ." greater-than test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: greater-than
    1 2 >
        IF ." greater-than test 2 failed" LF failed
        ELSE ." greater-than test 2 passed" LF passed
        THEN FLUSH
    \\ test 3: greater-than
    1 1 >
        IF ." greater-than test 3 failed" LF failed
        ELSE ." greater-than test 3 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: less-than
    1 2 <
        IF ." less-than test 1 passed" LF passed
        ELSE ." less-than test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: less-than
    2 1 <
        IF ." less-than test 2 failed" LF failed
        ELSE ." less-than test 2 passed" LF passed
        THEN FLUSH
    \\ test 3: less-than
    1 1 <
        IF ." less-than test 3 failed" LF failed
        ELSE ." less-than test 3 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: greater-than or equal
    2 1 >=
        IF ." greater-than or equal test 1 passed" LF passed
        ELSE ." greater-than or equal test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: greater-than or equal
    1 1 >=
        IF ." greater-than or equal test 2 passed" LF passed
        ELSE ." greater-than or equal test 2 failed" LF failed
        THEN FLUSH
    \\ test 3: greater-than or equal
    1 2 >=
        IF ." greater-than or equal test 3 failed" LF failed
        ELSE ." greater-than or equal test 3 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: less-than or equal
    1 2 <=
        IF ." less-than or equal test 1 passed" LF passed
        ELSE ." less-than or equal test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: less-than or equal
    1 1 <=
        IF ." less-than or equal test 2 passed" LF passed
        ELSE ." less-than or equal test 2 failed" LF failed
        THEN FLUSH
    \\ test 3: less-than or equal
    2 1 <=
        IF ." less-than or equal test 3 failed" LF failed
        ELSE ." less-than or equal test 3 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: not-equal
    1 2 !=
        IF ." not-equal test 1 passed" LF passed
        ELSE ." not-equal test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: not-equal
    2 1 !=
        IF ." not-equal test 2 passed" LF passed
        ELSE ." not-equal test 2 failed" LF failed
        THEN FLUSH
    \\ test 3: not-equal
    1 1 !=
        IF ." not-equal test 3 failed" LF failed
        ELSE ." not-equal test 3 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: equal to zero
    0 =0
        IF ." equal to zero test 1 passed" LF passed
        ELSE ." equal to zero test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: equal to zero
    1 =0
        IF ." equal to zero test 2 failed" LF failed
        ELSE ." equal to zero test 2 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: not equal to zero
    1 !=0
        IF ." not equal to zero test 1 passed" LF passed
        ELSE ." not equal to zero test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: not equal to zero
    0 !=0
        IF ." not equal to zero test 2 failed" LF failed
        ELSE ." not equal to zero test 2 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: greater-than zero
    1 >0
        IF ." greater-than zero test 1 passed" LF passed
        ELSE ." greater-than zero test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: greater-than zero
    0 >0
        IF ." greater-than zero test 2 failed" LF failed
        ELSE ." greater-than zero test 2 passed" LF passed
        THEN FLUSH
    \\ ----------------------------------
    \\ test 1: less-than zero
    -1 <0
        IF ." less-than zero test 1 passed" LF passed
        ELSE ." less-than zero test 1 failed" LF failed
        THEN FLUSH
    \\ test 2: less-than zero
    0 <0
        IF ." less-than zero test 2 failed" LF failed
        ELSE ." less-than zero test 2 passed" LF passed
        THEN FLUSH

    LF
    fail_cnt @
    pass_cnt @
    2dup
    ." Count : " + . LF
    ." Passed: " . LF
    ." Failed: " . LF
;

: test_stack ( -- )
    \\ ----------------------------------
    \\ passed and failed counter init
    0 pass_cnt !
    0 fail_cnt !
    \\ ----------------------------------
    \\ test: noop
    0 noop
        =0 IF ." noop test passed" LF passed
        ELSE ." noop test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: drop
    0 1 drop
        =0 IF ." drop test passed" LF passed
        ELSE ." drop test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: 2drop
    0 1 2 2drop
        =0 IF ." 2drop test passed" LF passed
        ELSE ." 2drop test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: dup
    1 dup
        = IF ." dup test passed" LF passed
        ELSE ." dup test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: 2dup
    1 1 2dup
        = IF
            = IF ." 2dup test passed" LF passed
            ELSE ." 2dup test failed" LF failed
            THEN
        ELSE ." 2dup test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: swap
    1 2 swap
       1 = IF
            2 = IF ." swap test passed" LF passed
            ELSE ." swap test failed" LF failed
            THEN
        ELSE ." swap test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: 2swap
    1 2 3 4 2swap
       + 3 = IF
            + 7 = IF ." 2swap test passed" LF passed
            ELSE ." 2swap test failed" LF failed
            THEN
        ELSE ." 2swap test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: pick
    1 2 3 4
       0 pick
       1 = IF ." pick test passed" LF passed
        ELSE ." pick test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: over
    1 2 over
       1 = IF ." over test passed" LF passed
        ELSE ." over test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: 2over
    1 1 3 3 2over
       + 2 = IF ." 2over test passed" LF passed
        ELSE ." 2over test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: rot
    1 2 3 rot
       1 = IF ." rot test passed" LF passed
        ELSE ." rot test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: 2rot
    1 2 3 4 5 6 2rot
       + 3 = IF ." 2rot test passed" LF passed
        ELSE ." 2rot test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: nip
    1 2 3 4 nip
       swap 2 = IF ." nip test passed" LF passed
        ELSE ." nip test failed" LF failed
        THEN FLUSH
    \\ ----------------------------------
    \\ test: tuck
    1 2 tuck
       2drop 2 = IF ." tuck test passed" LF passed
        ELSE ." tuck test failed" LF failed
        THEN FLUSH

    LF
    fail_cnt @
    pass_cnt @
    2dup
    ." Count : " + . LF
    ." Passed: " . LF
    ." Failed: " . LF
;



/*
    >r r> 2>r 2r> r@ 2r@ ! @
    and or xor ~ << >>
    1+ 1- + - * / * / mod /mod negate abs min max

    pc sp rp

*/
