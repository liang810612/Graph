// ----------------------------
// projects/graph/TestGraph.c++
// Copyright (C) 2014
// Glenn P. Downing
// ----------------------------

// https://code.google.com/p/googletest/wiki/V1_7_Primer#Basic_Assertions

/*
Google Test Libraries:
    % ls -al /usr/include/gtest/
    ...
    gtest.h
    ...

    % locate libgtest.a
    /usr/lib/libgtest.a

    % locate libpthread.a
    /usr/lib/x86_64-linux-gnu/libpthread.a
    /usr/lib32/libpthread.a

    % locate libgtest_main.a
    /usr/lib/libgtest_main.a

To compile the test:
    % g++-4.7 -fprofile-arcs -ftest-coverage -pedantic -std=c++11 -Wall Graph.c++ TestGraph.c++ -o TestGraph -lgtest -lgtest_main -lpthread

To run the test:
    % valgrind TestGraph

To obtain coverage of the test:
    % gcov-4.7 -b Graph.c++ TestGraph.c++
*/

// --------
// includes
// --------

#include <iostream> // cout, endl
#include <iterator> // ostream_iterator
#include <sstream>  // ostringstream
#include <utility>  // pair

#include "boost/graph/adjacency_list.hpp"  // adjacency_list
#include "boost/graph/topological_sort.hpp"// topological_sort

#include "gtest/gtest.h"

#include "Graph.h"

// ---------
// TestGraph
// ---------

 #define DEFS   typedef typename TestFixture::graph_type         graph_type; \
                          typedef typename TestFixture::vertex_descriptor  vertex_descriptor; \
                          typedef typename TestFixture::edge_descriptor    edge_descriptor;\
                          typedef typename TestFixture::vertex_iterator    vertex_iterator;\
                          typedef typename TestFixture::edge_iterator      edge_iterator;\
                          typedef typename TestFixture::adjacency_iterator adjacency_iterator;\
                          typedef typename TestFixture::vertices_size_type vertices_size_type;\
                          typedef typename TestFixture::edges_size_type    edges_size_type;


template <typename G>
struct TestGraph : testing::Test {
    // --------
    // typedefs
    // --------

    typedef          G                     graph_type;
    typedef typename G::vertex_descriptor  vertex_descriptor;
    typedef typename G::edge_descriptor    edge_descriptor;
    typedef typename G::vertex_iterator    vertex_iterator;
    typedef typename G::edge_iterator      edge_iterator;
    typedef typename G::adjacency_iterator adjacency_iterator;
    typedef typename G::vertices_size_type vertices_size_type;
    typedef typename G::edges_size_type    edges_size_type;};

// directed, sparse, unweighted
// possibly connected
// possibly cyclic
typedef testing::Types<
            boost::adjacency_list<boost::setS, boost::vecS, boost::directedS>,
            Graph>
        my_types;

TYPED_TEST_CASE(TestGraph, my_types);
////////////////////////////////////////////////////////////////////////////////
TYPED_TEST(TestGraph, Vertex) {
    typedef typename TestFixture::graph_type         graph_type;
    typedef typename TestFixture::vertex_descriptor  vertex_descriptor;
    typedef typename TestFixture::edge_descriptor    edge_descriptor;
    typedef typename TestFixture::vertex_iterator    vertex_iterator;
    typedef typename TestFixture::edge_iterator      edge_iterator;
    typedef typename TestFixture::adjacency_iterator adjacency_iterator;
    typedef typename TestFixture::vertices_size_type vertices_size_type;
    typedef typename TestFixture::edges_size_type    edges_size_type;

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);

    vertex_descriptor vd = vertex(0, g);
    ASSERT_EQ(vdA, vd);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(1, vs);}

TYPED_TEST(TestGraph, Edge) {
    typedef typename TestFixture::graph_type         graph_type;
    typedef typename TestFixture::vertex_descriptor  vertex_descriptor;
    typedef typename TestFixture::edge_descriptor    edge_descriptor;
    typedef typename TestFixture::vertex_iterator    vertex_iterator;
    typedef typename TestFixture::edge_iterator      edge_iterator;
    typedef typename TestFixture::adjacency_iterator adjacency_iterator;
    typedef typename TestFixture::vertices_size_type vertices_size_type;
    typedef typename TestFixture::edges_size_type    edges_size_type;

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    edge_descriptor edAB = add_edge(vdA, vdB, g).first;

    std::pair<edge_descriptor, bool> p1 = add_edge(vdA, vdB, g);
    ASSERT_EQ(edAB,  p1.first);
    ASSERT_EQ(false, p1.second);

    std::pair<edge_descriptor, bool> p2 = edge(vdA, vdB, g);
    ASSERT_EQ(edAB, p2.first);
    ASSERT_EQ(true, p2.second);

    edges_size_type es = num_edges(g);
    ASSERT_EQ(1, es);

    vertex_descriptor vd1 = source(edAB, g);
    ASSERT_EQ(vdA, vd1);

    vertex_descriptor vd2 = target(edAB, g);
    ASSERT_EQ(vdB, vd2);}

