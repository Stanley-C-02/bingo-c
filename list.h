/**source: list.h
 * provides declarations of list ADT implementations
 * but also hides the implementation details
 */

typedef void *IntList;

IntList *initialize(int);
IntList *intList(int);
void add(IntList *, int);
void removeLast(IntList *);
int check(IntList *, int);
int contains(IntList *, int);
void print(IntList *);
void printInts(IntList *);
int size(IntList *);
