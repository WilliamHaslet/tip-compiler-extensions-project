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
