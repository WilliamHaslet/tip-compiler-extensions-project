# Deliverable 3

William Haslet\
Ruthvik Gajjala

### Overview of what we added for deliverable 2

For deliverable three, we implemented type constraints for all the AST nodes that we added in the previous deliverable, added assignablity 
checking for the relevant AST nodes, and implemented new tip types for arrays and booleans. For all the new code we added, we wrote unit tests that check correct and incorrect uses of types and fully covered all the new code we wrote.

### Tricky aspects of our solution

The largest challenge we overcame in this deliverable was writing the array-related parts of the type contraints and finding bugs that appeared in 
this part of the code. We found it difficult to locate all the places in the existing code that needed to be changed with our new tip types and it also
took a while for us to learn how the process of type checking worked and how to add our new AST nodes into that system.

### Design alternatives and how we decided among them

We didn't have to make any huge design decisions in this deliverable since a solid foundation had been laid out in the base tip code that we 
extended. We only had to make smaller desisions about things like how to order the type constraint output, which we based off of the 
exisiting outputs.

### Approach to testing and how we achieved high coverage

We split our testing between the unifier tests and the type constraint collector tests. In each file, we based our own tests off of the 
exisiting tests for base tip. In the unifier tests, we added on-the-fly unification tests that check type safe sip functions and 
type unsafe sip functions for each of the AST nodes that we added to the type constraint visitor. We also added collected unification tests
for our new tip types, array and boolean, in order to get coverage of the accept function in their tip type files. In the type constraint 
collector tests, we tested a sip program's type constraint output against an expected output for each of our AST node types. With these
two types of tests, we were able to get code coverage of every new line of code that we wrote and the resulting coverage report 
has the same coverage as the origin repo, if not better.
