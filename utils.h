#define   LJ_SWAP(type, a, b)  do { type temp = *(a); \
                                    *(a) = *(b); \
                                    *(b) = temp; } while (0);