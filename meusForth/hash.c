/**@file hash.c
 *
 * @author Original author: Copied from the NIST ATM Simulator (public domain)
 * @date   Creation date  : 1998/03/03
 *
 *
 * @version X.X.X
 *
 * VERSION    DATE/AUTHOR              COMMENT
 *
 *
 *
 * Copyright (c) 2008
 *
 *
 *
 * Works with arbitrary length keys (< 700,000 bytes) & records.
 *  - ht_create       Make a table
 *  - ht_insert       Insert a new record
 *  - ht_delete       Delete a record
 *  - ht_find         Look up a record
 *  - ht_obliterate   Destroy the table & free all of its memory
 *  - ht_iter         CLU-like iterator to return all records in the table
 *
 * Internal routines that should not be called by others
 *  - ht_qfind        Finds a record in a bucket (queue)
 *  - hash            Computes the hash value of a key
 *  - ht_iter_setup   Initializes the iteration
 *
 * Notes on use:
 *  The table automatically allocates memory & makes a copy of a record that
 *  is inserted, so the storage that is passed to the ht_insert function may be
 *  reused.  However, the pointer that is returned from the ht_find is a
 *  pointer into the table.  The memory pointed should NOT be modified.
 *
 * @cmd<<%PRJ% file=%PFE%>>
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hash.h"

static void ht_iter_setup(Htable ht);
static Hash_elem* ht_qfind(queue* q, int8_t* key, int32_t size);
static uint32_t hash(Htable ht, int8_t* key);

// Hash routine.  More-or-less from Knuth Vol. III.
// Macro to rotate (rather than just shift) a number.  This macro will
// work only on machines with 8-bit bytes.
#define hash_rotl(x) ((x) & (1 << (8 * sizeof(x) - 1)) ? ((x) << 1) | 1 : (x) << 1)

/**
 * Create function--the size passed should be approximately the maximum
 * number of records to be stored in the table, and should be a prime number.
 *
 * @param size the total number of key-record pairs
 * @param key_size the maximum int8_t count for a key
 * @param rec_size the maximum int8_t count for a record
 * @return a pointer to the table
 */
Htable ht_create(int32_t size,
                 int32_t key_size,
                 int32_t rec_size)
{
    Htable ht = 0;

    // Allocate memory with wild abandon
    ht = (Htable) calloc(1, sizeof(struct _Htable));

    assert(ht);

    ht->ht_size     = size;
    ht->ht_key_size = key_size;
    ht->ht_rec_size = rec_size;

    ht->ht_table = (Hash_bucket*) calloc((size_t) size, sizeof(Hash_bucket));

    assert(ht->ht_table);

  return ht;
}

/**
 * Insert a new record into the hash table.  Duplicate insertions
 * are not allowed
 *
 * @param mode ht the hash table
 * @param mode key the key part of a key-record pair
 * @param mode pvoid
 * @param mode rec the record part of a key-record pair
 * @return 0 if sim_calloc() failed, 1 if okay, -1 if record already exists
 */
int32_t ht_insert(Htable    ht,
                  int8_t*   key,
                  void*     pvoid,
                  int8_t*   rec)

{
    Hash_elem*  he;
    int32_t     num;

    num = hash(ht, key);

    if(!ht_qfind(&ht->ht_table[num], key, ht->ht_key_size))
    {

        he = (Hash_elem*) calloc(1, (size_t) (sizeof(Hash_elem) + ht->ht_key_size + ht->ht_rec_size));

        if(!he)
            return 0; // calloc failed

        // Copy key and record into the Hash_elem
        memmove((void*) he->he_key,
                (void const*) key,
                ht->ht_key_size);


        if(rec)
            memmove((void*) (he->he_key + ht->ht_key_size),
                    (void const*) rec,
                    ht->ht_rec_size);

        if(pvoid)
            he->he_pvoid = pvoid;

        ((q_elt*)(he))->qe_next = 0;

        if(ht->ht_table[num].q_head == 0)
        {
            ht->ht_table[num].q_head = (q_elt*) (he);
        }
        else
        {
            ht->ht_table[num].q_tail->qe_next = (q_elt*) (he);
        }

        ht->ht_table[num].q_tail = (q_elt*) (he);

        if(++(ht->ht_table[num].q_len) > ht->ht_table[num].q_max)
            ht->ht_table[num].q_max = ht->ht_table[num].q_len;

        return 1;
    }
    else
        return -1;  // A record with this key already exists
}

/**
 * Remove a key-record pair from the table object.
 *
 * @param ht the hash table
 * @param key the key
 * @return 1 if successful, 0 if not found
 */
