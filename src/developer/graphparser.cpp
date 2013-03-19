/*!
 * \file
 */
#include "graphparser.hpp"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <QDomDocument>
#include <QDebug>

namespace Developer {

namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

/*!
 * \brief This internal struct contains the grammar for parsing an alternative
 *  format graph
 */
template <typename Iterator>
struct alternative_grammar : qi::grammar< Iterator, graph_t(), ascii::space_type >
{

    alternative_grammar() : alternative_grammar::base_type(graph, "graph")
    {
        using namespace qi::labels;
        identifier %= qi::char_("a-zA-Z") >> *(qi::char_("a-zA-Z0-9"));
        node_identifier %= qi::char_("a-zA-Z") >> *(qi::char_("a-zA-Z0-9"))
                                               >> -(qi::string("(R)"));
        quoted_string %= qi::lit('"') >> qi::lexeme[*(qi::char_ - '"')] >> '"';
        node %= qi::lit("(") >> node_identifier >> "," >> quoted_string >> ","
                             >> "(" >> qi::double_ >> "," >> qi::double_ >> ")"
                             >> ")";
        nodes %= node % ",";
        edge %= qi::lit("(") >> identifier >> "," >> identifier >> ","
                             >> quoted_string >> ")";
        edges %= edge % ",";
        graph %= -(qi::lit("{")) >> "(" >> qi::double_ >> "," >> qi::double_
                                 >> ")" >> "|" >> nodes >> "|" >> edges
                                 >> -(qi::lit("}"));

        identifier.name("identifier");
        node_identifier.name("node identifier");
        quoted_string.name("quoted string");
        node.name("node");
        nodes.name("node list");
        edge.name("edge");
        edges.name("edge list");
        graph.name("graph");

        qi::on_error<qi::fail>
        (
            graph,
                    std::cout << phoenix::val("Parser error! Expecting: ")
                    << _4
                    << phoenix::val(" here: \"")
                    << phoenix::construct<std::string>(_3, _2)
                    << phoenix::val("\"")
                    << std::endl
        );
    }

    //! \brief Identifiers are strings which conform to a particular pattern
    //!
    //! [a-zA-Z_][a-zA-Z0-9_}{,62}
    qi::rule<Iterator, std::string(), ascii::space_type> identifier;
    //! \brief Like a regular identifier, node identifiers can also be suffixed
    //! with a "(R)" literal to indicate a root node
    //!
    //! [a-zA-Z_][a-zA-Z0-9_}{,62}(\(R\))?
    qi::rule<Iterator, std::string(), ascii::space_type> node_identifier;
    //!  \brief Quoted strings contain any number of non-quote characters until
    //! the terminating quote
    //!
    //! "[^"]*"
    qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
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
    //! {? (canvasX, canvasY) | nodes | edges }?
    qi::rule<Iterator, graph_t(), ascii::space_type> graph;
};

graph_t parseAlternativeGraph(const std::string &graphString)
{
    graph_t ret;
    std::string::const_iterator iter = graphString.begin();
    std::string::const_iterator end = graphString.end();

    alternative_grammar<std::string::const_iterator> parser;
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

graph_t parseDotGraph(const std::string &graphString)
{
    return graph_t();
}

graph_t parseGxlGraph(const QString &graphString)
{
    QDomDocument doc("graph");
    if(!doc.setContent(graphString))
    {
        qDebug() << "Could not parse the input string, is it valid GXL?";
        qDebug() << "Input: " << graphString;
        return graph_t();
    }

    return graph_t();
}

}