TYPED_TEST(TestGraph, Vertices) {
    typedef typename TestFixture::graph_type         graph_type;
    typedef typename TestFixture::vertex_descriptor  vertex_descriptor;
    typedef typename TestFixture::edge_descriptor    edge_descriptor;
    typedef typename TestFixture::vertex_iterator    vertex_iterator;
    typedef typename TestFixture::edge_iterator      edge_iterator;
    typedef typename TestFixture::adjacency_iterator adjacency_iterator;
    typedef typename TestFixture::vertices_size_type vertices_size_type;
    typedef typename TestFixture::edges_size_type    edges_size_type;

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    std::pair<vertex_iterator, vertex_iterator> p = vertices(g);
    vertex_iterator                             b = p.first;
    vertex_iterator                             e = p.second;
    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdA, vd);}
    ++b;
    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdB, vd);}
    ++b;
    ASSERT_EQ(e, b);}

TYPED_TEST(TestGraph, Edges) {
    typedef typename TestFixture::graph_type         graph_type;
    typedef typename TestFixture::vertex_descriptor  vertex_descriptor;
    typedef typename TestFixture::edge_descriptor    edge_descriptor;
    typedef typename TestFixture::vertex_iterator    vertex_iterator;
    typedef typename TestFixture::edge_iterator      edge_iterator;
    typedef typename TestFixture::adjacency_iterator adjacency_iterator;
    typedef typename TestFixture::vertices_size_type vertices_size_type;
    typedef typename TestFixture::edges_size_type    edges_size_type;

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);

    edge_descriptor edAB = add_edge(vdA, vdB, g).first;
    edge_descriptor edAC = add_edge(vdA, vdC, g).first;

    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                           b = p.first;
    edge_iterator                           e = p.second;
    if (b != e) {
        edge_descriptor ed = *b;
        ASSERT_EQ(edAB, ed);}
    ++b;
    if (b != e) {
        edge_descriptor ed = *b;
        ASSERT_EQ(edAC, ed);}
    ++b;
    ASSERT_EQ(e, b);}

TYPED_TEST(TestGraph, Adjacent_Vertices) {
    typedef typename TestFixture::graph_type         graph_type;
    typedef typename TestFixture::vertex_descriptor  vertex_descriptor;
    typedef typename TestFixture::edge_descriptor    edge_descriptor;
    typedef typename TestFixture::vertex_iterator    vertex_iterator;
    typedef typename TestFixture::edge_iterator      edge_iterator;
    typedef typename TestFixture::adjacency_iterator adjacency_iterator;
    typedef typename TestFixture::vertices_size_type vertices_size_type;
    typedef typename TestFixture::edges_size_type    edges_size_type;

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);


    add_edge(vdA, vdB, g);
    add_edge(vdA, vdC, g);


    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;



    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdB, vd);}
    ++b;
    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdC, vd);}
    ++b;

    ASSERT_EQ(e, b);}
//////////////////////////////////////////////////////////////////////////////////////////

//--------
//add_edge
//--------

TYPED_TEST(TestGraph, Add_Edge_1) {
    DEFS
    
    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    edge_descriptor ed = add_edge(vdA, vdB, g).first;
    edge_descriptor ed2 = add_edge(vdA, vdB, g).first;

    ASSERT_EQ(ed, ed2);
    ASSERT_EQ(num_vertices(g), 2);
}

TYPED_TEST(TestGraph, Add_Edge_2) {
    DEFS
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(0, 1, g);

    ASSERT_EQ(1, num_edges(g));
    ASSERT_TRUE(a.second);
}

TYPED_TEST(TestGraph, Add_Edge_3) {
    DEFS
    
    graph_type g;
    vertex_descriptor vdA;
    vertex_descriptor vdB;
    edge_descriptor ed;

    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }
    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }

    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdB, vdA, g).first;
    }

    ASSERT_EQ(num_edges(g), 30);
    ASSERT_EQ(num_vertices(g), 60);
}

