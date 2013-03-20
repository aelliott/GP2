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
        node_identifier %= qi::char_("a-zA-Z") >> *(qi::char_("a-zA-Z0-9"))
                                               >> -(qi::string("(R)"));
        quoted_string %= qi::lit('"') >> qi::lexeme[*(qi::char_ - '"')] >> '"';
        param %= identifier >> ":" >> identifier;
        params %= qi::lit("(") >> -(param % ";") >> ")";
        node %= qi::lit("(") >> node_identifier >> "," >> quoted_string >> ","
                             >> "(" >> qi::double_ >> "," >> qi::double_ >> ")"
                             >> ")";
        nodes %= node % ",";
        edge %= qi::lit("(") >> identifier >> "," >> identifier >> ","
                             >> quoted_string >> ")";
        edges %= edge % ",";
        graph %= qi::lit("(") >> qi::double_ >> "," >> qi::double_ >> ")" >> "|"
                              >> -(nodes) >> "|" >> -(edges);
        interface %= qi::lit("(") >> identifier >> "," >> identifier >> ")";
        interfaces %= interface % ",";
        rule %= documentation >> identifier >> -(params) >> "="
                              >> "{"  >> -(graph)  >> "}" >> "=>"  >> "{"
                              >> -(graph)  >> "}" >> "interface"  >> "=" >> "{"
                              >> -(interfaces) >> "}"  >> -(qi::lit("where")
                              >> qi::lexeme[+(qi::char_)]);

        rule.name("rule");
        documentation.name("documentation comment");
        comment.name("comment");
        identifier.name("identifier");
        node_identifier.name("node identifier");
        quoted_string.name("quoted string");
        param.name("parameter");
        params.name("parameter section");
        node.name("node");
        nodes.name("node list");
        edge.name("edge");
        edges.name("edge list");
        graph.name("graph");
        interface.name("interface");
        interfaces.name("interface list");

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
    //! \brief An interface definition matching two differently identified nodes
    //!
    //! (lhs_node, rhs_node)
    qi::rule<Iterator, interface_t(), ascii::space_type> interface;
    //! \brief A collection of interfaces making up the whole interface between
    //! the LHS and RHS graphs
    //!
    //! interface {, interface}
    qi::rule<Iterator, std::vector<interface_t>(), ascii::space_type> interfaces;
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

    if(!r)
    {
        std::cout << "    Rule parsing failed." << std::endl;
        std::cout << "    Remaining string contents: " << std::string(iter, end)
                  << std::endl;
    }
    else if(iter != end)
    {
        std::cout << "    Parsing ended before the end of the provided string."
                  << std::endl;
        std::cout << "    Remaining string contents: " << std::string(iter, end)
                  << std::endl;
    }

    return ret;
}

}
