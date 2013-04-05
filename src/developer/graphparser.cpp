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
#include <QStringList>
#include <QRegExp>

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
        identifier %= qi::char_("a-zA-Z_") >> *(qi::char_("a-zA-Z0-9_"));
        node_identifier %= +(qi::char_("a-zA-Z0-9"))
                                               >> -(qi::string("(R)"));
        label %=  list >> -(qi::bool_);
        list %= qi::lit("empty") | atom | identifier | list >> ":" >> list;
        atom %= qi::double_ | quoted_string;
        quoted_string %= qi::lit('"') >> qi::lexeme[*(qi::char_ - '"')] >> '"';
        node %= qi::lit("(") >> node_identifier >> "," >> label >> ","
                             >> "(" >> qi::double_ >> "," >> qi::double_ >> ")"
                             >> ")";
        nodes %= node % ",";
        edge %= qi::lit("(") >> node_identifier >> "," >> node_identifier >> ","
                             >> node_identifier >> "," >> label >> ")";
        edges %= edge % ",";
        graph %= qi::lit("(") >> qi::double_ >> "," >> qi::double_ >> ")" >> "|"
                              >> -(nodes) >> "|" >> -(edges);

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
    qi::rule<Iterator, std::string(), ascii::space_type> label;
    //! \brief A list is the basic datatype of GP, it is a colon-delimited
    //! sequence of atoms
    //!
    //! empty | atom | identifier | list {: list}
    qi::rule<Iterator, std::string(), ascii::space_type> list;
    qi::rule<Iterator, std::string(), ascii::space_type> atom;
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

    if(!r)
    {
        std::cout << "    Graph parsing failed." << std::endl;
        std::cout << "    Input: " << graphString << std::endl;
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

graph_t parseDotGraph(const std::string &graphString)
{
    return graph_t();
}

graph_t parseGxlGraph(const QString &graphString)
{
    graph_t result;
    result.canvasX = 0;
    result.canvasY = 0;

    QDomDocument doc("graph");
    if(!doc.setContent(graphString))
    {
        qDebug() << "    Could not parse the input string, is it valid GXL?";
        qDebug() << "    Input: " << graphString;
        return result;
    }

    QDomNodeList nodes = doc.elementsByTagName("gxl");
    if(nodes.count() < 1)
    {
        qDebug() << "    Parse Error: GXL input did not contain a <gxl> root node.";
        qDebug() << "    Input: " << graphString;
        return result;
    }

    QDomNode root = nodes.at(0);
    nodes = root.childNodes();

    for(int i = 0; i < nodes.count(); ++i)
    {
        QDomElement elem = nodes.at(i).toElement();
        if(elem.tagName() == "graph")
        {
            // Now we're talking, we've got a graph. Check for canvas dimensions
            if(elem.hasAttribute("canvasWidth"))
                result.canvasX = elem.attribute("canvasWidth").toDouble();
            if(elem.hasAttribute("canvasHeight"))
                result.canvasY = elem.attribute("canvasHeight").toDouble();

            nodes = nodes.at(i).childNodes();

            // These are node IDs, which may be simple integers
            QRegExp identifier("[a-zA-Z0-9]{,63}");
            // We're not going to return to the parent loop, re-use i.
            for(i = 0; i < nodes.count(); ++i)
            {
                elem = nodes.at(i).toElement();

                // Handle nodes stored in this graph
                if(elem.tagName() == "node")
                {
                    node_t node;
                    node.xPos = 0;
                    node.yPos = 0;

                    // Start with compulsary attributes: id, label
                    if(!elem.hasAttribute("id"))
                    {
                        qDebug() << "    Parse Error: <node> missing 'id' attribute.";
                        continue;
                    }
                    else
                    {
                        QString id = elem.attribute("id");
                        if(identifier.exactMatch(id))
                            node.id = id.toStdString();
                        else
                        {
                            qDebug() << "    Parse Warning: <node> id contains illegal characters. Stripping them.";
                            qDebug() << "    Input: " << id;
                            identifier.indexIn(id);
                            id = identifier.cap(0);
                            node.id = id.toStdString();
                        }
                    }

                    if(!elem.hasAttribute("label"))
                    {
                        // This isn't an attribute, is it a child node?
                        QDomNodeList children = nodes.at(i).childNodes();
                        bool found = false;
                        QString l;
                        if(children.count() > 0)
                        {
                            for(int j = 0; j < children.count() && !found; ++j)
                            {
                                QDomElement e = children.at(j).toElement();
                                if(e.tagName() == "label")
                                {
                                    l = e.text();
                                    found = true;
                                }
                            }
                        }

                        if(!found)
                        {
                            qDebug() << "    Parse Warning: <node> missing 'label' attribute. Assuming label = id.";
                            node.label = node.id;
                        }
                        else
                            node.label = l.toStdString();
                    }
                    else
                        node.label = elem.attribute("label").toStdString();

                    // Then check for optional attributes: root, position
                    if(elem.hasAttribute("root"))
                    {
                        if(QVariant(elem.attribute("root")).toBool())
                        {
                            node.id += "(R)";
                        }
                    }

                    if(elem.hasAttribute("position"))
                    {
                        QStringList coords = elem.attribute("position").split(",");
                        if(coords.size() < 2)
                        {
                            qDebug() << "    Parse Warning: <node> 'position' attribute does not contain a comma separated list of values, ignoring.";
                        }
                        else
                        {
                            node.xPos = coords.at(0).toDouble();
                            node.yPos = coords.at(1).toDouble();
                        }
                    }

                    result.nodes.push_back(node);
                }
                else if(elem.tagName() == "edge")
                {
                    edge_t edge;

                    // Start with compulsary attributes: id, label
                    if(!elem.hasAttribute("id"))
                    {
                        qDebug() << "    Parse Error: <edge> missing 'id' attribute.";
                        continue;
                    }
                    else
                    {
                        QString id = elem.attribute("id");
                        if(identifier.exactMatch(id))
                            edge.id = id.toStdString();
                        else
                        {
                            qDebug() << "    Parse Warning: <edge> id contains illegal characters. Stripping them.";
                            qDebug() << "    Input: " << id;
                            identifier.indexIn(id);
                            id = identifier.cap(0);
                            edge.id = id.toStdString();
                        }
                    }

                    // Start with compulsary attributes: from, to
                    if(!elem.hasAttribute("from"))
                    {
                        qDebug() << "    Parse Error: <edge> missing 'from' attribute";
                        continue;
                    }

                    if(!elem.hasAttribute("to"))
                    {
                        qDebug() << "    Parse Error: <edge> missing 'to' attribute";
                        continue;
                    }

                    edge.from = elem.attribute("from").toStdString();
                    edge.to = elem.attribute("to").toStdString();

                    // Then check for optional attributes: label
                    if(elem.hasAttribute("label"))
                    {
                        edge.label = elem.attribute("label").toStdString();
                    }

                    result.edges.push_back(edge);
                }
            }

            return result;
        }
    }

    return result;
}

}