TYPED_TEST(TestGraph, Add_Edge_4) {
    DEFS
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(0, 1, g);
    std::pair<edge_descriptor, bool> b = add_edge(1, 2, g);
    std::pair<edge_descriptor, bool> c = add_edge(0, 2, g);
    std::pair<edge_descriptor, bool> d = add_edge(1, 3, g);
   // std::pair<edge_descriptor, bool> f = add_edge(1, 4, g);

    ASSERT_EQ(4, num_edges(g));
    ASSERT_TRUE(a.second);
    ASSERT_TRUE(b.second);
    ASSERT_TRUE(c.second);
    ASSERT_TRUE(d.second);
}

TYPED_TEST(TestGraph, Add_Edge_5) {
    DEFS
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(0, 1, g);
    std::pair<edge_descriptor, bool> b = add_edge(1, 2, g);
    std::pair<edge_descriptor, bool> d = add_edge(1, 2, g);

    ASSERT_EQ(2, num_edges(g));
    ASSERT_TRUE(a.second);
    ASSERT_TRUE(b.second);
    ASSERT_FALSE(d.second);
}

//----------
//add_vertex
//----------

TYPED_TEST(TestGraph, Add_vetex_1) {
    DEFS
    
    graph_type g;

    add_vertex(g);

    ASSERT_EQ(1, num_vertices(g));
}

TYPED_TEST(TestGraph, Add_vetex_2) {
    DEFS
    
    graph_type g;

    for(int i = 0; i < 100; ++i)
        add_vertex(g);
    ASSERT_EQ(100, num_vertices(g));
}

TYPED_TEST(TestGraph, Add_vetex_3) {
    DEFS
    
    graph_type g;

    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);

    ASSERT_EQ(12, num_vertices(g));
}


//-----------------
//adjacent_vertices
//-----------------

TYPED_TEST(TestGraph, Adjacent_Vertices_1) {
    DEFS
    
    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);

    add_edge(vdA, vdB, g);
    add_edge(vdA, vdC, g);

    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdB, vd);}
    ++b;
    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdC, vd);}
    ++b;
    ASSERT_EQ(e, b);}

TYPED_TEST(TestGraph, Adjacent_Vertices_2) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    
    for(int i = 0; i < 20; ++i)
    {
        vertex_descriptor temp = add_vertex(g);
        edge_descriptor temp2 = add_edge(vdA, temp, g).first;
    }

    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;

    adjacency_iterator test = b;
    for(int i = 0; i < 20; ++i)
        ++test;
    
    ASSERT_EQ(test, e);}

TYPED_TEST(TestGraph, Adjacent_Vertices_3) {
    DEFS

    graph_type g;
    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);
    vertex_descriptor vdD = add_vertex(g);
    edge_descriptor edD = add_edge(vdA, vdD, g).first;
    edge_descriptor edB = add_edge(vdA, vdB, g).first;
    edge_descriptor edA = add_edge(vdA, vdA, g).first;
    edge_descriptor edC = add_edge(vdA, vdC, g).first;

    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;

    ASSERT_EQ(*b, vdA);
    ++b;
    ASSERT_EQ(*b, vdB);
    ++b;
    ASSERT_EQ(*b, vdC);
    ++b;
    ASSERT_EQ(*b, vdD);
    ++b;
    ASSERT_EQ(b, e);}


// ----
// edge
// ----

TYPED_TEST(TestGraph, Edge_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    edge_descriptor edAB = add_edge(vdA, vdB, g).first;

    std::pair<edge_descriptor, bool> p1 = add_edge(vdA, vdB, g);
    ASSERT_EQ(edAB,  p1.first);
    ASSERT_EQ(false, p1.second);

    std::pair<edge_descriptor, bool> p2 = edge(vdA, vdB, g);
    ASSERT_EQ(edAB, p2.first);
    ASSERT_EQ(true, p2.second);

    edges_size_type es = num_edges(g);
    ASSERT_EQ(1, es);

    vertex_descriptor vd1 = source(edAB, g);
    ASSERT_EQ(vdA, vd1);

    vertex_descriptor vd2 = target(edAB, g);
    ASSERT_EQ(vdB, vd2);}

