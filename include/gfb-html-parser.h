//
// Created by goforbroke on 09.01.19.
//

#ifndef SPORT_BET_COLLECTOR_ALL_H
#define SPORT_BET_COLLECTOR_ALL_H

#include <string>
#include <list>
#include <htmlcxx/html/ParserDom.h>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <memory>
//#include <list>

#include <curl/curl.h>
#include <iostream>

#include "hcxselect.h"

namespace gfb::html::parser {

    static const char *const FAKE_USER_AGENT = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36";

    std::string getContent(std::string url);

    std::string getContentForce(std::string url, std::vector<std::string> proxies);

    std::list<std::string> findBySelector(std::string htmlContent, std::string selector);

    class ExtraNode {
    public:
        explicit ExtraNode(std::string html);

        ExtraNode(const ExtraNode &node);

        const std::vector<std::shared_ptr<gfb::html::parser::ExtraNode>> getChildren(const std::string &selector) const;

        const std::shared_ptr<gfb::html::parser::ExtraNode> getChild(const std::string &selector) const;

        const std::string getAttr(const std::string &name) const;

        const std::string getText() const;

        const std::string getLocalText() const;

    private:
        htmlcxx::HTML::ParserDom parser;
        std::string rawHtml;
        tree<htmlcxx::HTML::Node> domTree;
        htmlcxx::HTML::Node rootNode;

        static htmlcxx::HTML::Node getRootNode(const tree<htmlcxx::HTML::Node> &mytree);
    };
}

#endif //SPORT_BET_COLLECTOR_ALL_H
