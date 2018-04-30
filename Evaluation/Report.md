
# dab130

## Files
% ls

    Makefile
    README.md
    analysis_request.hpp
    code_analysis.cpp
    code_analysis.hpp
    code_analysis_t.cpp
    get_language_from_filename.cpp
    get_language_from_filename.hpp
    get_language_from_filename_t.cpp

## Changed files
% git diff baa2b2fa58cb52ada94597a9b2e5d687f6a3261f --name-only

    code_analysis.cpp
    code_analysis_t.cpp

## Build
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

## Test
% make test

    ./code_analysis_t
    Using stdin requires a declared language
    Extension not supported 
    Extension not supported 

## Test Oracle: code\_analysis\_t.cpp
% cp ~collard/CodeAnalysisMetadataEvaluation/code_analysis_t.cpp .  
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% make test

    ./code_analysis_t
    ./code_analysis_t: code_analysis_t.cpp:62: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:79: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:96: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:110: main Check `filename == ""' failed.
    Extension not supported 
    ./code_analysis_t: code_analysis_t.cpp:130: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:147: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:164: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:181: main Check `code_analysis(request) == true' failed.

## Test Oracle: code\_analysis.cpp
% cp ~collard/CodeAnalysisMetadataEvaluation/code_analysis.cpp  .  
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% make test

    ./code_analysis_t
    Extension not supported
    Assertion failed: (code_analysis(request) == false), function main, file code_analysis_t.cpp, line 46.
    make: *** [test] Abort trap: 6

## Code
% srcml code_analysis.cpp code_analysis_t.cpp -o project.xml
% srcml --xpath="//src:function[src:name='code_analysis']" project.xml | srcml

     1 bool code_analysis(const analysis_request& request) {
     2 
     3     auto filename = analysis_filename(request);
     4 
     5     auto url = analysis_url(request);
     6 
     7     auto language = analysis_language(request, filename);
     8 
     9     // code analysis processing that is not yet implemented
    10     if(request.given_filename != "" && language == ""){
    11        std::cerr << "Extension not supported " << std:: endl;
    12        return false;
    13        }
    14     if(request.entry_filename != "" && language == ""){
    15        std::cerr << "Extension not supported " << std:: endl;
    16        return false;
    17        }
    18     if(request.option_filename != "" && language == ""){
    19        std::cerr << "Using stdin requires a declared language" << std::endl;
    20        return false;
    21        }	
    22 
    23     return false;
    24 }

% srcml --xpath="//src:function[src:name='analysis_filename']" project.xml | srcml

     1 std::string analysis_filename(const analysis_request& request) {
     2     if(request.option_filename != ""){
     3        return request.option_filename; 				// if there is a stdin value return it.
     4        
     5     }else if(request.entry_filename == "data"){
     6        return request.given_filename; 					// this will return a file on the disk. 
     7                                      // if there isn't a stdin value and the data is not source code
     8     }else if(request.entry_filename != "" && request.option_language == ""){
     9        return request.entry_filename; 
    10                                      //if there isn't a stdin value and the data is source code		
    11     }else if(request.given_filename != "" && request.option_language == ""){
    12        return request.given_filename; 				
    13        
    14     }								   
    15     
    16     return "";
    17 }

% srcml --xpath="//src:function[src:name='analysis_url']" project.xml | srcml

     1 std::string analysis_url(const analysis_request& request) {
     2     if(request.option_url != ""){
     3          return request.option_url;
     4     }else if(request.given_url != ""){
     5          return request.given_url;
     6        }
     7        
     8        return "";
     9 }

% srcml --xpath="//src:function[src:name='analysis_language']" project.xml | srcml

     1 std::string analysis_language(const analysis_request& request, const std::string& filename) {
     2     if(request.option_language != ""){
     3        return request.option_language;
     4     }if(filename != ""){
     5        return get_language_from_filename(filename); 
     6     }
     7     return ""; 
     8 }