TYPED_TEST(TestGraph, Edge_2) {
    DEFS


    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    edge_descriptor ed = add_edge(vdA, vdB, g).first;
    edge_descriptor ed2 = edge(vdA, vdB, g).first;
    edge_descriptor ed3 = add_edge(vdB, vdA, g).first;
    edge_descriptor ed4 = edge(vdB, vdA, g).first;

    ASSERT_EQ(ed, ed2);
    ASSERT_EQ(ed3, ed4);
    ASSERT_EQ(num_vertices(g), 2);
}


TYPED_TEST(TestGraph, Edge_3) {
    DEFS

    graph_type g;
    add_edge(1, 1, g).second;
    ASSERT_TRUE(add_edge(4, 4, g).second);
    ASSERT_TRUE(add_edge(10, 10, g).second);
    pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                      b = p.first;
    edge_iterator                      e = p.second;
    ASSERT_EQ(11, num_vertices(g));
    ASSERT_EQ(3, num_edges(g));
}

TYPED_TEST(TestGraph, Edge_4) {
    DEFS

    graph_type g;
    
    ASSERT_TRUE(add_edge(1, 110, g).second);
    ASSERT_TRUE(add_edge(42, 99, g).second);
    ASSERT_TRUE(add_edge(0, 110, g).second);
    ASSERT_TRUE(add_edge(0, 1102, g).second);
    ASSERT_TRUE(add_edge(0, 9999, g).second);

    pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                      b = p.first;
    edge_iterator                      e = p.second;
    ASSERT_EQ(10000, num_vertices(g));
    ASSERT_EQ(5, num_edges(g));
}

TYPED_TEST(TestGraph, Edge_5) {
    DEFS

    graph_type g;
    add_edge(0, 1, g);
    ASSERT_TRUE(add_edge(1, 2, g).second);
    ASSERT_TRUE(add_edge(2, 3, g).second);
    pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                      b = p.first;
    edge_iterator                      e = p.second;
    ASSERT_EQ(4, num_vertices(g));
    ASSERT_EQ(3, num_edges(g));
}

// -----
// edges
// -----

TYPED_TEST(TestGraph, Edges_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);

    std::pair<edge_descriptor, bool> edAB = add_edge(vdA, vdB, g);
    std::pair<edge_descriptor, bool> edAC = add_edge(vdA, vdC, g);
    add_edge(vdB, vdC, g);

    ASSERT_TRUE(edAB.second);
    ASSERT_TRUE(edAC.second);

    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                           b = p.first;
    edge_iterator                           e = p.second;

    if (b != e) {
        edge_descriptor ed = *b;
        ASSERT_EQ(edAB.first, ed);}
    ++b;
    if (b != e) {
        edge_descriptor ed = *b;
        ASSERT_EQ(edAC.first, ed);}
    ++b;
    ++b;
    ASSERT_EQ(e, b);
}

TYPED_TEST(TestGraph, Edges_2) {
    DEFS

    graph_type g;

    vertex_descriptor v = add_vertex(g);

    ASSERT_EQ(0, num_edges(g));

    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator b = p.first;
    edge_iterator e = p.second;

    ASSERT_EQ(b, e);
}

TYPED_TEST(TestGraph, Edges_3) {
    DEFS

    graph_type g;

    vertex_descriptor vdA;
    vertex_descriptor vdB;
    edge_descriptor ed;

    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }
    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }

        std::pair<edge_iterator, edge_iterator> p = edges(g);
        edge_iterator                           b = p.first;
        edge_iterator                           e = p.second;

        edge_iterator temp = b;
        for(int i = 0; i < 20; ++i)
            ++temp;
        ASSERT_EQ(temp, e);    

        ASSERT_EQ(num_edges(g), 20);
        ASSERT_EQ(num_vertices(g), 40);
}

TYPED_TEST(TestGraph, edges_4) //edge from vertex to itself
{
    typedef typename TestFixture::graph_type         graph_type;
    typedef typename TestFixture::vertex_descriptor  vertex_descriptor;
    typedef typename TestFixture::edge_descriptor    edge_descriptor;
    typedef typename TestFixture::vertex_iterator    vertex_iterator;
    typedef typename TestFixture::edge_iterator      edge_iterator;
    typedef typename TestFixture::adjacency_iterator adjacency_iterator;
    typedef typename TestFixture::vertices_size_type vertices_size_type;
    typedef typename TestFixture::edges_size_type    edges_size_type;

    graph_type g;
    vertex_descriptor vdA = add_vertex(g);
    edge_descriptor edAA = add_edge(vdA, vdA, g).first;
    bool test = add_edge(vdA, vdA, g).second;

    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator b = p.first;
    edge_iterator e = p.second;

    edge_descriptor ed = *b;
    ASSERT_EQ(ed, edAA);

    ASSERT_EQ(num_edges(g), 1);
    ASSERT_FALSE(test);
}

