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
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
		
	}

    return 0;
}
