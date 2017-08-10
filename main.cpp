// std imports
#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <tuple>
#include <string>
#include <functional>
#include <fstream>
#include <future>
#include <algorithm>

// project files
#include "Downloader.h"
#include "Extractor.h"


std::vector<std::future<std::string>> spawn_fetchers(std::vector<std::string> links) {
    std::vector<std::future<std::string>> futures;
    std::transform(links.begin(), links.end(), std::back_inserter(futures),
                   [](std::string& prompage) {
                       return std::async(get_page, prompage);
                   });
    return futures;
}

std::vector<std::string> get_futures(std::vector<std::future<std::string>> futures) {
    std::vector<std::string> pages;
    std::transform(futures.begin(), futures.end(), std::back_inserter(pages),
                   [](std::future<std::string> &requestedPage) { return requestedPage.get(); });
    return pages;
}

std::vector<std::string> fetch_pages(std::vector<std::string> links) {
    return get_futures(spawn_fetchers(links));
}

void fetch() {
    std::ifstream sourceFile(".RSSSourceFile");
    // maps web pages to the last avaiable hash made.
    // this is in order to check for changes in the RSS page.
    std::map<std::string, size_t> sources;
    std::string source;
    size_t hash;
    // populate the map
    while (sourceFile >> source) {
        sourceFile >> hash;
        sources[source] = hash;
    }
    // extract the string values from the map
    std::vector<std::string> source_strings;
    std::transform(sources.begin(), sources.end(), std::back_inserter(source_strings),
                   [] (std::pair<std::string, std::size_t> d) { return d.first;});
    // spawn tasks tofetch the required RSS pages.
    std::vector<std::string> results = fetch_pages(source_strings);

    std::vector<std::string> links;
    int i = 0;
    for (std::pair<const std::string, size_t> &s : sources) {
        // if the RSS page has had a new entry added ie. the has has changed
        // the new added link will be extracted using an XMLDecoder and stored in the vector links.
        if (std::get<1>(s) != std::hash<std::string>{}(results[i])) {
            XMLDecoder<1> decoder(results[0]);
            const std::string link = decoder.parse();
            links.push_back(link);
        }
        i++;
    }
    auto pages = fetch_pages(links);
    for(auto& article : pages) {
        std::string exert = article.substr(10);
        auto f = std::ofstream{exert};
        f << article;
        f.close();
    }
}

void add(std::string source) {
    std::ofstream sourceFile;
    sourceFile.open(".RSSSourceFile");
    std::string page = get_page(source); // fetch the page so a starting hash can be taken
    size_t originalHash = std::hash<std::string>{}(page);
    sourceFile << source << ' ' << originalHash;
}

int main(int argc, char** argv) {
    if(argc < 2 || argc  > 5) {
        std::cerr << "Usage: Hunger add/fetch {RSS stream location}\n";
    } else if(strcmp(argv[1], "add") == 0) {
        add(argv[2]);
    } else if(strcmp(argv[1], "fetch") == 0) {
        fetch();
    } else {
        std::cerr << "Usage: Hunger add/fetch {RSS stream location}\n";
    }
    return 0;
}