bool ht_delete(Htable   ht,
               int8_t*  key)
{
    Hash_elem*  he      = 0;
    Hash_elem*  prev    = 0;
    q_elt*      next    = 0;
    int32_t     num     = 0;

    num = hash(ht, key);

    if((prev = ht_qfind(&ht->ht_table[num], key, ht->ht_key_size)))
    {
        // Since qfind returns a pointer to the previous item, we can just use qe_dela to delete it
        he = prev->he_next;

        next = ((q_elt*) (prev))->qe_next;

        if(ht->ht_table[num].q_tail == next)
        {
            if(ht->ht_table[num].q_head == next)
                ht->ht_table[num].q_tail = 0;
            else
                ht->ht_table[num].q_tail = (q_elt*) (prev);
        }

        ((q_elt*) (prev))->qe_next = next->qe_next;

        next->qe_next = 0;

        if(--(ht->ht_table[num].q_len) < ht->ht_table[num].q_min)
            ht->ht_table[num].q_min = ht->ht_table[num].q_len;

        if(he->he_pvoid)
            free(he->he_pvoid);

        free((char*) he);

        return true;
    }
    else
        return false; // Not found
}

/**
 * Searches for record.
 *
 * @param ht the hash table
 * @param key the key
 * @return a valid pointer to a record or 0 if not found. Note that the
 * pointer points into the hash table and so should not be modified.  Note
 * also that if you delete this record after finding it, this pointer is no
 * longer valid.
 */
int8_t* ht_rec_find(Htable  ht,
                    int8_t* key)
{
    Hash_elem*  prev    = 0;
    int32_t     num     = 0;

    num = hash(ht, key);

    if((prev = ht_qfind(&ht->ht_table[num], key, ht->ht_key_size)))
    {
        // Since qfind returns a pointer to the previous item,
        // we must follow the link once.
        // Return a pointer to the record part of the entry
        return (int8_t*) (prev->he_next->he_key + ht->ht_key_size);
    }
    else
    {
        return 0; // Not found
    }
}

/**
 * Searches for record.
 *
 * @param ht the hash table
 * @param key the key
 * @return a valid pointer to a record or 0 if not found. Note that the
 * pointer points into the hash table and so should not be modified.  Note
 * also that if you delete this record after finding it, this pointer is no
 * longer valid.
 */
void* ht_pvoid_find(Htable  ht,
                    int8_t* key)
{
    Hash_elem*  prev    = 0;
    int32_t     num     = 0;

    num = hash(ht, key);

    prev = ht_qfind(&ht->ht_table[num], key, ht->ht_key_size);

    if(prev)
    {
        // Since qfind returns a pointer to the previous item,
        // we must follow the link once.
        // Return a pointer to the record part of the entry
        return (void*) prev->he_next->he_pvoid;
    }
    else
    {
        return 0; // Not found
    }
}

/**
 * Wipe out the hash table. Free all of its memory.
 *
 * @param ht the hash table
 * @return void
 */
void ht_obliterate(Htable ht)

{
    int32_t     i       = 0;
    Hash_elem*  he      = 0;
    Hash_elem*  next    = 0;

    for(i = 0; i < ht->ht_size; i++)
    {
        he = (Hash_elem*) ht->ht_table[i].q_head;

        while(he)
        {
            next = he->he_next;

            if(he->he_pvoid)
                free(he->he_pvoid);

            free((char*) he);
            he = next;
       }
    }

    free((char*) ht->ht_table);
    free((char*) ht);
}

/**
 * Initializes the current bucket to zero and sets the next iteration
 * item to the queue's head.
 *
 * @param ht the hash table
 * @return void
 */
void ht_iter_setup(Htable ht)
{
    ht->ht_iter_current_bucket  = 0;
    ht->ht_iter_next_item       = (Hash_elem*) (ht->ht_table[0].q_head);
}

/**
 * Return all of the records in the hash table, in some random order.
 * While iterating, you may delete any record from the table
 * that has already been returned (including the most recent one).
 * ht_key_iter returns a char* p such that p points to the key and p + key_size
 * points to the record.
 *
 * @param ht the hash table
 * @return a key or zero when the iteration is complete
 */
int8_t* ht_key_iter(Htable ht)
{
    Hash_elem* he = 0;

    ht_iter_setup(ht);

    while(!ht->ht_iter_next_item)
    {
        ht->ht_iter_current_bucket++;

        if(ht->ht_iter_current_bucket == ht->ht_size)
            return 0;

        ht->ht_iter_next_item = (Hash_elem*) (ht->ht_table[ht->ht_iter_current_bucket].q_head);
    }

    he                      = ht->ht_iter_next_item;
    ht->ht_iter_next_item   = he->he_next;

    return((int8_t*) (he->he_key));
}

/**
 * Return all of the records in the hash table, in some random order.
 * While iterating, you may delete any record from the table
 * that has already been returned (including the most recent one).
 * ht_key_iter returns a char* p such that p points to the key and p + key_size
 * points to the record.
 *
 * @param ht the hash table
 * @return a key or zero when the iteration is complete
 */