## Test Cases 
% srcml code_analysis_t.cpp --xpath="//src:function[src:name='main']/src:block" | srcml

    {
    
        // all parts of the request are empty
        {
            analysis_request request;
            request.given_filename  = "";
            request.entry_filename  = "";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "";
    
            auto filename = analysis_filename(request);
            assert(filename == "");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "");
            assert(code_analysis(request) == false);
        }
        {
           // Test case: option_filename 
           // The filename can be given explicitly as the option_filename, and this supersedes any other way of determining the filename.
           // This test will return the file type "C++" for option_filename over given_filename and entry_filename.
            analysis_request request;
            request.given_filename  = "tester.aj";
            request.entry_filename  = "testing.java";
            request.given_url       = "";
            request.option_filename = "test.cpp";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "test.cpp");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
           
        }
        {	
           // Test case: analysis_url
           // The url is the given_url for a directory and a URL. There is an option_url, which takes precedence.
           //This will return www.google.com over www.github.com
           analysis_request request;
           request.given_filename  = "";
            request.entry_filename  = "";
            request.given_url       = "www.github.com";
            request.option_filename = "";
            request.option_url      = "www.google.com";
            request.option_language = "";
           
           auto filename = analysis_filename(request);
           assert(filename == "");
            assert(analysis_url(request) == "www.google.com");
            assert(analysis_language(request, filename) == "");
            assert(code_analysis(request) == false);
        }
        {
           // Test case: analysis_language
           // The language can be explicitly given as an option (option_language) and this supersedes all other language settings
           analysis_request request;
           request.given_filename  = "testing.java";
            request.entry_filename  = "tester.aj";
            request.given_url       = "";
            request.option_filename = "tested.cs";
            request.option_url      = "";
            request.option_language = "c++";
           
           auto filename = analysis_filename(request);
           assert(filename == "tested.cs");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "c++");
            assert(code_analysis(request) == false);
        }
        {
           // Test case: analysis_language
           // The language is based on the file extension. The completed, provided function, get_language_from_filename(), is used for this.
           // This is a pass test case that will return C++ from the given option_filename
           analysis_request request;
           request.given_filename  = "testing.aj";
            request.entry_filename  = "test.java";
            request.given_url       = "";
            request.option_filename = "test.cpp";
            request.option_url      = "";
            request.option_language = "";
           
           auto filename = analysis_filename(request);
           filename = analysis_filename(request);
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
        }
        {
           //Test case: given_filename
           //The given_filename is the name of the file on the disk. For a source-code archive the entry_filename is used instead.		
            analysis_request request;
            request.given_filename  = "test.cpp";
            request.entry_filename  = "tester.aj";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "tester.aj");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "AspectJ");
            assert(code_analysis(request) == false); 
           
        }
        {
           //Test case: entry_filename
           //For a regular file the entry_filename is the literal string ‚M-^@M-^\data‚M-^@M-^], and you should use the given_filename
            analysis_request request;
            request.given_filename  = "test.cpp";
            request.entry_filename  = "data";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "test.cpp");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
           
        }
        {
           //Test case: given_filename with a dash
           //The given_filename is a single dash ‚M-^@M-^\-‚M-^@M-^] when the input is from standard input.
            analysis_request request;
            request.given_filename  = "-";
            request.entry_filename  = "";
            request.given_url       = "";
            request.option_filename = "testing.cpp";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "testing.cpp");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
           
        }
        {
           //Test case: given_filename with a dash as a non source code archive.
           //Note that the entry_filename is not valid (i.e, ‚M-^@M-^\data‚M-^@M-^]) for a non source code archive,
           //and the option_filename must be used. 
            analysis_request request;
            request.given_filename  = "-";
            request.entry_filename  = "data";
            request.given_url       = "";
            request.option_filename = "testing.cpp";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "testing.cpp");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
           
        }
        {
           //Test case:  If the option_filename is blank (i.e., ‚M-^@M-^\‚M-^@M-^]), then the resulting filename is blank,
           //and the language is based on the option_language.
            analysis_request request;
            request.given_filename  = "testing.aj";
            request.entry_filename  = "test.java";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "C++";
           
            auto filename = analysis_filename(request);
            assert(filename == "");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "C++");
            assert(code_analysis(request) == false);
           
        }
        {
           //Test case: When the input is from standard input and a language cannot be determined, 
           //output the error message ‚M-^@M-^\Using stdin requires a declared language‚M-^@M-^]	
            analysis_request request;
            request.given_filename  = "";
            request.entry_filename  = "";
            request.given_url       = "";
            request.option_filename = "test.cpsdf";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "test.cpsdf");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "");
            assert(code_analysis(request) == false);
           
        }
        {
           //Test case: Error handling If the file extension is used to determine the language, 
           //and there is no mapping for that language, output the error message ‚M-^@M-^\Extension not supported‚M-^@M-^]		
            analysis_request request;
            request.given_filename  = "test.cpsdf";
            request.entry_filename  = "";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "test.cpsdf");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "");
            assert(code_analysis(request) == false);
           
        }
        {
           //Test case: Error handling If the file extension is used to determine the language, 
           //and there is no mapping for that language, output the error message ‚M-^@M-^\Extension not supported‚M-^@M-^]		
            analysis_request request;
            request.given_filename  = "";
            request.entry_filename  = "test.cpsdf";
            request.given_url       = "";
            request.option_filename = "";
            request.option_url      = "";
            request.option_language = "";
           
            auto filename = analysis_filename(request);
            assert(filename == "test.cpsdf");
            assert(analysis_url(request) == "");
            assert(analysis_language(request, filename) == "");
            assert(code_analysis(request) == false);
           
        }
    
        return 0;
    }

