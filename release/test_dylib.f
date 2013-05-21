
\\ Test the FFI - importing dynamic libraries

IMPORT ./d.dylib
    1 addFunction 2
IMPORTEND

: test ( -- )
    100 99 addFunction .
;