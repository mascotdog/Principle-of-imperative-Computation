#pragma once

/* Client interface for stacks. */
/* See lib/stacks.c0 */

/* You can't say 'typedef move stackelem', because given the order in
 * which things are defined, we haven't defined what a move is yet */
#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>
typedef int stackelem;

/* Client side implementation for hashtables. (Optional) */
/* See lib/ht.c0 */

/* It is possible to correctly do peg3.c0 with both elements and keys
 * defined to be a "struct two_ints*". It's maybe not too obvious that
 * this is the case. It might be easier to think of how boards could
 * be represented as a struct containing *four* integers first. */

struct two_ints {
  int i1;
  int i2;
  int best_num_pegs;
};

typedef struct two_ints* htelem;
typedef struct two_ints* htkey;

// This is a terrible hashing implementation
int HASH(htkey k) //��Ҫ���ģ�
//@requires k != NULL;
{
  return k->i1 + k->i2;
}

// This is a reasonable key equality function
bool htkey_equal(htkey k1, htkey k2) 
//@requires k1 != NULL;
//@requires k2 != NULL;
{
  return k1->i1 == k2->i1 && k1->i2 == k2->i2;
}

// This is a reasonable key extraction function (they key is the whole elem!)
htkey htelem_key(htelem e) 
//@requires e != NULL;
{
  return e;
}