## Literal Values used in Testing
% srcml code\_analysis_t.cpp --xpath="//src:literal" | srcml | tr '\0' '\n' |grep '"' | sort -u | tr '\n' ','

    "-" "AspectJ" "C++" "c++" "data" "test.cpp" "test.cpsdf" "test.java" 
    "tested.cs" "tester.aj" "testing.aj" "testing.cpp" "testing.java" 
    "www.github.com" "www.google.com" 

## Error Messages
% srcml code_analysis.cpp --xpath="//src:function[src:name='code_analysis']//src:expr[contains(., 'cerr')]//src:literal" | srcml | tr '\0'  '\n'

    "Extension not supported "
    "Extension not supported "
    "Using stdin requires a declared language"

## Commits

### Commit 7ea1a5
% git checkout -q 7ea1a5  
% make  

    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    ld: warning: ld: warning: ignoring file code_analysis.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis.oignoring file code_analysis_t.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis_t.o
    
    Undefined symbols for architecture x86_64:
      "_main", referenced from:
         implicit entry/start for main executable
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [code_analysis_t] Error 1

% git show

    commit 7ea1a5b55d35a09f936f754573f05246949c2ef0
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 09:58:24 2018 -0400
    
        Added first test case for option_filename. failed assertion
    
    diff --git a/code_analysis.o b/code_analysis.o
    new file mode 100644
    index 0000000..78c1203
    Binary files /dev/null and b/code_analysis.o differ
    diff --git a/code_analysis_t b/code_analysis_t
    new file mode 100755
    index 0000000..8a1b58f
    Binary files /dev/null and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 820446d..e744ee4 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -27,6 +27,25 @@ int main() {
             assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);
         }
    +	{
    +		// Test case: option_filename 
    +		// The filename can be given explicitly as the option_filename, and this supersedes any other way of determining the filename.
    +		// This test will return the file type "C++" for option_filename over given_filename and entry_filename.
    +        analysis_request request;
    +        request.given_filename  = "tester.aj";
    +        request.entry_filename  = "testing.java";
    +        request.given_url       = "";
    +        request.option_filename = "test.cpp";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "test.cpp");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    new file mode 100644
    index 0000000..949dfda
    Binary files /dev/null and b/code_analysis_t.o differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    new file mode 100644
    index 0000000..f4be325
    Binary files /dev/null and b/get_language_from_filename.o differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    new file mode 100755
    index 0000000..189eb94
    Binary files /dev/null and b/get_language_from_filename_t differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    new file mode 100644
    index 0000000..ad2e2e1
    Binary files /dev/null and b/get_language_from_filename_t.o differ


