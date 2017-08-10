//
// Created by ollie on 07/05/17.
//

#ifndef RSSHUNGER_DOWNLOADER_H
#define RSSHUNGER_DOWNLOADER_H

// std
#include <string>

// boost
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/network/protocol/http/client.hpp>


std::string get_page(std::string page) {
    using namespace boost::network;
    http::client client;
    http::client::request request(page);
    request << header("Connection", "close");
    http::client::response response = client.get(request);
    return body(response);
}


#endif //RSSHUNGER_DOWNLOADER_H
