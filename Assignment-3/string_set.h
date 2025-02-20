/* A 'string set' is defined as a set of strings stored
 * in a hashtable that uses chaining to resolve collisions.
 *
 * Peter Jensen and Richard Timpson
 * February 9, 2019
 */

#ifndef STRING_SET_H
#define STRING_SET_H

#include "node.h"
#include <vector>

namespace cs3505
{
class string_set
{
    friend class node;
    // Default visibility is private.

    node **table; // The hashtable, a pointer to a node pointer
                  //   (which will really be an array of node pointers)
    int capacity; // The size of the hashtable array
    int size;     // The number of elements in the set
    node *head;   // the head of the doubly linked list
    node *tail;   // the tail of the doubly linked list

  public:
    string_set(int capacity = 100);      // Constructor.  Notice the default parameter value.
    string_set(const string_set &other); // Copy constructor
    ~string_set();                       // Destructor

    void add(const std::string &target);            // Not const - modifies the object
    void remove(const std::string &target);         // Not const - modifies the object
    bool contains(const std::string &target) const; // Const - does not change the object
    std::vector<std::string> get_elements() const;
    int get_size() const; // Const - does not change object

    string_set &operator=(const string_set &rhs); // Not const - modifies this object

    static int creation_count;
    static int deletion_count;

  private:
    int hash(const std::string &s) const; // Const - does not change this object
    void clean();                         // Not const - modifies the object
};

} // namespace cs3505

#endif
