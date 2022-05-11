# utils.h - functions and macros for basic utilities
`utils.h` contains macros and functions used for (very) basic utilities, like swapping things around, copying memory, and comparing regions of memory.

---

## LJ_SWAP - swap two values in memory
`LJ_SWAP(type, a, b)` is a macro to swap the values of two memory locations.

**parameters**: 
- `type` is the type of the things being swapped
- `a` and `b` are pointers to values of that type

**usage example**:

```c
int i = 63;
int j = 45;
LJ_SWAP(int, &i, &j);
lj_assert(i == 45);
lj_assert(j == 63);
```

**usage notes**: not entirely hygienic! if `a` and `b` are expressions that have side effects or if `type` is incorrect, the resulting behavior is undefined.

**warnings**: like all macros, *there are no checks to guarantee inputs are valid*. use at your own risk. on the plus side, if all inputs are valid, then there are no known failure states.

**tags**: #macro, #generic, #utils