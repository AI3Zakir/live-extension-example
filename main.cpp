#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/NullStream.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <phpcpp.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::JSON::Parser;
using Poco::JSON::ParseHandler;
using namespace Poco::JSON;
using Poco::Dynamic::Var;

/**
 * Make request with POCO
 *
 * @param session
 * @param request
 * @param response
 * @return
 */
bool
doRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request, Poco::Net::HTTPResponse &response) {
    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);

    Php::out << response.getStatus() << " - " << response.getReason() << std::endl;
    if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED) {
        std::string responseStr;
        StreamCopier::copyToString(rs, responseStr);

        Parser parser;
        Var result = parser.parse(responseStr);
        Array::Ptr arr = result.extract<Array::Ptr>();
        Array::ConstIterator it;

        for (it = arr->begin(); it != arr->end(); it++) {
            Php::out << "my children:" << it->convert<std::string>() << std::endl;
        }

        return true;
    } else {
        //it went wrong ?
        return false;
    }
}

/**
 * Make http request with PHP-CPP
 *
 * @return
 */
Php::Value make_request_with_cpp(Php::Parameters &params) {
    std::string input = params[0];

    Php::out << input << std::endl;

    URI uri(input);
    std::string path(uri.getPathAndQuery());
    if (path.empty()) path = "/";
    HTTPClientSession session(uri.getHost(), uri.getPort());
    HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
    HTTPResponse response;

    if (!doRequest(session, request, response)) {
        std::cerr << "Invalid Request." << std::endl;
        return 1;
    }
}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

/**
 *  Function that is called by PHP right after the PHP process
 *  has started, and that returns an address of an internal PHP
 *  strucure with all the details and features of your extension
 *
 *  @return void*   a pointer to an address that is understood by PHP
 */
PHPCPP_EXPORT void *get_module() {
    // static(!) Php::Extension object that should stay in memory
    // for the entire duration of the process (that's why it's static)
    static Php::Extension extension("extensionExample", "1.0");
    extension.add<make_request_with_cpp>("make_request_with_cpp", {
            Php::ByVal("input", Php::Type::String)
    });
    // return the extension
    return extension;
}
}