### Commit 7e33c0
% git checkout -q 7e33c0  
% make  

    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    ld: warning: ignoring file code_analysis_t.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis_t.o
    ld: warning: ignoring file code_analysis.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis.o
    Undefined symbols for architecture x86_64:
      "_main", referenced from:
         implicit entry/start for main executable
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [code_analysis_t] Error 1

% git show

    commit 7e33c0514797658ca0459a2750ee11959710bbdb
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:00:19 2018 -0400
    
        test
    
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 189eb94..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ad2e2e1..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit a5803a
% git checkout -q a5803a  
% make  

    make: Nothing to be done for `all'.

% git show

    commit a5803a36dcd4d28eb06b8259c9d7c53cf9ba3292
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:09:34 2018 -0400
    
        added code in function analysis_filename. Got a new assertion error.
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index b874829..5bbc48a 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -32,7 +32,20 @@ bool code_analysis(const analysis_request& request) {
      * @retval filename
      */
     std::string analysis_filename(const analysis_request& request) {
    -
    +	if(request.option_filename != ""){
    +		return request.option_filename; 				// if there is a stdin value return it.
    +		
    +	}else if(request.entry_filename == "data"){
    +		return request.given_filename; 					// this will return a file on the disk. 
    +														// if there isn't a stdin value and the data is not source code
    +	}else if(request.entry_filename != "" && request.option_language == ""){
    +		return request.entry_filename; 
    +														//if there isn't a stdin value and the data is source code		
    +	}else if(request.given_filename != "" && request.option_language == ""){
    +		return request.given_filename; 				
    +		
    +	}								   
    +	return "";
         return "";
     }
     
    diff --git a/code_analysis.o b/code_analysis.o
    index 78c1203..e25ca89 100644
    Binary files a/code_analysis.o and b/code_analysis.o differ
    diff --git a/code_analysis_t b/code_analysis_t
    index 8a1b58f..405c545 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    new file mode 100755
    index 0000000..189eb94
    Binary files /dev/null and b/get_language_from_filename_t differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    new file mode 100644
    index 0000000..ad2e2e1
    Binary files /dev/null and b/get_language_from_filename_t.o differ


### Commit ded867
% git checkout -q ded867  
% make  

    make: Nothing to be done for `all'.

