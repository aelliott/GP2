/*!
 * \file
 */
#ifndef PARSERTYPES_HPP
#define PARSERTYPES_HPP

#include <QDebug>

#ifndef Q_MOC_RUN
#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#endif // Q_MOC_RUN

#include <vector>
#include <string>

namespace Developer {

typedef boost::variant< int, std::string > atom_t;

struct label_t
{
    std::vector<atom_t> values;
    boost::optional<bool> marked;
};

/*!
 * \brief The node_t struct is a POD datatype for representing a node
 */
struct node_t
{
    //! The node's identifier
    std::string id;
    //! The node's label
    label_t label;
    //! The node's x coordinate within the canvas
    double xPos;
    //! The node's y coordinate within the canvas
    double yPos;
};

/*!
 * \brief The edge_t struct is a POD datatype for representing an edge
 */
struct edge_t
{
    //! The edge's identifier
    std::string id;
    //! The identifier of the "from" node
    std::string from;
    //! The identifier of the "to" node
    std::string to;
    //! The edge's label
    label_t label;
};

/*!
 * \brief The graph_t struct is a POD datatype for representing a graph
 */
struct graph_t
{
    //! The x dimension for the canvas in pixels
    double canvasX;
    //! The y dimension for the canvas in pixels
    double canvasY;
    //! A vector containing the graph's nodes
    std::vector<node_t> nodes;
    //! A vector containing the graph's edges
    std::vector<edge_t> edges;
};

/*!
 * \brief The param_t struct is a POD datatype for representing a parameter
 */
struct param_t
{
    //! The parameter's identifier
    std::string id;
    //! The paramater's datatype (list, atom, int, string)
    std::string type;
};

/*!
 * \brief The interface_t struct is a POD datatype for representing an interface
 *  between the LHS and RHS graphs
 */
struct interface_t
{
    //! The ID of the LHS node
    std::string lhsId;
    //! The ID of the RHS node
    std::string rhsId;
};

/*!
 * \brief The rule_t struct is a POD datatype for representing a rule
 */
struct rule_t
{
    //! The contents of an initial /*!...*/ comment
    std::string documentation;
    //! The rule's identifier
    std::string id;
    //! A vector containing the rule's parameters
    std::vector<param_t> parameters;
    //! The rule's LHS graph
    boost::optional<graph_t> lhs;
    //! The rule's RHS graph
    boost::optional<graph_t> rhs;
    //! The interface between the LHS and RHS
    std::vector<interface_t> interfaces;
    //! The rule's condition, there doesn't have to be one
    boost::optional<std::string> condition;
};

}

// Leave the Developer namespace so that these boost::fusion macro calls are in
// the global scope

BOOST_FUSION_ADAPT_STRUCT(
    Developer::label_t,
    (std::vector<Developer::atom_t>, values)
    (boost::optional<bool>, marked)
    )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::node_t,
        (std::string, id)
        (Developer::label_t, label)
        (double, xPos)
        (double, yPos)
        )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::edge_t,
        (std::string, id)
        (std::string, from)
        (std::string, to)
        (Developer::label_t, label)
        )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::graph_t,
        (double, canvasX)
        (double, canvasY)
        (std::vector<Developer::node_t>, nodes)
        (std::vector<Developer::edge_t>, edges)
        )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::param_t,
        (std::string, id)
        (std::string, type)
        )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::interface_t,
        (std::string, lhsId)
        (std::string, rhsId)
        )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::rule_t,
        (std::string, documentation)
        (std::string, id)
        (std::vector<Developer::param_t>, parameters)
        (boost::optional<Developer::graph_t>, lhs)
        (boost::optional<Developer::graph_t>, rhs)
        (std::vector<Developer::interface_t>, interfaces)
        (boost::optional<std::string>, condition)
        )

// Finished with boost::fusion now, return to Developer::

namespace Developer {

// A set of QDebug operator<< overloads to handle these custom types

QDebug operator<<(QDebug dbg, const std::string &str);
QDebug operator<<(QDebug dbg, const param_t &param);
QDebug operator<<(QDebug dbg, const std::vector<param_t> &params);
QDebug operator<<(QDebug dbg, const node_t &node);
QDebug operator<<(QDebug dbg, const std::vector<node_t> &nodes);
QDebug operator<<(QDebug dbg, const edge_t &edge);
QDebug operator<<(QDebug dbg, const std::vector<edge_t> &edges);
QDebug operator<<(QDebug dbg, const interface_t &interface);
QDebug operator<<(QDebug dbg, const std::vector<interface_t> &interfaces);
QDebug operator<<(QDebug dbg, const graph_t &graph);
QDebug operator<<(QDebug dbg, const rule_t &rule);

}

#endif // PARSERTYPES_HPP