// ---------
// num_edges
// ---------

TYPED_TEST(TestGraph, num_edges_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA;
    vertex_descriptor vdB;
    edge_descriptor ed;

    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }
    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }
    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }

    ASSERT_EQ(num_edges(g), 30);
    ASSERT_EQ(num_vertices(g), 60);
}

TYPED_TEST(TestGraph, num_edges2) {
    DEFS

    graph_type g;

    vertex_descriptor u = add_vertex(g);
    vertex_descriptor v = add_vertex(g);
    add_edge(u,v,g);
    add_edge(v,u,g);
    for(int i = 0; i < 100; ++i)
    {
        add_edge(u, i, g);
        add_edge(v, i, g);
    }

    ASSERT_EQ(200, num_edges(g));
}

TYPED_TEST(TestGraph, num_edges3) {
    DEFS

    graph_type g;

    add_edge(0, 3, g);
    add_edge(0, 5, g);
    add_edge(0, 6, g);   

    edges_size_type es = num_edges(g);

    ASSERT_EQ(3, es);

    add_edge(0, 4, g);
    add_edge(0, 2, g);
    add_edge(0, 2, g);
    add_edge(0, 2, g);

    std::pair<adjacency_iterator, adjacency_iterator > p = adjacent_vertices(0, g);

    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;

    for (int i = 2; b != e; ++b) {
        ASSERT_EQ(*b, i);
        ++i;
    }
}

// ------------
// num_vertices
// ------------

TYPED_TEST(TestGraph, num_vertices_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    std::pair<edge_descriptor, bool> pe = add_edge(0, 1, g);
                                     pe = add_edge(0, 1, g);


    std::pair<vertex_iterator, vertex_iterator> p = vertices(g);

    vertex_iterator vend = p.second;

    vertex_descriptor vd = vertex(1, g);
    ASSERT_EQ(vdB, vd);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(2, vs);
}
TYPED_TEST(TestGraph, num_vertices_2) {
    DEFS

    graph_type g;

    vertex_descriptor u = add_vertex(g);
    vertex_descriptor v = 999;
    add_edge(u,v,g);

    ASSERT_EQ(1000, num_vertices(g));
}
TYPED_TEST(TestGraph, num_vertices_3) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);
    vertex_descriptor vdD = add_vertex(g);

    vertex_descriptor vd = vertex(0, g);
    ASSERT_EQ(vdA, vd);

    vertex_descriptor ve = vertex(1, g);
    ASSERT_EQ(vdB, ve);

    vertex_descriptor vf = vertex(2, g);
    ASSERT_EQ(vdC, vf);

    vertex_descriptor vg = vertex(3, g);
    ASSERT_EQ(vdD, vg);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(4, vs);
}

// ------
// source
// ------

TYPED_TEST(TestGraph, source_1) {
    DEFS

    graph_type g;
    std::pair<edge_descriptor, bool> firstPair  = add_edge(0, 1, g);
    std::pair<edge_descriptor, bool> secondPair = add_edge(1, 0, g);

    edges_size_type es = num_edges(g);
    ASSERT_EQ(2, es);

    edge_descriptor edAB =  firstPair.first;
    edge_descriptor edBA = secondPair.first;

    vertex_descriptor src  = source(edAB, g);
    vertex_descriptor targ = target(edAB, g);

    std::pair<edge_descriptor, bool> thirdPair  = add_edge(src, targ, g);
    std::pair<edge_descriptor, bool> fourthPair = add_edge(targ, src, g);

    ASSERT_EQ(false,  thirdPair.second);
    ASSERT_EQ(false, fourthPair.second);
}

TYPED_TEST(TestGraph, source_2) {
    DEFS

    graph_type g;

    vertex_descriptor u = add_vertex(g);
    for(int i = 0; i < 100; ++i)
        add_edge(u, i, g);
    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator b = p.first;
    edge_iterator e = p.second;

    while(b != e)
    {
        edge_descriptor ed = *b;
        ++b;
        ASSERT_EQ(source(ed,g), u);
    }

    ASSERT_EQ(b, e);

}