% git show

    commit ded867bf3a1f1fa5d5648a556c4739f5b9b6f4fd
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:11:28 2018 -0400
    
        removed return value for assertion for analysis_language. Did not impliment function yet. Code will compile without errors.
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 405c545..f903ba5 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index e744ee4..922f35b 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -42,7 +42,7 @@ int main() {
             auto filename = analysis_filename(request);
             assert(filename == "test.cpp");
             assert(analysis_url(request) == "");
    -        assert(analysis_language(request, filename) == "C++");
    +        assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);
     		
     	}
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 949dfda..07ebe36 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 5f67ef
% git checkout -q 5f67ef  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 5f67ef67273bfe5e18d446ec0819b6f0282b3753
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:15:01 2018 -0400
    
        added new test case for analysis_url. No code implimented in function yet. Fails assertion as of now.
    
    diff --git a/code_analysis_t b/code_analysis_t
    index f903ba5..46c4dee 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 922f35b..3056d12 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -46,6 +46,24 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{	
    +		// Test case: analysis_url
    +		// The url is the given_url for a directory and a URL. There is an option_url, which takes precedence.
    +		//This will return www.google.com over www.github.com
    +		analysis_request request;
    +		request.given_filename  = "";
    +        request.entry_filename  = "";
    +        request.given_url       = "www.github.com";
    +        request.option_filename = "";
    +        request.option_url      = "www.google.com";
    +        request.option_language = "";
    +		
    +		auto filename = analysis_filename(request);
    +		assert(filename == "");
    +        assert(analysis_url(request) == "www.google.com");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 07ebe36..6ab7846 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit ef9284
% git checkout -q ef9284  
% make  

    make: Nothing to be done for `all'.

% git show

    commit ef9284f1329161095d66822ded8448bd80aa6be5
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:17:45 2018 -0400
    
        added if statements in analysis_url function. Code currently compiles and passes test cases.
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 5bbc48a..fb4d4e9 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -45,7 +45,7 @@ std::string analysis_filename(const analysis_request& request) {
     		return request.given_filename; 				
     		
     	}								   
    -	return "";
    +	
         return "";
     }
     
    @@ -54,8 +54,13 @@ std::string analysis_filename(const analysis_request& request) {
      * @retval URL
      */
     std::string analysis_url(const analysis_request& request) {
    -
    -    return "";
    +	if(request.option_url != ""){
    +			return request.option_url;
    +	}else if(request.given_url != ""){
    +			return request.given_url;
    +		}
    +		
    +		return "";
     }
     
     /** Language extracted from the request and the filename
    diff --git a/code_analysis.o b/code_analysis.o
    index e25ca89..7f049f9 100644
    Binary files a/code_analysis.o and b/code_analysis.o differ
    diff --git a/code_analysis_t b/code_analysis_t
    index 46c4dee..2033bf2 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ


### Commit f0d025
% git checkout -q f0d025  
% make  

    make: Nothing to be done for `all'.

% git show

    commit f0d0256b140df10173c922d3aa4f89488684a569
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:20:04 2018 -0400
    
        added new test case for anaylsis_language. Currently fails assertions because funtion does not have code yet.
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 2033bf2..0de4b0f 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 3056d12..c033478 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -64,6 +64,23 @@ int main() {
             assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);
     	}
    +	{
    +		// Test case: analysis_language
    +		// The language can be explicitly given as an option (option_language) and this supersedes all other language settings
    +		analysis_request request;
    +		request.given_filename  = "testing.java";
    +        request.entry_filename  = "tester.aj";
    +        request.given_url       = "";
    +        request.option_filename = "tested.cs";
    +        request.option_url      = "";
    +        request.option_language = "c++";
    +		
    +		auto filename = analysis_filename(request);
    +		assert(filename == "tested.cs");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "c++");
    +        assert(code_analysis(request) == false);
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 6ab7846..6e77b44 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit d9abab
% git checkout -q d9abab  
% make  

    make: Nothing to be done for `all'.

% git show

    commit d9ababdc1d80979f5ff3da6a51a42399af7c11ec
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:25:19 2018 -0400
    
        analysis_langauge function is failing assertion in other test cases. Fixing now
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index fb4d4e9..8e110a5 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -69,6 +69,10 @@ std::string analysis_url(const analysis_request& request) {
      * @retval language
      */
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
    -
    -    return "";
    +	if(request.option_language != ""){
    +		return request.option_language;
    +	}if(filename != ""){
    +		return get_language_from_filename(filename); 
    +	}
    +	return ""; 
     }
    diff --git a/code_analysis.o b/code_analysis.o
    index 7f049f9..81dca60 100644
    Binary files a/code_analysis.o and b/code_analysis.o differ
    diff --git a/code_analysis_t b/code_analysis_t
    index 0de4b0f..aeda1cc 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ


### Commit da02f1
% git checkout -q da02f1  
% make  

    make: Nothing to be done for `all'.

