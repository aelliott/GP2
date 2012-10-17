#ifndef GRAPH_HPP
#define GRAPH_HPP

// This includes "node.hpp" by proxy
#include "edge.hpp"

class Graph
{
public:
    /*!
     * Describes the format the graph should be expressed in.
     */
    enum GraphTypes
    {
        //! Default format: use the default defined in the program settings
        Default,
        //! Dot format
        Dot,
        //! GXL (Graph eXchange Language) format
        GXL
    };

    Graph();
};

#endif // GRAPH_HPP