void* ht_pvoid_iter(Htable ht)
{
    Hash_elem* he = 0;

    ht_iter_setup(ht);

    while(!ht->ht_iter_next_item)
    {
        ht->ht_iter_current_bucket++;

        if(ht->ht_iter_current_bucket == ht->ht_size)
            return 0;

        ht->ht_iter_next_item = (Hash_elem*) (ht->ht_table[ht->ht_iter_current_bucket].q_head);
    }

    he                      = ht->ht_iter_next_item;
    ht->ht_iter_next_item   = he->he_next;

    return((void*) (he->he_pvoid));
}

/**
 * Looks for a key's record in a queue.
 *
 * @param q the queue
 * @param key the key
 * @param size the key size
 * @return the previous record in the queue, which simplifies the delete process
 */
Hash_elem* ht_qfind(queue*  q,
                    int8_t* key,
                    int32_t size)
{
    Hash_elem*  qe      = 0;
    Hash_elem * prev    = 0;

    for(qe = (Hash_elem*)(q->q_head), prev = (Hash_elem*) (&q->q_head); qe; qe = qe->he_next)
    {
        if(!memcmp(key, qe->he_key, size))
            return prev;

        prev = qe;
    }

    // If we get here, we haven't found it
    return 0;
}

/**
 * Hashes a key.
 *
 * @param ht the hash table
 * @param key the key
 * @return the hashed item
 */
uint32_t hash(Htable    ht,
              int8_t*   key)
{
    uint32_t    hash_val    = 0;
    int32_t     j           = 0;

    for(hash_val = j = 0; j < ht->ht_key_size; j++, key++)
    {
        hash_val ^= *key * 23;   // ^ is XOR

        // Rotate so that permutations hash to different values
        hash_val = hash_rotl(hash_val);
    }

    return (uint32_t) (hash_val % ht->ht_size);
}

/*******************************************************************************
--  End, Dictionary implementation code.
*******************************************************************************/
#ifdef TEST_HASH
// To compile:  > gcc -o dict -std=c99 -DTEST_HASH hash.c
#define RECORD_SZ 64
#define KEY_SZ    64
static void (*function)(void*) = (void (*)(void *)) 0;

/**
 * Simply invokes the function given as the second parameter for each
 * node in the dictionary, passing it the key and the associated data.
 *
 * @param dict
 * @param func
 * @return void
 */
void dict_enum(Htable dict,
               void (*func)(int8_t*, int8_t*))
{
    int8_t* key     = 0;
    int8_t* record  = 0;
    int32_t cnt     = 0;

    while(key = ht_key_iter(dict))
    {
        cnt++;

        // print the key and the record
        func(key, key + KEY_SZ);

        ht_delete(dict, key);
    }
}

void printer(int8_t* key, int8_t* record)
{
    printf("%s: %s\n", (char*) key, (char*) record);
}

int main(void)
{
    Htable dict         = 0;

    int32_t size        = 6;
    int32_t key_size    = 64;
    int32_t rec_size    = 64;
    int32_t status      = false;
    int     i           = 0;
    int8_t* key         = 0;
    int8_t* rec         = 0;

    int8_t* keys[] = {(int8_t*) "The first key...\0",
                      (int8_t*) "The second key...\0",
                      (int8_t*) "The third key...\0",
                      (int8_t*) "The fourth key...\0",
                      (int8_t*) "The fifth key...a much longer key than the rest.\0",
                      (int8_t*) "The sixth and last key...\0",
                      (int8_t*) 0};

    int8_t* records[] = {(int8_t*) "The first record...\0",
                         (int8_t*) "The second record...\0",
                         (int8_t*) "The third record...\0",
                         (int8_t*) "The fourth record...\0",
                         (int8_t*) "The fifth record...\0",
                         (int8_t*) "The sixth record...a much longer record than the rest.\0"};

    dict = ht_create(size,
                     key_size,
                     rec_size);

    //-------------------
    printf("\n--- Inserting the key-record pairs ---\n");
    printf("--------------------------------------\n");

    for(i = 0; keys[i] != 0; i++ )
    {
        status = ht_insert(dict, keys[i], 0, records[i]);

        if(status == 0)
            printf("- calloc failed for this insertion: %d\n", i);
        else if(status == -1)
            printf("- record already exists.\n");
        else if(status == 1) // success
            /* no code */ ;
    }

    //-------------------
    printf("\n--- Enumerating and deleting the key-record pairs ---\n");
    printf("-----------------------------------------------------\n");

    for(i = 0; keys[i] != 0; i++)
    {
        // Prints then deletes the key-record enumeration one at a time
        dict_enum(dict, printer);
    }

    //-------------------
    printf("\n--- Verifying that all the kets were deleted ---\n");
    printf(" ------------------------------------------------\n");

    for(i = 0; keys[i] != 0; i++)
    {
        rec = ht_rec_find(dict, keys[i]);

        if(!rec)
            printf("Validated; %s was deleted.\n", keys[i]);
        else
            printf("Error; %s was deleted but is still in the dictionaty.\n", keys[i]);
    }

    ht_obliterate(dict);

    return 0;
}

#endif // TEST_HASH
