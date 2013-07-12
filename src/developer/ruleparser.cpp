/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
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
        identifier %= qi::lexeme[qi::char_("a-zA-Z_")
                >> *(qi::char_("a-zA-Z0-9_"))];
        node_identifier %= +(qi::char_("a-zA-Z0-9"))
                                               >> -(qi::string("(R)"));
        label %=  list >> -(qi::bool_);
        list %= atom % ":";
        atom %= qi::double_ | quoted_string | identifier;
        quoted_string %= qi::lexeme[qi::char_('"') >> *(qi::char_ - '"')
                                                   >> qi::char_('"')];
        variables %= identifier % ",";
        param %= variables >> ":" >> identifier;
        params %= qi::lit("(") >> -(param % ";") >> ")";
        node %= qi::lit("(") >> node_identifier >> "," >> label >> ","
                             >> "(" >> qi::double_ >> "," >> qi::double_ >> ")"
                             >> ")";
        nodes %= node % ",";
        edge %= qi::lit("(") >> node_identifier >> "," >> node_identifier >> ","
                             >> node_identifier >> "," >> label >> ")";
        edges %= edge % ",";
        graph %= qi::lit("(") >> qi::double_ >> "," >> qi::double_ >> ")" >> "|"
                              >> -(nodes) >> "|" >> -(edges);
        interface %= qi::lit("(") >> node_identifier >> "," >> node_identifier >> ")";
        interfaces %= interface % ",";
        rule %= documentation >> identifier >> -(params) >> "="
                              >> "{"  >> -(graph)  >> "}" >> "=>"  >> "{"
                              >> -(graph)  >> "}" >> "interface"  >> "=" >> "{"
                              >> -(interfaces) >> "}"  >> -(qi::lit("where")
                              >> qi::lexeme[*(qi::char_)]);

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

    qi::rule<Iterator, std::string(), ascii::space_type> documentation;
    qi::rule<Iterator, std::string(), ascii::space_type> identifier;
    qi::rule<Iterator, std::string(), ascii::space_type> node_identifier;
    qi::rule<Iterator, label_t(), ascii::space_type> label;
    qi::rule<Iterator, std::vector<atom_t>(), ascii::space_type> list;
    qi::rule<Iterator, atom_t(), ascii::space_type> atom;
    qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
    qi::rule<Iterator, std::vector<std::string>(), ascii::space_type> variables;
    qi::rule<Iterator, param_t(), ascii::space_type> param;
    qi::rule<Iterator, std::vector<param_t>(), ascii::space_type> params;
    qi::rule<Iterator, node_t(), ascii::space_type> node;
    qi::rule<Iterator, std::vector<node_t>(), ascii::space_type> nodes;
    qi::rule<Iterator, edge_t(), ascii::space_type> edge;
    qi::rule<Iterator, std::vector<edge_t>(), ascii::space_type> edges;
    qi::rule<Iterator, graph_t(), ascii::space_type> graph;
    qi::rule<Iterator, interface_t(), ascii::space_type> interface;
    qi::rule<Iterator, std::vector<interface_t>(), ascii::space_type> interfaces;
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
