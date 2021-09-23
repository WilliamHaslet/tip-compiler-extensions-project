William Haslet (wlh4dh)
Ruthvik Gajjala (rrg5kq)

## Tricky aspects of the project
In our extention of the TIP parser, most of the problems we ran into
involved learning our way around the codebase and understanding the 
syntax of ANTLR grammar definitions. In the beginning, it was difficult 
to find the files that we needed to edit and to figure out how to run
the parser tests, but once we got a grasp on these details the actual 
implementation of the parser extensions went pretty smoothly. Our only 
major issue when writing the grammar was deciding where to place some 
of the extensions like increment and decrement, but we eventually
realized that they belonged with the statements and that most of the
other extention belonged in expressions.

## Design alternatives
When designing our tests, we had to choose between writing large tests 
that would check many of the parser extensions at once and writing
smaller tests that would focus on individal elements of the grammar.
We decided that using the smaller test case option would make debugging
the gammar easier because if a test case failed, we would immediaty know 
what element of the grammar had a problem. When designing the grammar, 
we choose not to define some operators like and and or in the lexicon
because they were only being used in one place and the extra definition 
would be unnecessary.

## Testing
Our approach to testing the parser extensions was to design small
test cases that would check that each newly added element of the 
grammar is being parsed correctly. By writing multiple tests that
covered various correct and incorrect use cases for each extention 
that we added to the grammar, we were able to make sure that almost 
all of the grammar extension's uses cases were covered. 
We based the structure of our tests on the tests from the
TIPParserTest.cpp file. We also removed the TIP Lexer tests 
that check for the mod operator and the less than or equal to 
operator because these operators are part of our parser extensions 
and should no longer cause errors when used.

## Coverage
As mentioned above, we attempted to use various different test cases
for each rule we added to the grammar. We covered the main use cases,
as well as some of the common error cases that could occur. This
coverage of each rule is summarized below.

Boolean types, not, and/or: For these rules, we tested that each
keyword could be used within valid expressions. For the boolean 
types, we had to ensure that true/false were accepted within the
grammar, so we made tests to support this. For the not
unary operator, since this is not a standalone expression, we tested
that it could be used appropriately with an additional expression,
as well as that it would not be parsable when used with a capital
"N" in the fail case. For and/or binary operators, we tested each
individually, as well as a test case combining both to make sure
that they are parsable. 

Arrays: We covered each main use case when it came to arrays. This
include the standard definition, the "of" usage, as well as the empty
array case. We also tested that array element reference and length
operators parse successfully. We also included tests for arrays with
missing brackets and commas, making sure that they fail the parse.

Modulo/Negation: For the modulo operator, we wrote a test with both
correct and incorrect use. For the negation operator, we wrote a test
to see if it parses correctly.

Relational Operators: For each relational operator we added, we wrote
an individual test to make sure that they are parsable. We also added
tests at the end of the file with cases on incorrect usage (such as an
additional greater than sign) to make sure that they fail the parse.

Ternary Operator: For the ternary operator, we checked that the syntax
can be adequately parsed in a correct usage test case.

Increment and Decrement: We included individual test cases for each of these
statements.

For Loops: We tested each of the iterator style and range style for loop in 
individual test cases. We also decided to add an additional test case for testing
the optional "by" argument in the range style for loop to better our coverage.


