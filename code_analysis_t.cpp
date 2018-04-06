/*
  @file code_analysis_t.cpp

  Test program for analysis requests
*/

#include "code_analysis.hpp"

#include <string>
#include <cassert>

int main() {

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
		//For a regular file the entry_filename is the literal string “data”, and you should use the given_filename
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
		//The given_filename is a single dash “-” when the input is from standard input.
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
		//Note that the entry_filename is not valid (i.e, “data”) for a non source code archive,
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
		//Test case:  If the option_filename is blank (i.e., “”), then the resulting filename is blank,
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

    return 0;
}
