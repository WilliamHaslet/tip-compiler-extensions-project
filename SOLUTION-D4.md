# Deliverable 4 #

William Haslet \
Ruthvik Gajjala

### Overview ###

For this deliverable, we impelmented codegen functions for ternary, arrays, of arrays, array element refrence, array length, increment, decrement, 
true, false, and, or, not, negation, for range loops, and for iterator loops. We also modified existing codegen functions to add support for 
greater then or equal to, less than, less then or equal to, and modulo. Next we added a command line option to disable type infrence and wrote 
system tests to validate and get coverage for of codegen functions.

### Tricky Parts of Our Solution ###

The most difficult part of our solution was getting the arrays working. We didn't know how we wanted to store them, but once we decided to just
allocate space and index in with get element pointer, the rest of the array operations became much easier. We were also intially confused as to how
we were going to access the array itself in the length and element reference expressions because in these cases we don't have explicit access to the
size and elements of the array. Once we realized it was as simple as generating code for the array before accessing elements or length. Other than that,
another tricky part was understanding the parameters that the LLVM IRBuilder functions took, as well as understanding what to return in each codegen
function.

### Design Decisions ###

As mentioned above, we decided to use a simple memory allocation to store our arrays. This made indexing simple, since all we had to do 
was use get element pointer with the array pointer and the index offset. We also choose to store the array length at the 0th index of 
the array's memory, which made the array length operator easy to implement. Another design decision we chose was to use branching in our
element reference expression in order to generate an execution error when the index was out of bounds. Specifically, the execution error will show
the index used that was considered to be out of bounds. 

### Testing and Coverage ###

We used system tests to check the correctness of our codegen functions and get coverage for them. Every line of our codegen functions is 
covered except for the error lines that are thrown when a codegen call return nullptr. We kept our tests small so that the features that 
each individual test tests are as few as possible. Our testing process is modeled after the iotests seen within the system tests. We created sip programs and checked that the actual output was equal to the expected output of our programs. Through this, we achieved excellent coverage over our codegen functions, and were able to verify that our compiler actually worked as intended. 
