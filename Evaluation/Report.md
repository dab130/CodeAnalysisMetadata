
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
% srcml --xpath="//src:function[src:name='code_analysis']" project.xml | srcml | cat -v

    bool code_analysis(const analysis_request& request) {
    
        auto filename = analysis_filename(request);
    
        auto url = analysis_url(request);
    
        auto language = analysis_language(request, filename);
    
        // code analysis processing that is not yet implemented
    	if(request.given_filename != "" && language == ""){
    		std::cerr << "Extension not supported " << std:: endl;
    		return false;
    		}
    	if(request.entry_filename != "" && language == ""){
    		std::cerr << "Extension not supported " << std:: endl;
    		return false;
    		}
    	if(request.option_filename != "" && language == ""){
    		std::cerr << "Using stdin requires a declared language" << std::endl;
    		return false;
    		}	
    
        return false;
    }

% srcml --xpath="//src:function[src:name='analysis_filename']" project.xml | srcml | cat -v

    std::string analysis_filename(const analysis_request& request) {
    	if(request.option_filename != ""){
    		return request.option_filename; 				// if there is a stdin value return it.
    		
    	}else if(request.entry_filename == "data"){
    		return request.given_filename; 					// this will return a file on the disk. 
    														// if there isn't a stdin value and the data is not source code
    	}else if(request.entry_filename != "" && request.option_language == ""){
    		return request.entry_filename; 
    														//if there isn't a stdin value and the data is source code		
    	}else if(request.given_filename != "" && request.option_language == ""){
    		return request.given_filename; 				
    		
    	}								   
    	
        return "";
    }

% srcml --xpath="//src:function[src:name='analysis_url']" project.xml | srcml | cat -v

    std::string analysis_url(const analysis_request& request) {
    	if(request.option_url != ""){
    			return request.option_url;
    	}else if(request.given_url != ""){
    			return request.given_url;
    		}
    		
    		return "";
    }

% srcml --xpath="//src:function[src:name='analysis_language']" project.xml | srcml | cat -v

    std::string analysis_language(const analysis_request& request, const std::string& filename) {
    	if(request.option_language != ""){
    		return request.option_language;
    	}if(filename != ""){
    		return get_language_from_filename(filename); 
    	}
    	return ""; 
    }

## Test Cases 
% srcml code_analysis_t.cpp --xpath="//src:function[src:name='main']/src:block" | srcml | cat -v

sed: RE error: illegal byte sequence
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

    make: Nothing to be done for `all'.

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

    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit ded867
% git checkout -q ded867  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 5f67ef
% git checkout -q 5f67ef  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit ef9284
% git checkout -q ef9284  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit f0d025
% git checkout -q f0d025  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit d9abab
% git checkout -q d9abab  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit da02f1
% git checkout -q da02f1  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit b16887
% git checkout -q b16887  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 0cb3d8
% git checkout -q 0cb3d8  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 7b57e2
% git checkout -q 7b57e2  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 3bc365
% git checkout -q 3bc365  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 304ac8
% git checkout -q 304ac8  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 5960cf
% git checkout -q 5960cf  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 6e1ad0
% git checkout -q 6e1ad0  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 95f828
% git checkout -q 95f828  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 63407c
% git checkout -q 63407c  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit ec2a9b
% git checkout -q ec2a9b  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

### Commit 0766d0
% git checkout -q 0766d0  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          _main in get_language_from_filename_t.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

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

