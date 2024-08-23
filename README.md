All the public functions that this simple vector class has and what they do:

1) 4 different constructors (1. with no parameters, 2. with size as parameter, 3. with a ilist of values als parameter and 4. copy constructor)
2) Destructor
3) operator= -> swaps values between two vectors
4) pish_back -> generic push_back, adds element to the back
5) operator[] -> access element on specific index
6) size() -> returns number of elements
7) empty() -> true if vector is empty
8) clear() -> delete all elements and create new, default memory area
9) reserve(size_t n) -> expand memory area to desired value
10) shrink_to_fit() -> reduce memory area so that last element is at the end of memory space
11) pop_back() -> delete last element
12) capacity() -> returns memory size
13) print(ostream o) -> prints the whole vector
14) begin & end -> uses iterator class to return iterator to begin/end of memory space
15) insert -> insert element inside vector
16) erase -> erase specific element
