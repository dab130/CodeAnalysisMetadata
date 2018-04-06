/*
  @file code_analysis.cpp

  Implementation of analysis requests
*/

#include "code_analysis.hpp"
#include "get_language_from_filename.hpp"

#include <string>

/** Generate source analysis based on the request
 * @param request Data that forms the request
 * @retval true   Valid request
 * @retval false  Invalid filename or unable to deduce language
 */
bool code_analysis(const analysis_request& request) {

    auto filename = analysis_filename(request);

    auto url = analysis_url(request);

    auto language = analysis_language(request, filename);

    // code analysis processing that is not yet implemented

    return false;
}

/** Filename extracted from the request
 * @param  request Data that forms the request
 * @retval filename
 */
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
    return "";
}

/** URL extracted from the request
 * @param  request Data that forms the request
 * @retval URL
 */
std::string analysis_url(const analysis_request& request) {

    return "";
}

/** Language extracted from the request and the filename
 * @param  request Data that forms the request
 * @param  filename Filename extracted from the request
 * @retval language
 */
std::string analysis_language(const analysis_request& request, const std::string& filename) {

    return "";
}
