# Deliverable 2

William Haslet\
Ruthvik Gajjala

### Overview of what we added for deliverable 2

For deliverable two we implemented AST node classes and expanded existing AST node classes for true, false, not, and, or, array expressions, of array expressions, array access, array length, modulo, negation, greater than or equal to, less than, less than or equal to, the ternary operator, increment, decrement, iterator for loops, and range for loops. After adding the new AST nodes, we added visit functions to the AST builder and expanded the pretty printer with functions for our new node types. We added tests for each of our AST nodes using both pretty printer tests, AST printer tests.

### Tricky aspects of our solution

When we first started on deliverable two, the most difficult part was understanding what parts of the project needed to be changed. As we implemented AST nodes and learned our way around the code base, we began to understand how each part of the AST came together. When writing tests for our AST nodes, it was tricky to decide how to get the required coverage for the node’s print functions, and we ended getting the coverage by adding tests to the ASTPrinterTest.cpp file.

### Design alternatives and how we decided among them

When implementing some of the AST nodes, we had to make decisions about adding the expression into an existing node type like binary expression, or if we should make it a new node instead. For modulo, we decided to add it to the existing binary expression, but for unary operators like not and negation, we found it easier to just add them as separate node types.

### Approach to testing and how we achieved high coverage

We chose to do our testing and get coverage using pretty printer tests and AST printer tests. By using the pretty printer tests, we were able to get coverage of the accept functions, AST builder functions, and pretty printer functions of each of our new node types. By using the AST printer tests, we were able to get coverage of the print functions in the AST nodes that we added. The coverage for each of our new AST nodes is around 80% because we do not yet cover the codegen function, but every line other than those is covered.
