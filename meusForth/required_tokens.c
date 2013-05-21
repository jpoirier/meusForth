/**@file required_tokens.c
 *
 * @author Original author:
 * @date   Creation date  :
 *
 *
 * @version X.X.X
 *
 * VERSION    DATE/AUTHOR              COMMENT
 *
 *
 *
 * Copyright (c) 2005, Alex Kouznetsov, BSD license
 *
 * @cmd<<%PRJ% file=%PFE%>>
 */
/* ----------------------------------------------------------------------------

included in xVM. At start-up console checks if these tokens exist.
   ------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "other.h"
#include "config.h"

#include "token_info.h"
#include "xvm_exe.h"
#include "compiler.h"
#include "required_tokens.h"

TTokenInfo req_xvm_token_tab[] =
{
    /* literals */
    1, "LIT0", "( -- 0 )",
    1, "LIT1", "( -- 1 )",
    1, "LIT2", "( -- 2 )",
    1, "LIT3", "( -- 3 )",
    1, "LIT4", "( -- 4 )",
    1, "LIT5", "( -- 5 )",
    1, "LIT6", "( -- 6 )",
    1, "LIT7", "( -- 7 )",
    1, "LIT8", "( -- 8 )",
    1, "LIT9", "( -- 9 )",
    1, "LIT10", "( -- 10 )",
    1, "LIT11", "( -- 11 )",
    1, "LIT12", "( -- 12 )",
    1, "LIT13", "( -- 13 )",
    1, "LIT14", "( -- 14 )",
    1, "LIT15", "( -- 15 )",
    3, "_LLIT", "( -- val )",
    2, "_LIT", "( -- val )",
    3, "_LLIT", "( -- val )",

    /* flow control */
    2, "_SJZ", "( a -- )",
    2, "_SJNZ", "( a -- )",
    2, "_SJMP", "( -- )",
    3, "_LGOTO", "( -- )",
    3, "_LCALL", "( -- )",
    1, "_RETURN", "( -- )",

    /* various */
    1, "2>R", "( x1 x2 -- ) R:( -- x1 x2 )\nTransfers two top elements of data stack to return stack",
    1, "2R>", "( -- x1 x2 ) R:( x1 x2 -- )\nTransfers two top elements of return stack to data stack",
    1, "2DUP", "( x1 x2 -- x1 x2 x1 x2 )\nDuplicate cell pair x1 x2.",
    1, "1+",   "( x -- x+1 )\nIncrement",
    1, "2DROP", "( x1 x2 -- )\nDrop cell pair x1 x2 from the stack.",
    1, ">R", "( x -- ) R:( -- x )\nTransfers top of data stack to return stack",
    1, "R>", "( -- x ) R:( x -- )\nTransfers top of return stack to data stack",

    /* end of list */
    0, "error", "out of bonds",
};

/**
 * Validates that the required native token set exists.
 *
 * @param void
 * @return total native token count
 */
int32_t all_required_tokens_exist(void)
{
    int32_t     i   = 0;
    int32_t     j   = 0;
    int32_t     n   = 0;

    for(i = 0; i < MAX_TOKEN_NO; i++)
    {
        if(req_xvm_token_tab[i].len == 0)
            break;
        else
        {
            n = find_token(req_xvm_token_tab[i].name);

            if(n < 0)
            {
                printf("ERROR: Token %s not found\n", (char*) req_xvm_token_tab[i].name);

                j++;
            }
        }
    }

    return j;
}

// end of file

