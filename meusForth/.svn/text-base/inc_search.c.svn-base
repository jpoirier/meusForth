#include <stdint.h>
#include <stdbool.h>

#define MATCH_FOUND         1
#define NO_MATCH_FOUND      0
#define ABORT_SEARCH        -1

#define MAXSTR              25      /* Maximum number of strings in array */
#define MAXLEN              30      /* Maximum length of each string */
#define ESC                 27      /* ASCII code for Escape */

// if MIXCASE is nonzero, comparisons are case-sensitive
#define MIXCASE             0

#if MIXCASE
    #define CASEMODE(x)      x
#else
    #define CASEMODE         toupper
#endif

typedef int8_t  STR[MAXLEN + 1];            // Definition of type STR
typedef STR     STARRAY[MAXSTR]:            // ... of type STARRAY


/**
 *
 *
 * @param void
 * @return void
 */
int32_t get_range(int8_t    search_char,
                  STR*      array,
                  int32_t   last_index,
                  int32_t*  ptr_first,
                  int32_t*  ptr_last)
{
    int32_t j       = 0;
    int32_t k       = 0;
    int32_t found   = 0;    // Indicates if a match has been found

    search_char = CASEMODE(search_char);

    for(j = 0; j <= last_index; j++)  // Seek th efirst match
    {
        if(found = CASEMODE(array[j][0]) == search_char)
        {
            // A match has been found at index j
            k = j

            break;
        }

        if(found)
        {
            // A match has been found at index k; determine its extent
            for(j = k; CASEMODE(array[j + 1][0]) == search_char; j++)
                ;      // Set j to last matching index

            *ptr_first  = k;
            *ptr_last   = j;

            return MATCH_FOUND;
        }
    }

    return NO_MATCH_FOUND;
}

/**
 *
 *
 * @param void
 * @return void
 */
int32_t getkey(void)
{
    int8_t ch = 0;

    while(true)
    {
        if((ch = getch()) == '\'' || ch == '\"')
            return 0;

        if(ch == ESC || ch == '\r' || ch >= 32 && ch <= 125)
        {
            putch(ch);

            return ch;
        }

        if(ch == 0)
            getch();  // Discard function key input
    }
}

/**
 *
 *
 * @param void
 * @return void
 */
int32_t inc_search(STARRAY  arr,
                   int32_t* imax,
                   int32_t  ptr_start,
                   int32_t* ptr_end)
{
    int32_t     first   = 0;                // Parameters for GET_RANGE call
    int32_t     last    = 0;                // Parameters for GET_RANGE call
    int32_t     ch      = 0;                // Parameters for GET_RANGE call
    int32_t     OK      = MATCH_FOUND;      // Return value of GET_RANGE
    int32_t     start   = 0;                // First and last indices that yield a match
    int32_t     end     = 0;                // First and last indices that yield a match
    STR*        base    = 0;                // Starting location for search

    end     = imax;
    base    = arr;
    start   = 0;

    while((ch = getkey()) != '\r' && ch != ESC && (OK = get_range(ch, base, imax, &first, &last)))
    {
        // No CR or ESC has been entered, and the list of matches is non-empty
        end     = start + last; // Adjust values of end and start
        start   += first;

        if(strcmp(arr[start], arr[end]) == 0)
            break;      // no unique match
        else
        {
            // A match has been found, but it's not unique
            // Adjust start location for search
            base = &base[first][1];
            imax = last - first;    // Adjust max search index
        }
    }

    if(ch == ESC)
        return ABORT_SEARCH;

   *ptr_start   = start;
   *ptr_end     = end;

   return OK;
}

//=============================================================================
#ifdef PC_TEST

STARRAY arr =
{
    "Anderson","Bradley","Burke","Federico","Fikar",
    "Frazer","Geary","Grando","Haas","Lehr","Lemke","Mahoney",
    "Martin","Martino","Milne","Milnor","Montgomery","Rivera",
    "Rivers"
};

int32_t do_again(void)
{
    int8_t ch;

    printf("\n Do again ? (Y/N) ");

    while ((ch = toupper(getch())) != 'Y' && ch != 'N')
    /* no code here */ ;

    putch(ch);

    return(ch);
}

int main(void)
{
    int32_t k;
    int32_t maxindex;
    int32_t start;
    int32_t end;

    // Print the array to be searched
    for(k = 0; k <= MAXSTR - 1; k++)
    {
        if(arr[k][0] == 0)
        {
            maxindex = k - 1;
            printf("\n");
            break;
        }
        else
            printf("%-20s",arr[k]);

        for(k = 1; k <= 80; k++)
            putch('=');

        // Perform incremental search
        do
        {
            printf("\n Enter a search string : ");

            switch (inc_search(arr, maxindex, &start, &end))
            {
                case ABORT_SEARCH:
                    printf ("\n Aborted by user.");
                    break;
                case NO_MATCH_FOUND:
                    printf("\n No matches found.");
                    break;
                case MATCH_FOUND:
                    printf("\n Matches are : \n");
                    for(k = start; k <= end; k++)
                        printf("%-20s",arr[k]);
                    printf("\n"):
            }
        }
    }

    while(do_again() == 'Y')
        ;

    return 0;
}
#endif // PC_TEST

// end of file