% git show

    commit da02f1fff8d4b3fad3ca717fca160ccb4bc93c7b
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:28:54 2018 -0400
    
        added the C++ back to line 45 so that test now will pass with updated code in the analysis_language function
    
    diff --git a/code_analysis_t b/code_analysis_t
    index aeda1cc..9460909 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index c033478..f1a411b 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -42,7 +42,7 @@ int main() {
             auto filename = analysis_filename(request);
             assert(filename == "test.cpp");
             assert(analysis_url(request) == "");
    -        assert(analysis_language(request, filename) == "");
    +        assert(analysis_language(request, filename) == "C++");
             assert(code_analysis(request) == false);
     		
     	}
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 6e77b44..b3a960f 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit b16887
% git checkout -q b16887  
% make  

    make: Nothing to be done for `all'.

% git show

    commit b168876932a7fad56eb5a348aa85ffbff91e7e06
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:31:37 2018 -0400
    
        added another test case for analysis_language. Language is base on file ext
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 9460909..f79e9fc 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index f1a411b..3b2179a 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -81,6 +81,24 @@ int main() {
             assert(analysis_language(request, filename) == "c++");
             assert(code_analysis(request) == false);
     	}
    +	{
    +		// Test case: analysis_language
    +		// The language is based on the file extension. The completed, provided function, get_language_from_filename(), is used for this.
    +		// This is a pass test case that will return C++ from the given option_filename
    +		analysis_request request;
    +		request.given_filename  = "testing.aj";
    +        request.entry_filename  = "test.java";
    +        request.given_url       = "";
    +        request.option_filename = "test.cpp";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +		auto filename = analysis_filename(request);
    +		filename = analysis_filename(request);
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index b3a960f..3d67f30 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 0cb3d8
% git checkout -q 0cb3d8  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 0cb3d8fed60a99d9112f4997c29ed0503593442e
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:33:58 2018 -0400
    
        added test case for analysis_filename. Needed to test the given_filename
    
    diff --git a/code_analysis_t b/code_analysis_t
    index f79e9fc..3fbc5f4 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 3b2179a..c50fd29 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -99,6 +99,24 @@ int main() {
             assert(analysis_language(request, filename) == "C++");
             assert(code_analysis(request) == false);
     	}
    +	{
    +		//Test case: given_filename
    +		//The given_filename is the name of the file on the disk. For a source-code archive the entry_filename is used instead.		
    +        analysis_request request;
    +        request.given_filename  = "test.cpp";
    +        request.entry_filename  = "tester.aj";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "tester.aj");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "AspectJ");
    +        assert(code_analysis(request) == false); 
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 3d67f30..e1c767a 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 7b57e2
% git checkout -q 7b57e2  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 7b57e2f3d5c8b9a922f1d651dc4efa4cfcfe255a
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:38:55 2018 -0400
    
        added test case for entry_filename
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 3fbc5f4..8f1910e 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index c50fd29..8b4fd54 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -117,6 +117,24 @@ int main() {
             assert(code_analysis(request) == false); 
     		
     	}
    +	{
    +		//Test case: entry_filename
    +		//For a regular file the entry_filename is the literal string ‚Äúdata‚Äù, and you should use the given_filename
    +        analysis_request request;
    +        request.given_filename  = "test.cpp";
    +        request.entry_filename  = "data";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "test.cpp");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index e1c767a..6a11c76 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 3bc365
% git checkout -q 3bc365  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 3bc365c0ea641ac9dd6479c7d90d597a20945c0a
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:40:30 2018 -0400
    
        added test case for when option_filename and given_filename has a dash
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 8f1910e..7f0e1d3 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 8b4fd54..ac11fe6 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -135,6 +135,24 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{
    +		//Test case: given_filename with a dash
    +		//The given_filename is a single dash ‚Äú-‚Äù when the input is from standard input.
    +        analysis_request request;
    +        request.given_filename  = "-";
    +        request.entry_filename  = "";
    +        request.given_url       = "";
    +        request.option_filename = "testing.cpp";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "testing.cpp");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 6a11c76..6db87da 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 304ac8
% git checkout -q 304ac8  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 304ac875d8e5aa56c3b96e36367ace367f828c17
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:42:01 2018 -0400
    
        added Test case: given_filename with a dash as a non source code archive.
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 7f0e1d3..dc016b7 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index ac11fe6..6168a9f 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -153,6 +153,25 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{
    +		//Test case: given_filename with a dash as a non source code archive.
    +		//Note that the entry_filename is not valid (i.e, ‚Äúdata‚Äù) for a non source code archive,
    +		//and the option_filename must be used. 
    +        analysis_request request;
    +        request.given_filename  = "-";
    +        request.entry_filename  = "data";
    +        request.given_url       = "";
    +        request.option_filename = "testing.cpp";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "testing.cpp");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 6db87da..3ef5edb 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 5960cf
% git checkout -q 5960cf  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 5960cf605e4da8166de3371e96820633dcd789c6
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:43:38 2018 -0400
    
        added Test case:  If the option_filename is blank (i.e., ‚Äú‚Äù), then the resulting filename is blank,
    
    diff --git a/code_analysis_t b/code_analysis_t
    index dc016b7..9a49a98 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 6168a9f..a6654ce 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -172,6 +172,24 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{
    +		//Test case:  If the option_filename is blank (i.e., ‚Äú‚Äù), then the resulting filename is blank,
    +		//and the language is based on the option_language.
    +        analysis_request request;
    +        request.given_filename  = "testing.aj";
    +        request.entry_filename  = "test.java";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "C++";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "C++");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 3ef5edb..96458a0 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 6e1ad0
% git checkout -q 6e1ad0  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 6e1ad03cdde5cd9de2c0a597e57da1a159058b1c
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:52:40 2018 -0400
    
        added test case for when the input is from standard input and a language cannot be determined,
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 9a49a98..8aa0fdb 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index a6654ce..9ea82a0 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -190,6 +190,24 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{
    +		//Test case: When the input is from standard input and a language cannot be determined, 
    +		//output the error message ‚ÄúUsing stdin requires a declared language‚Äù	
    +        analysis_request request;
    +        request.given_filename  = "";
    +        request.entry_filename  = "";
    +        request.given_url       = "";
    +        request.option_filename = "test.cpsdf";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "test.cpsdf");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 96458a0..0f77104 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 95f828
% git checkout -q 95f828  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 95f8289af6453b7d3bcb71f970eef6d45036ad3a
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:56:40 2018 -0400
    
        added code inside code_analysis to display error handling messages.
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 8e110a5..bf2d9e7 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -8,6 +8,7 @@
     #include "get_language_from_filename.hpp"
     
     #include <string>
    +#include <iostream>
     
     /** Generate source analysis based on the request
      * @param request Data that forms the request
    @@ -23,6 +24,18 @@ bool code_analysis(const analysis_request& request) {
         auto language = analysis_language(request, filename);
     
         // code analysis processing that is not yet implemented
    +	if(request.given_filename != "" && language == ""){
    +			std::cerr << "Extension not supported " << std:: endl;
    +			return false;
    +		}
    +		if(request.entry_filename != "" && language == ""){
    +			std::cerr << "Extension not supported " << std:: endl;
    +			return false;
    +		}
    +		if(request.option_filename != "" && language == ""){
    +			std::cerr << "Using stdin requires a declared language" << std::endl;
    +			return false;
    +		}	
     
         return false;
     }
    diff --git a/code_analysis.o b/code_analysis.o
    index 81dca60..7b6abe3 100644
    Binary files a/code_analysis.o and b/code_analysis.o differ
    diff --git a/code_analysis_t b/code_analysis_t
    index 8aa0fdb..213c09f 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ


### Commit 63407c
% git checkout -q 63407c  
% make  

    make: Nothing to be done for `all'.

