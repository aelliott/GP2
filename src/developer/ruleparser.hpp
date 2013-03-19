#ifndef RULEPARSER_HPP
#define RULEPARSER_HPP

#include <QDebug>

#include <boost/variant/recursive_variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include <vector>
#include <string>

namespace Developer {

struct node_t
{
    std::string id;
    std::string label;
    double xPos;
    double yPos;
};

struct edge_t
{
    std::string from;
    std::string to;
    std::string label;
};

struct graph_t
{
    double canvasX;
    double canvasY;
    std::vector<node_t> nodes;
    std::vector<edge_t> edges;
};

struct param_t
{
    std::string id;
    std::string type;
};

struct rule_t
{
    std::string id;
    std::vector<param_t> parameters;
    graph_t lhs;
};

rule_t parseRule(const std::string &rule);

}

BOOST_FUSION_ADAPT_STRUCT(
        Developer::node_t,
        (std::string, id)
        (std::string, label)
        (double, xPos)
        (double, yPos)
        )

BOOST_FUSION_ADAPT_STRUCT(
        Developer::edge_t,
        (std::string, from)
        (std::string, to)
        (std::string, label)
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
        Developer::rule_t,
        (std::string, id)
        (std::vector<Developer::param_t>, parameters)
        (Developer::graph_t, lhs)
        )

namespace Developer {

QDebug operator<<(QDebug dbg, const std::string &str);
QDebug operator<<(QDebug dbg, const param_t &param);
QDebug operator<<(QDebug dbg, const std::vector<param_t> &params);
QDebug operator<<(QDebug dbg, const std::pair<double, double> &coords);
QDebug operator<<(QDebug dbg, const node_t &node);
QDebug operator<<(QDebug dbg, const std::vector<node_t> &nodes);
QDebug operator<<(QDebug dbg, const edge_t &edge);
QDebug operator<<(QDebug dbg, const std::vector<edge_t> &edges);
QDebug operator<<(QDebug dbg, const graph_t &graph);
QDebug operator<<(QDebug dbg, const rule_t &rule);

}

#endif // RULEPARSER_HPP
