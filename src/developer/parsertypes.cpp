/*!
 * \file
 */
#include "parsertypes.hpp"

namespace Developer {

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
