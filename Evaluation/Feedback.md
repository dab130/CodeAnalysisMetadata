# dab130
Score: 88

## Files & Changed Files
* Object and executable files added to repository. This had to be done via a command that you performed, i.e., ```git add ...```. This could have caused major problems with the build. Do not add object files and executables to source directories
* Proper files changed

## Build
* Successfully builds, no errors, no warnings

## Test
* Passes all user tests
* No extraneous output

## Test Oracle: code\_analysis\_t.cpp
* Fails a couple of tests in the oracle test suite with student's implementation

## Test Oracle: code\_analysis.cpp
* Fails an early test in the student's test suite with oracle implementation

## Code
* Necessary code appears present
* Place comments **before** the code they describe. As can be seen, very difficult to follow
* If immediately returning, why use nested if structure? Confusing.
* Line 4 of ```analysis_language()``` appears to be an else due to indentation and formatting, but is not. Separate your if-statements by a blank line.
* Indentation in ```analysis_url()``` is very inconsistent
* Sometimes you leave a blank line before the return, sometimes you don't. Why?
* Suggest space before start of if-then block and before else
* Strange block indentation in if-statement. Suggest indent closing '}' same as if.
* Non-ascii characters in comments

## Test Cases
* Good set of test cases
* Inconsistent indentation

## Literal Values used in Testing
* Realistic values used based on the problem description

## Error Messages
* Proper error messages
* Messages written to proper output
* Why the extra blank space on the one error message?

## Commits
* Initial adding of object files (*.o) and executables caused major problems in builds. In the project there were no new files to create, or to add to the repository. These files were explicitly added (```git add ...```).
* A lot of implementation added all at once (commit a5803a). This is **not** what TDD is about
* Tests and implementation code committed separately. Difficult to tell if TDD process followed with those. Suggest ```git squash``` if you want to use such fine-grained commits.

