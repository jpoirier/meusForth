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
 *
 *
 * @cmd<<%PRJ% file=%PFE%>>
 */
#ifndef HASH_H
#define HASH_H

#include <stdint.h>
#include <stdbool.h>

//#define KEY_MAX_CHAR_CNT            33
//#define RECORD_MAX_CHAR_CNT

typedef int8_t* caddr_t;

typedef struct  _q_elt  {           // queue element: cast to right type
    struct  _q_elt*     qe_next;    // it's just a pointer to next elt
    caddr_t             qe_data;    // Pointer to stuff being stored
} q_elt;

typedef struct  _queue  {   // queue header
    q_elt*      q_head;     // first element in queue
    q_elt*      q_tail;     // last element in queue
    int32_t     q_len;      // number of elements in queue
    int32_t     q_max;      // maximum length
    int32_t     q_min;      // minimum length
    uint32_t    v_tild;     // OE for FQ
} queue;

typedef struct _Hash_elem {
    struct _Hash_elem*  he_next;        // Pointer for use by queue
    void*               he_pvoid;       // some user defined structure
    int8_t              he_key[];       // Start of the key here, the record
                                        // sits at the offset ht_key_size.
} Hash_elem;

typedef queue Hash_bucket;

typedef struct _Htable {
    int32_t         ht_size;
    int32_t         ht_key_size;
    int32_t         ht_rec_size;
    Hash_bucket*    ht_table;
    // Next two vars are used by ht_iter
    int32_t         ht_iter_current_bucket;
    Hash_elem*      ht_iter_next_item;
} *Htable;

#ifdef __cplusplus
extern "C" {
#endif

    extern Htable ht_create(int32_t size, int32_t key_size, int32_t rec_size);
    extern int32_t ht_insert(Htable ht, int8_t* key, void* pvoid, int8_t* rec);
    extern bool ht_delete(Htable ht, int8_t* key);
    extern void ht_obliterate(Htable ht);
    extern int8_t* ht_rec_find(Htable  ht, int8_t* key);
    extern void* ht_pvoid_find(Htable  ht, int8_t* key);
    extern int8_t* ht_key_iter(Htable ht);
    extern void* ht_pvoid_iter(Htable ht);

#ifdef __cplusplus
}
#endif

#endif   // HASH_H

// end of file

