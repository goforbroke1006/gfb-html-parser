//
// Created by goforbroke on 11.02.19.
//

// You can build it on linux with next command:
// g++ -Wall --std=c++11 -I/usr/local/include/htmlcxx -I/usr/local/include/gfb-html-parser samples/basic.cpp -o sample-basic -L/usr/local/lib/ -lcurl -lhtmlcxx -lgfb-html-parser

#include <iostream>

#include <gfb-html-parser/gfb-html-parser.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./sample-basic http://hello-cpp/heroes.html" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        const std::string &content = gfb::html::parser::getContent(argv[1]);
        gfb::html::parser::ExtraNode page(content);

        for (auto &item : page.getChildren(
                R"(div[class*="wrapper"] > div[class*="card"])")) { // looking for div.XX-cart-XXXX elements
            std::string line;
            line += item->getChild("[class*=\"title\"]")->getLocalText(); // grab titles

            auto deadlineEl = item->getChild("[class*=\"deadline\"]"); // grab deadline infos
            if (deadlineEl) {
                line += " - ";
                line += deadlineEl->getLocalText();
            }

            std::cout << line << std::endl; // write to STDOUT
        }
    } catch (std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
    }
}