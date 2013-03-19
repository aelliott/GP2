#include "ruleparser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

namespace Developer {

namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

template <typename Iterator>
struct rule_grammar : qi::grammar< Iterator, rule_t(), ascii::space_type >
{
    rule_grammar() : rule_grammar::base_type( rule, "rule" )
    {
        using namespace qi::labels;
        comment %= qi::lit("/*") >> *((qi::char_ - '*') | "*" >> !qi::lit("/"))
                                 >> qi::lit("*/");
        identifier %= qi::char_("a-zA-Z") >> *(qi::char_("a-zA-Z0-9"));
        param %= identifier >> ":" >> identifier;
        params %= qi::lit("(") >> (param % ";") >> ")";
        node %= qi::lit("(") >> identifier >> "_" >> identifier >> "_" >> "("
                             >> qi::double_ >> "," >> qi::double_ >> ")" >> ")";
        nodes %= node % ",";
        edge %= qi::lit("(") >> identifier >> "," >> identifier >> ","
                             >> identifier >> ")";
        edges %= edge % ",";
        graph %= qi::lit("{") >> "(" >> qi::double_ >> "," >> qi::double_ >> ")"
                              >> "|" >> nodes >> "|" >> edges >> "}";
        rule %= -comment >> identifier >> -comment >> params >> -comment
                            >> "=" >> -comment >> graph >> -comment;

        rule.name("rule");
        identifier.name("identifier");

        qi::on_error<qi::fail>
        (
            rule,
                    std::cout << phoenix::val("Parser error! Expecting: ")
                    << _4
                    << phoenix::val(" here: \"")
                    << phoenix::construct<std::string>(_3, _2)
                    << phoenix::val("\"")
                    << std::endl
        );
    }

    qi::rule<Iterator, qi::unused_type(), ascii::space_type> comment;
    qi::rule<Iterator, std::string(), ascii::space_type> identifier;
    qi::rule<Iterator, param_t(), ascii::space_type> param;
    qi::rule<Iterator, std::vector<param_t>(), ascii::space_type> params;
    qi::rule<Iterator, node_t(), ascii::space_type> node;
    qi::rule<Iterator, std::vector<node_t>(), ascii::space_type> nodes;
    qi::rule<Iterator, edge_t(), ascii::space_type> edge;
    qi::rule<Iterator, std::vector<edge_t>(), ascii::space_type> edges;
    qi::rule<Iterator, graph_t(), ascii::space_type> graph;
    qi::rule<Iterator, rule_t(), ascii::space_type> rule;
};

rule_t parseRule(const std::string &rule)
{
    rule_t ret;
    std::string::const_iterator iter = rule.begin();
    std::string::const_iterator end = rule.end();

    rule_grammar<std::string::const_iterator> parser;
    bool r = phrase_parse(iter, end, parser, ascii::space, ret);

    if(r)
    {
        std::cout << "Success" << std::endl;
    }
    else
    {
        std::cout << "Failure" << std::endl;
    }

    return ret;
}

QDebug operator<<(QDebug dbg, const std::string &str)
{
    dbg.nospace() << QString(str.c_str());
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const param_t &param)
{
    dbg.nospace() << "param(" << param.id << ", " << param.type << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<param_t> &params)
{
    dbg.nospace() << "params(";
    bool first = true;
    for(std::vector<param_t>::const_iterator iter = params.begin();
        iter != params.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::pair<double, double> &coords)
{
    dbg.nospace() << "(x: " << coords.first << ", y:" << coords.second << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const node_t &node)
{
    dbg.nospace() << "node((" << node.xPos << "," << node.yPos << ")" << ", "
                  << node.id << ", " << node.label << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<node_t> &nodes)
{
    dbg.nospace() << "nodes(";
    bool first = true;
    for(std::vector<node_t>::const_iterator iter = nodes.begin();
        iter != nodes.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const edge_t &edge)
{
    dbg.nospace() << "edge(" << edge.from << ", " << edge.to << ", "
                  << edge.label << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const std::vector<edge_t> &edges)
{
    dbg.nospace() << "edges(";
    bool first = true;
    for(std::vector<edge_t>::const_iterator iter = edges.begin();
        iter != edges.end(); ++iter)
    {
        if(first)
            first = false;
        else
            dbg.nospace() << ", ";
        dbg.nospace() << *iter;
    }
    dbg.nospace() << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const graph_t &graph)
{
    dbg.nospace() << "graph((" << graph.canvasX << "," << graph.canvasY << "), "
                  << graph.nodes << ", " << graph.edges << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const rule_t &rule)
{
    dbg.nospace() << "rule(" << rule.id << ", " << rule.parameters << ","
                  << rule.lhs << ")";
    return dbg.space();
}

}