TYPED_TEST(TestGraph, source_3) {
    DEFS

    graph_type g;
    vertex_descriptor vdA;
    vertex_descriptor vdB;
    edge_descriptor ed;

    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }
    for(int i = 0; i < 10; ++i)
    {
        vdA = add_vertex(g);
        vdB = add_vertex(g);
        ed = add_edge(vdA, vdB, g).first;
    }

    vertex_descriptor vA = source(ed, g);

    ASSERT_EQ(vA, 38);
    ASSERT_EQ(num_edges(g), 20);
    ASSERT_EQ(num_vertices(g), 40);}

TYPED_TEST(TestGraph, source_4) {
    DEFS

    graph_type g;
    ASSERT_EQ(source(add_edge(20, 1, g).first, g), 20);

}

TYPED_TEST(TestGraph, source_5) {
    DEFS

    graph_type g;
    ASSERT_EQ(source(add_edge(235, 1, g).first, g), 235);

}

// ------
// target
// ------

TYPED_TEST(TestGraph, target_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);

    edge_descriptor ed = add_edge(vdA, vdB, g).first;
    edge_descriptor ed2 = add_edge(vdC, vdA, g).first;

    vertex_descriptor vA = target(ed, g);
    vertex_descriptor vB = target(ed2, g);

    ASSERT_EQ(vA, 1);
    ASSERT_EQ(vB, 0);
    ASSERT_EQ(num_edges(g), 2);
    ASSERT_EQ(num_vertices(g), 3);
}
TYPED_TEST(TestGraph, target_2) {
    DEFS
                                                                       
    graph_type g;

    vertex_descriptor u = add_vertex(g);
    for(int i = 0; i < 100; ++i)
        add_edge(u, i, g);
    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator b = p.first;
    edge_iterator e = p.second;

    for(int i = 0; i < 100; ++i)
    {
        edge_descriptor ed = *b;
        ++b;
        ASSERT_EQ(target(ed,g), i);
    }

    ASSERT_EQ(b, e);}

TYPED_TEST(TestGraph, target_3) {
    DEFS

    graph_type g;

    ASSERT_EQ(target(add_edge(4, 7, g).first, g), 7) ;}

// ------
// vertex
// ------

TYPED_TEST(TestGraph, Vertex_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);

    vertex_descriptor vd = vertex(0, g);
    ASSERT_EQ(vdA, vd);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(1, vs);}

TYPED_TEST(TestGraph, Vertex_2) {
    DEFS

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    edge_descriptor edAB = add_edge(vdA, vdB, g).first;
    edge_descriptor edA5 = add_edge(vdA, 5, g).first;

    ASSERT_EQ(num_vertices(g), 6);}

TYPED_TEST(TestGraph, Vertex_3) {
    DEFS

    graph_type g;

    vertex_descriptor u = add_vertex(g);
    vertex_descriptor w = add_vertex(g);
    vertex_descriptor v = vertex(1, g);
    ASSERT_EQ(w,v);}

// --------
// vertices
// --------

TYPED_TEST(TestGraph, Vertices_1) {
    DEFS

    graph_type g;

    vertex_descriptor vdA;

    for(int i = 0; i < 50; ++i)
        vdA = add_vertex(g);

    std::pair<vertex_iterator, vertex_iterator> p = vertices(g);
    vertex_iterator                             b = p.first;
    vertex_iterator                             e = p.second;

    while(b != e){
        ++b;
    }

    ASSERT_EQ(e, b);}

TYPED_TEST(TestGraph, Vertices_2) {
    DEFS

    graph_type g;

    std::pair<edge_descriptor, bool> edgePair = add_edge(0, 5, g);
    edge_descriptor edAX = edgePair.first;

    ASSERT_TRUE(edgePair.second);

    std::pair<vertex_iterator, vertex_iterator> p = vertices(g);
    vertex_iterator vbegin = p.first;
    vertex_iterator   vend = p.second;
    
    boost::tie(vbegin, vend) = vertices(g);

    vertices_size_type vs = num_vertices(g);

    ASSERT_EQ(1, *(++vbegin));
    ASSERT_EQ(5, *(--vend)  );
    ASSERT_EQ(6, vs);
}

TYPED_TEST(TestGraph, Vertices_3) {
    DEFS

    graph_type g;
    vertex_descriptor v = add_vertex(g);
    add_edge(v, 100, g);
    std::pair<vertex_iterator, vertex_iterator> p = vertices(g);
    vertex_iterator b = p.first;
    vertex_iterator e = p.second;
    ASSERT_EQ(*b, v);
    ++b;
    --e;

    ASSERT_EQ(100,*e);}