% git show

    commit 63407c262cc6d8306c6fcf15e05545636f55c47d
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 10:58:49 2018 -0400
    
        added Error handling If the file extension is used to determine the language,
    
    diff --git a/code_analysis_t b/code_analysis_t
    index 213c09f..5e81089 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 9ea82a0..2dfbd17 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -208,6 +208,24 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{
    +		//Test case: Error handling If the file extension is used to determine the language, 
    +		//and there is no mapping for that language, output the error message ‚ÄúExtension not supported‚Äù		
    +        analysis_request request;
    +        request.given_filename  = "test.cpsdf";
    +        request.entry_filename  = "";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "test.cpsdf");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index 0f77104..c0bc5e5 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit ec2a9b
% git checkout -q ec2a9b  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    ld: warning: ignoring file code_analysis_t.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis_t.o
    Undefined symbols for architecture x86_64:
      "_main", referenced from:
         implicit entry/start for main executable
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [code_analysis_t] Error 1

% git show

    commit ec2a9b392b09d096da79de6d6da711329bac87f8
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 11:04:27 2018 -0400
    
        added test case for entry_filename will also validate ext
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index bf2d9e7..5d5a78d 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -28,11 +28,11 @@ bool code_analysis(const analysis_request& request) {
     			std::cerr << "Extension not supported " << std:: endl;
     			return false;
     		}
    -		if(request.entry_filename != "" && language == ""){
    +	if(request.entry_filename != "" && language == ""){
     			std::cerr << "Extension not supported " << std:: endl;
     			return false;
     		}
    -		if(request.option_filename != "" && language == ""){
    +	if(request.option_filename != "" && language == ""){
     			std::cerr << "Using stdin requires a declared language" << std::endl;
     			return false;
     		}	
    diff --git a/code_analysis_t b/code_analysis_t
    index 5e81089..e78ffae 100755
    Binary files a/code_analysis_t and b/code_analysis_t differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 2dfbd17..ae3e56b 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -226,6 +226,24 @@ int main() {
             assert(code_analysis(request) == false);
     		
     	}
    +	{
    +		//Test case: Error handling If the file extension is used to determine the language, 
    +		//and there is no mapping for that language, output the error message ‚ÄúExtension not supported‚Äù		
    +        analysis_request request;
    +        request.given_filename  = "";
    +        request.entry_filename  = "test.cpsdf";
    +        request.given_url       = "";
    +        request.option_filename = "";
    +        request.option_url      = "";
    +        request.option_language = "";
    +		
    +        auto filename = analysis_filename(request);
    +        assert(filename == "test.cpsdf");
    +        assert(analysis_url(request) == "");
    +        assert(analysis_language(request, filename) == "");
    +        assert(code_analysis(request) == false);
    +		
    +	}
     
         return 0;
     }
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    index c0bc5e5..0a74803 100644
    Binary files a/code_analysis_t.o and b/code_analysis_t.o differ


### Commit 0766d0
% git checkout -q 0766d0  
% make  

    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    ld: warning: ignoring file code_analysis_t.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis_t.o
    Undefined symbols for architecture x86_64:
      "_main", referenced from:
         implicit entry/start for main executable
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [code_analysis_t] Error 1

% git show

    commit 0766d0abdaef0bc481f5dc0412e427891532f15d
    Author: dab130 <dab130@zips.uakron.edu>
    Date:   Fri Apr 6 11:16:46 2018 -0400
    
        updating code
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 5d5a78d..437ce67 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -25,16 +25,16 @@ bool code_analysis(const analysis_request& request) {
     
         // code analysis processing that is not yet implemented
     	if(request.given_filename != "" && language == ""){
    -			std::cerr << "Extension not supported " << std:: endl;
    -			return false;
    +		std::cerr << "Extension not supported " << std:: endl;
    +		return false;
     		}
     	if(request.entry_filename != "" && language == ""){
    -			std::cerr << "Extension not supported " << std:: endl;
    -			return false;
    +		std::cerr << "Extension not supported " << std:: endl;
    +		return false;
     		}
     	if(request.option_filename != "" && language == ""){
    -			std::cerr << "Using stdin requires a declared language" << std::endl;
    -			return false;
    +		std::cerr << "Using stdin requires a declared language" << std::endl;
    +		return false;
     		}	
     
         return false;

