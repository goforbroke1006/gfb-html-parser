//
// Created by goforbroke on 09.01.19.
//

#include "../include/gfb-html-parser.h"

size_t __write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string gfb::html::parser::getContent(std::string url) {
    std::string resp;
    CURL *c;
    c = curl_easy_init();
    if (c == nullptr) {
        throw std::runtime_error("CURL init error");
    }

    struct curl_slist *chunk = NULL;
    chunk = curl_slist_append(chunk,
                              "accept:          text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
    chunk = curl_slist_append(chunk, "accept-language: ru,en-US;q=0.9,en;q=0.8,uk;q=0.7");
    chunk = curl_slist_append(chunk, "x-compress:      null");
    chunk = curl_slist_append(chunk, "cache-control:   max-age=0");
    chunk = curl_slist_append(chunk,
                              "user-agent:      Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36");

    curl_easy_setopt(c, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, __write_callback);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &resp);

    CURLcode res = curl_easy_perform(c);
    curl_easy_cleanup(c);

    if (res != CURLE_OK) {
        std::cerr << "CURL (" << res << ") : " << curl_easy_strerror(res) << std::endl;
        throw std::runtime_error(curl_easy_strerror(res));
    }
    return resp;
}

std::string gfb::html::parser::getContentForce(std::string url, std::vector<std::string> proxies) {
    std::string resp;

    CURL *c;

    for (auto &proxyAddr : proxies) {

        c = curl_easy_init();
        if (c == nullptr) {
            throw std::runtime_error("CURL init error");
        }

        struct curl_slist *chunk = nullptr;
        chunk = curl_slist_append(chunk, "accept:          text/html,application/xhtml+xml,application/xml");
        chunk = curl_slist_append(chunk, "accept-language: ru,en-US;q=0.9,en;q=0.8,uk;q=0.7");
        chunk = curl_slist_append(chunk, "x-compress:      null");
        chunk = curl_slist_append(chunk, "cache-control:   max-age=0");
        chunk = curl_slist_append(chunk, "user-agent:      Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36");

        if (proxyAddr.length() > 0)
            curl_easy_setopt(c, CURLOPT_PROXY, proxyAddr.c_str());

        curl_easy_setopt(c, CURLOPT_TIMEOUT, 15);
        curl_easy_setopt(c, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(c, CURLOPT_URL, url.c_str());
        curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, __write_callback);
        curl_easy_setopt(c, CURLOPT_WRITEDATA, &resp);

        CURLcode res = curl_easy_perform(c);
        curl_easy_cleanup(c);

        if (res == CURLE_OK) {
            break;
        }

        std::cerr << "CURL (" << res << ") : " << curl_easy_strerror(res) << "\t[" << proxyAddr << "]" << std::endl;
    }

    return resp;
}

std::list<std::string> gfb::html::parser::findBySelector(std::string htmlContent, std::string selector) {
    htmlcxx::HTML::ParserDom parser;
    tree<htmlcxx::HTML::Node> dom = parser.parseTree(htmlContent.c_str());
    hcxselect::Selector s(dom);
//    try {
    s = s.select(selector.c_str());
//    } catch (hcxselect::ParseException &ex) {
//        cerr << "Parse error: '" << selectorStr << "': " << ex.what() << endl;
//        cerr << "              ";
//        for (int i = 1; i < ex.position(); i++) {
//            cerr << " ";
//        }
//        cerr << "^" << endl;
//        return 1;

//throw  std::runtime_error();
//
//    }
//    std::list<htmlcxx::HTML::Node> nodes;
    std::list<std::string> nodes;
    for (auto it : s) {

//        cout << endl
//             << readBuffer.substr((*it)->data.offset(), (*it)->data.length())
//             << endl << endl;
//        nodes.push_back((*it)->data);
        nodes.push_back(htmlContent.substr(it->data.offset(), it->data.length()));
    }
    return nodes;
}

gfb::html::parser::ExtraNode::ExtraNode(std::string html) {
    if (html.empty())
        throw std::runtime_error("empty html");

    if (html.find("<html>") == std::string::npos) {
        html = "<html>" + html + "</html>"; // TODO: workaround for css parser
    }

//    std::cout << "Create ExtraNode 1: " << html << std::endl;

    this->rawHtml = html;
    this->domTree = this->parser.parseTree(html);
    this->rootNode = getRootNode(this->domTree);
    this->rootNode.parseAttributes();
}

gfb::html::parser::ExtraNode::ExtraNode(const gfb::html::parser::ExtraNode &node) {
//    std::cout << "Create ExtraNode 2: " << node.rawHtml << std::endl;

    this->rawHtml = node.rawHtml;
    this->domTree = this->parser.parseTree(node.rawHtml);
    this->rootNode = getRootNode(this->domTree);
    this->rootNode.parseAttributes();
}

const std::vector<std::shared_ptr<gfb::html::parser::ExtraNode>>
gfb::html::parser::ExtraNode::getChildren(const std::string &selector) const {
    hcxselect::Selector s(domTree, selector);
    std::vector<std::shared_ptr<gfb::html::parser::ExtraNode>> nodes;
    for (auto it = s.begin(); it != s.end(); ++it) {
        const auto &childHtml = this->rawHtml.substr((*it)->data.offset(), (*it)->data.length());
        nodes.push_back(std::make_shared<gfb::html::parser::ExtraNode>(childHtml));
    }
    return nodes;
}

const std::shared_ptr<gfb::html::parser::ExtraNode>
gfb::html::parser::ExtraNode::getChild(const std::string &selector) const {
    const auto &elements = this->getChildren(selector);
    if (elements.empty())
        return std::shared_ptr<gfb::html::parser::ExtraNode>(nullptr);
    return elements.at(0);
}

const std::string
gfb::html::parser::ExtraNode::getAttr(const std::string &name) const {
    const std::pair<bool, std::string> &pair = rootNode.attribute(name);
    if (pair.first)
        return pair.second;
    return "";
}

const std::string
gfb::html::parser::ExtraNode::getText() const {
    std::string res;
    tree<htmlcxx::HTML::Node>::iterator it = this->domTree.begin();
    for (; it != this->domTree.end(); ++it) {
        if (!it->isTag() && !it->isComment())
            res.append(it->text());
    }
    return res;
}

const std::string
gfb::html::parser::ExtraNode::getLocalText() const {
    if (this->domTree.empty())
        return "";
    std::string res;
    tree<htmlcxx::HTML::Node>::iterator it = this->domTree.begin();
    const int depth = 3; // selected empirically
    tree<htmlcxx::HTML::Node>::fixed_depth_iterator fdit;
    try {
        fdit = this->domTree.begin_fixed(it, depth);
    } catch (std::range_error &ex) {
        return res;
    }
    while (true) {
        if (!fdit->isTag() && !fdit->isComment())
            res.append(fdit->text());
        if (nullptr == fdit.node->next_sibling)
            break;
        ++fdit;
    }
    return res;
}

htmlcxx::HTML::Node gfb::html::parser::ExtraNode::getRootNode(const tree<htmlcxx::HTML::Node> &mytree) {
    tree<htmlcxx::HTML::Node>::pre_order_iterator it = mytree.begin();
    ++it;
    ++it; // TODO: workaround for workaround for css parser
    return *it;
}
