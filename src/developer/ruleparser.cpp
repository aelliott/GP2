/*!
 * \file
 */
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

/*!
 * \brief This internal struct contains the grammar for parsing a rule
 */
template <typename Iterator>
struct rule_grammar : qi::grammar< Iterator, rule_t(), ascii::space_type >
{
    rule_grammar() : rule_grammar::base_type( rule, "rule" )
    {
        using namespace qi::labels;
        documentation %= qi::lit("/*!") >> qi::lexeme[*((qi::char_ - '*') | "*"
                                        >> !qi::lit("/"))] >> qi::lit("*/");
        comment %= qi::lit("/*") >> *((qi::char_ - '*') | "*" >> !qi::lit("/"))
                                 >> qi::lit("*/");
        identifier %= qi::char_("a-zA-Z") >> *(qi::char_("a-zA-Z0-9"));
        quoted_string %= qi::lit('"') >> qi::lexeme[*(qi::char_ - '"')] >> '"';
        param %= identifier >> ":" >> identifier;
        params %= qi::lit("(") >> (param % ";") >> ")";
        node %= qi::lit("(") >> identifier >> "," >> quoted_string >> "," >> "("
                             >> qi::double_ >> "," >> qi::double_ >> ")" >> ")";
        nodes %= node % ",";
        edge %= qi::lit("(") >> identifier >> "," >> identifier >> ","
                             >> quoted_string >> ")";
        edges %= edge % ",";
        graph %= qi::lit("{") >> "(" >> qi::double_ >> "," >> qi::double_ >> ")"
                              >> "|" >> nodes >> "|" >> edges >> "}";
        rule %= documentation >> -comment >> identifier >> -comment >> params
                              >> -comment >> "=" >> -comment >> graph
                              >> -comment >> "=>" >> -comment >> graph
                              >> -comment;

        rule.name("rule");
        documentation.name("documentation comment");
        comment.name("comment");
        identifier.name("identifier");
        quoted_string.name("quoted string");
        param.name("parameter");
        params.name("parameter section");
        node.name("node");
        nodes.name("node list");
        edge.name("edge");
        edges.name("edge list");
        graph.name("graph");

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

    //! \brief The documentation comment is a special case at the start of the
    //! file
    //!
    //! /*! documentation comment */
    qi::rule<Iterator, std::string(), ascii::space_type> documentation;
    //! \brief Normal comments may appear in many places in the file and are
    //! ignored
    //!
    //! /* comment */
    qi::rule<Iterator, qi::unused_type(), ascii::space_type> comment;
    //! \brief Identifiers are strings which conform to a particular pattern
    //!
    //! [a-zA-Z_][a-zA-Z0-9_}{,62}
    qi::rule<Iterator, std::string(), ascii::space_type> identifier;
    //!  \brief Quoted strings contain any number of non-quote characters until
    //! the terminating quote
    //!
    //! "[^"]*"
    qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
    //! \brief An individual parameter with an identifier and a type
    //!
    //! id : type
    qi::rule<Iterator, param_t(), ascii::space_type> param;
    //! \brief A semi-colon delimited list of parameters
    //!
    //! param {; param}
    qi::rule<Iterator, std::vector<param_t>(), ascii::space_type> params;
    //! \brief An individual node containing its id, label and position
    //!
    //! (id, label, (xPos, yPos))
    qi::rule<Iterator, node_t(), ascii::space_type> node;
    //! \brief A comma delimited list of nodes
    //!
    //! node {, node}
    qi::rule<Iterator, std::vector<node_t>(), ascii::space_type> nodes;
    //! \brief An individual edge containing the 'from' node id, the 'to' node
    //! id and the label
    //!
    //! (from, to, label)
    qi::rule<Iterator, edge_t(), ascii::space_type> edge;
    //! \brief A comma delimited list of edges
    //!
    //! edge {, edge}
    qi::rule<Iterator, std::vector<edge_t>(), ascii::space_type> edges;
    //! \brief A complete graph consisting of a canvas size, a set of nodes and
    //! a set of edges
    //!
    //! { (canvasX, canvasY) | nodes | edges }
    qi::rule<Iterator, graph_t(), ascii::space_type> graph;
    //! \brief A complete GP rule
    //!
    //! consisting of documentation, an identifier, a parameter list, a lhs
    //! graph, a rhs graph and an interface
    //!
    //! doc_comment id param_list = lhs_graph => rhs_graph interface
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

QDebug operator<<(QDebug dbg, const node_t &node)
{
    dbg.nospace() << "node(" << node.id  << ", " << node.label << ", ("
                  << node.xPos << "," << node.yPos << ")" << ")";
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
    dbg.nospace() << "rule(" << rule.documentation << ", " << rule.id << ", "
                  << rule.parameters << "," << rule.lhs << ", " << rule.rhs
                  << ")";
    return dbg.space();
}

}
