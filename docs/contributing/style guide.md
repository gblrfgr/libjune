# libjune overarching style guide
there are many stylistic guidelines and rules that cover all header files in libjune. most of them are kind of just my personal style. as many as i can remember are listed here.

---

## overall zen
### libjune must be portable
libjune must be c99-compliant through and through. that means *no inline assembly*. i know. it's horrifying. "but how do we make saturated adds faster on arm?", i hear you cry. the short answer is you don't. the needs of the many outweigh the needs of the few. your code needs to work in msvc, clang, gcc, and the c compiler you wrote for a comp sci project in college.

### you are not cool
i get it. you really want to put in that one sam & max reference. or spaceballs. or heartstopper. but libjune must be portable, and pop culture references are not portable. you are also not particularly clever. you are not better than other people for writing code like it's the linux kernel. the linux kernel wishes it was libjune. 

### silence speaks volumes
the c language is lacking in a lot of things. sometimes, to communicate something, we have to leave something important out. if the radio announcer always says "blue skies today", you will notice when he does not. use this effect to your advantage.

### errors should be tracked
if a function can enter an error state, it should absolutely report it somehow. the user has a right to know their call failed.

### prefer pure functions
any function that modifies global state or does not always return the same value for a given set of inputs should be treated with caution. ideally, anything dealing with state should be segregated into functions specifically for that purpose and it should be mentioned in their documentation.

## naming guidelines
### general
all constants and macros are in `SCREAMING_SNAKE_CASE`. everything else is in `non_screaming_snake_case`. all global identifiers intended for outside use are prefixed with `lj_`, while those intended only for internal use are prefixed with `lji_`. no identifier should begin or end with an underscore.

**examples:** `lj_str_is_big`, `LJ_SWAP`, `LJ_RATIONAL_PI_K`

**rationale**: keep a consistent visual vibe. both macros and constants are uppercased because [[#all constants are macros]]. prefixes are also used because c doesn't have namespaces, and `lji_` is used to signal that something maybe shouldn't be used.

### constants
all constants have the postfix `_K`.

**examples**: `LJ_RATIONAL_PI_K`, `LJ_RATIONAL_E_K`

**rationale**: keep a visual distinction between constant and non-constant macros.

### functions
all functions should have descriptive names. if it returns a `bool`, it should probably read like a predicate. if it operates primarily on a particular data type, that type should be mentioned in the name. do try to balance this against conciseness, though. 

**examples**: `lj_str_is_big`, `lji_bigstr_expand`, `lj_rational_sqrt`

**rationale**: makes code easier to read and understand. unfortunately, this isn't ruby. we don't get to put `?` or `!` at the end of function names. instead, we have to actually communicate these concepts directly (although 'this function modifies its arguments' *should* be clear from the definition, since [[#everything is const by default]]). overly long function names get unwieldy, though.

### types
all types end with `_t` or `_T`. whether something is a `struct`, a `union`, a `typedef`, or even a macro should not be indicated in the name of the function unless absolutely necessary.

**examples**: `LJ_VECTOR_T(int)`, `lj_string_t`, `lj_rational_t`

**rationale**: common practice in c. helps distinguish builtin types from user-defined ones. some types are meant to be opaque; the end user shouldn't need to know, for example, if `lj_string_t` is a `struct` or a `union`. they should call functions for that. it's okay. [[#inline your wicked one liners|those functions can be inlined]].

**side notes**: yes, some types are macros. c doesn't natively support generics, except through [[#twisted macro magic]]. we are twisted macro magicians, mostly because `hoard.h` needed to exist. it's just such a great name for a collections library.

### global variables
all global variables are in namespace `lji_` for encapsulation purposes. [[#globals should be opaque]]. beyond that, though, they have the same naming procedures as functions.

**examples**: `lji_crash_handlers`, `lji_waterboy_status`

**rationale**: globals should be avoided as much as possible, ideally. when we do have them, though, it's best to just treat them like normal variables.

## whitespace & curly braces
### block-level
all blocks have curly braces. all blocks are indented four spaces beyond the previous block. yes, all of them. "what if it's a wicked one-liner, though?" not if it isn't readable, it's not. i have had to take programming tests where they intentionally removed braces or messed with indentation to make things harder to read. [[#you are not cool]].

**example**:

```c
while (x != 3) {
	for (unsigned int i = 0; i < 10000; i++) {
		x++;
	}
}

if (lji_str_is_big(s)) {
	printf(lj_str_cstr(&s));
}
printf("\n");
```

as opposed to

```c
while (x != 3)
	for (unsigned int i = 0; i < 10000; i++)
		x++;

if (lji_str_is_big(s))
	printf(lj_str_cstr(&s));
	printf("\n"); // executes no matter what!
```

**rationale**: readability and ease of analysis. also, i'm a python programmer born and raised. i sometimes have to remind myself that indentaton *isn't* actually syntactically important in c.