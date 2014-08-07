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

 #define TEST_FIXTURES    typedef typename TestFixture::graph_type         graph_type; \
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
    TEST_FIXTURES
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(0, 1, g);
    std::pair<edge_descriptor, bool> b = add_edge(0, 1, g);

    ASSERT_EQ(1, num_edges(g));
    ASSERT_TRUE(a.second);
    ASSERT_FALSE(b.second);
}

TYPED_TEST(TestGraph, Add_Edge_2) {
    TEST_FIXTURES
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(0, 1, g);
    std::pair<edge_descriptor, bool> b = add_edge(1, 2, g);

    ASSERT_EQ(2, num_edges(g));
    ASSERT_TRUE(a.second);
    ASSERT_TRUE(b.second);
}

TYPED_TEST(TestGraph, Add_Edge_3) {
    TEST_FIXTURES
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(1, 2, g);

    ASSERT_EQ(1, num_edges(g));
    ASSERT_EQ(3, num_vertices(g));
    ASSERT_TRUE(a.second);
}

TYPED_TEST(TestGraph, Add_Edge_4) {
    TEST_FIXTURES
    
    graph_type g;

    std::pair<edge_descriptor, bool> a = add_edge(0, 1, g);
    std::pair<edge_descriptor, bool> b = add_edge(1, 2, g);
    std::pair<edge_descriptor, bool> c = add_edge(0, 2, g);
    std::pair<edge_descriptor, bool> d = add_edge(1, 3, g);

    ASSERT_EQ(4, num_edges(g));
    ASSERT_TRUE(a.second);
    ASSERT_TRUE(b.second);
    ASSERT_TRUE(c.second);
    ASSERT_TRUE(d.second);
}

TYPED_TEST(TestGraph, Add_Edge_5) {
    TEST_FIXTURES
    
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
    TEST_FIXTURES
    
    graph_type g;

    add_vertex(g);

    ASSERT_EQ(1, num_vertices(g));
}

TYPED_TEST(TestGraph, Add_vetex_2) {
    TEST_FIXTURES
    
    graph_type g;

    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);

    ASSERT_EQ(5, num_vertices(g));
}

TYPED_TEST(TestGraph, Add_vetex_3) {
    TEST_FIXTURES
    
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
    TEST_FIXTURES
    
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
    TEST_FIXTURES

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);
    vertex_descriptor vdD = add_vertex(g);

    add_edge(vdD, vdC, g);
    add_edge(vdD, vdB, g);
    add_edge(vdD, vdA, g);

    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdB, g);
    std::pair<adjacency_iterator, adjacency_iterator> q = adjacent_vertices(vdD, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    adjacency_iterator b2 = q.first;
    adjacency_iterator e2 = q.second;

    if (b2 != e2) {
        vertex_descriptor vd2 = *b2;
        ASSERT_EQ(0, vd2);}
    ++b2;
        ASSERT_EQ(1, *b2);
    ++b2;
        ASSERT_EQ(2, *b2);
    ++b2;
    ASSERT_EQ(e2, b2);}

TYPED_TEST(TestGraph, Adjacent_Vertices_3) {
    TEST_FIXTURES

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);
    vertex_descriptor vdD = add_vertex(g);

    add_edge(vdA, vdB, g);
    add_edge(vdA, vdC, g);
    add_edge(vdB, vdD, g);
    add_edge(vdD, vdD, g);
    add_edge(vdD, vdB, g);
    add_edge(vdD, vdA, g);


    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdB, g);
    std::pair<adjacency_iterator, adjacency_iterator> q = adjacent_vertices(vdD, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    adjacency_iterator b2 = q.first;
    adjacency_iterator e2 = q.second;

    if (b != e) {
        vertex_descriptor vd = *b;
        ASSERT_EQ(vdD, vd);}
    ++b;
    
    ASSERT_EQ(e, b);
    if (b2 != e2) {
        vertex_descriptor vd2 = *b2;
        ASSERT_EQ(vdA, vd2);}
    ++b2;
        ASSERT_EQ(vdB, *b2);
    ++b2;
        ASSERT_EQ(vdD, *b2);
    ++b2;
    ASSERT_EQ(e2, b2);}

// ----------------
// constructor
// ----------------

TYPED_TEST(TestGraph, Constructor_1) {
    TEST_FIXTURES

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
    ASSERT_EQ(vdB, vd2);

    for (int i = 0; i < 50; ++i) {
        add_vertex(g);
    }

    {    
        graph_type g2(g);

        vertex_descriptor vdC = add_vertex(g2);
        vertex_descriptor vdD = add_vertex(g2);

        edge_descriptor edCD = add_edge(vdC, vdD, g2).first;

        std::pair<edge_descriptor, bool> q1 = add_edge(vdC, vdD, g2);
        ASSERT_EQ(edCD,  q1.first);
        ASSERT_EQ(false, q1.second);

        std::pair<edge_descriptor, bool> q2 = edge(vdC, vdD, g2);
        ASSERT_EQ(edCD, q2.first);
        ASSERT_EQ(true, q2.second);

        edges_size_type esg2 = num_edges(g2);

        ASSERT_EQ(2, esg2);

        vertex_descriptor testVertex = source(edCD, g2);
        ASSERT_EQ(vdC, testVertex);

        vertex_descriptor testVertex2 = target(edCD, g2);
        ASSERT_EQ(vdD, testVertex2);

        ASSERT_EQ(54, num_vertices(g2));
    }
}

TYPED_TEST(TestGraph, Constructor_2) {
    TEST_FIXTURES

    graph_type g;

    graph_type g2(g);

    edges_size_type    es_g = num_edges(g);
    vertices_size_type vs_g = num_vertices(g);

    ASSERT_EQ(0, es_g);    
    ASSERT_EQ(0, vs_g);

    edges_size_type    es_g2 = num_edges(g2);
    vertices_size_type vs_g2 = num_vertices(g2);

    ASSERT_EQ(0, es_g2);
    ASSERT_EQ(0, vs_g2);

    {
        std::pair<edge_iterator, edge_iterator> p = edges(g);
    
        edge_iterator                           b = p.first;
        edge_iterator                           e = p.second;
    
        ASSERT_EQ(e, b);
    }

    {
        std::pair<edge_iterator, edge_iterator> p = edges(g2);
    
        edge_iterator                           b = p.first;
        edge_iterator                           e = p.second;
    
        ASSERT_EQ(e, b);
    }

    {
        std::pair<vertex_iterator, vertex_iterator> p = vertices(g);
        vertex_iterator vbegin = p.first;
        vertex_iterator   vend = p.second;

        ASSERT_EQ(vbegin, vend);
    }
}

TYPED_TEST(TestGraph, Constructor_3) {
    TEST_FIXTURES

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
    ASSERT_EQ(vdB, vd2);

    {    
        graph_type g2(g);

        edge_descriptor edAB = add_edge(vdA, vdB, g2).first;

        std::pair<edge_descriptor, bool> p2 = edge(vdA, vdB, g2);
        ASSERT_EQ(edAB, p2.first);
        ASSERT_EQ(true, p2.second);

        edges_size_type es = num_edges(g2);
        ASSERT_EQ(1, es);

        vertex_descriptor vd1 = source(edAB, g2);
        ASSERT_EQ(vdA, vd1);

        vertex_descriptor vd2 = target(edAB, g2);
        ASSERT_EQ(vdB, vd2);
    }
}

// ----
// edge
// ----

TYPED_TEST(TestGraph, Edge_1) {
    TEST_FIXTURES

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
    TEST_FIXTURES

    graph_type g;
    add_edge(0, 1, g);
    add_edge(0, 2, g);

    
    std::pair<edge_descriptor, bool> edAD = edge(0, 3, g);
    std::pair<edge_descriptor, bool> edCD = edge(3, 4, g);
    std::pair<edge_descriptor, bool> edDE = edge(3, 40, g);

    edge_descriptor ed = edAD.first;

    ASSERT_EQ(ed, edAD.first);
    ASSERT_TRUE(!(edAD.second));
    ASSERT_TRUE(!(edCD.second));
    ASSERT_TRUE(!(edDE.second));
}

TYPED_TEST(TestGraph, Edge_3) {
    TEST_FIXTURES

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
    TEST_FIXTURES

    graph_type g;
    
    ASSERT_TRUE(add_edge(1, 1100, g).second);
    ASSERT_TRUE(add_edge(424, 999, g).second);
    ASSERT_TRUE(add_edge(0, 1102, g).second);
    ASSERT_TRUE(add_edge(0, 11020, g).second);
    ASSERT_TRUE(add_edge(0, 99999, g).second);

    pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                      b = p.first;
    edge_iterator                      e = p.second;
    ASSERT_EQ(100000, num_vertices(g));
    ASSERT_EQ(5, num_edges(g));
}

TYPED_TEST(TestGraph, Edge_5) {
    TEST_FIXTURES

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
    TEST_FIXTURES

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
    TEST_FIXTURES

    graph_type g;

    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);
    add_vertex(g);

    add_edge(0, 1, g);
    add_edge(0, 2, g);
    add_edge(0, 3, g);
    add_edge(0, 4, g);
    add_edge(0, 5, g);
    add_edge(0, 7, g);
    add_edge(1, 2, g);
    add_edge(1, 5, g);
    add_edge(2, 1, g);
    add_edge(4, 3, g);
    add_edge(4, 5, g);
    add_edge(6, 2, g);
    add_edge(7, 6, g);

    pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                      b = p.first;
    edge_iterator                      e = p.second;

    ASSERT_EQ(13, num_edges(g));
}

TYPED_TEST(TestGraph, Edges_3) {
    TEST_FIXTURES

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);

    //std::pair<edge_descriptor, bool> edAB           = 
    add_edge(vdA, vdB, g);
    std::pair<edge_descriptor, bool> edAB_duplicate;

    for (int i = 0; i < 100; ++i) {
        edAB_duplicate = add_edge(vdA, vdB, g);
        ASSERT_EQ(edAB_duplicate.second, false);
    }

    edges_size_type es = num_edges(g);

    ASSERT_EQ(1, es);
}

// ---------
// num_edges
// ---------

TYPED_TEST(TestGraph, num_edges_1) {
    TEST_FIXTURES

    graph_type g;

    std::pair<edge_descriptor, bool> edAD           = add_edge(0, 3, g);
    std::pair<edge_descriptor, bool> edAD_duplicate = add_edge(0, 3, g);

    ASSERT_TRUE(edAD.second);
    ASSERT_TRUE(!(edAD_duplicate.second));

    edges_size_type es = num_edges(g);

    ASSERT_EQ(1, es);

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

TYPED_TEST(TestGraph, num_edges2) {
    TEST_FIXTURES

    graph_type g;

    add_edge(0, 3, g);
    add_edge(0, 3, g);
    add_edge(0, 5, g);
    
    edges_size_type es = num_edges(g);

    ASSERT_EQ(2, es);

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

TYPED_TEST(TestGraph, num_edges3) {
    TEST_FIXTURES

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
    TEST_FIXTURES

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
    TEST_FIXTURES

    graph_type g;
    add_vertex(g);
    add_vertex(g);

    add_edge(3, 4, g);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(5, vs);
}
TYPED_TEST(TestGraph, num_vertices_3) {
    TEST_FIXTURES

    graph_type g;
    add_vertex(g);
    add_vertex(g);

    add_edge(3, 4, g);

    add_vertex(g);
    add_vertex(g);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(7, vs);
}

// ------
// source
// ------

TYPED_TEST(TestGraph, source_1) {
    TEST_FIXTURES

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
    TEST_FIXTURES

    graph_type g;
    ASSERT_EQ(source(add_edge(0, 1, g).first, g), 0);

}

TYPED_TEST(TestGraph, source_3) {
    TEST_FIXTURES

    graph_type g;
                 
    ASSERT_EQ(source(add_edge(209, 3, g).first, g), 209) ;}

TYPED_TEST(TestGraph, source_4) {
    TEST_FIXTURES

    graph_type g;
    ASSERT_EQ(source(add_edge(10, 1, g).first, g), 10);

}

TYPED_TEST(TestGraph, source_5) {
    TEST_FIXTURES

    graph_type g;
    ASSERT_EQ(source(add_edge(2345, 1, g).first, g), 2345);

}

// ------
// target
// ------

TYPED_TEST(TestGraph, target_1) {
    TEST_FIXTURES

    graph_type g;
    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(2, 0, g);

    vertex_descriptor start;

    std::pair<edge_iterator, edge_iterator> edgesPair = edges(g);

    edge_iterator b = edgesPair.first;
    edge_iterator e = edgesPair.second;

    while (b != e) {
        start = target(*b, g);
        ++b;
    }

    ASSERT_EQ(0, start);
}
TYPED_TEST(TestGraph, target_2) {
    TEST_FIXTURES
                                                                       
    graph_type g;

    ASSERT_EQ(target(add_edge(209, 3, g).first, g), 3) ;}

TYPED_TEST(TestGraph, target_3) {
    TEST_FIXTURES

    graph_type g;

    ASSERT_EQ(target(add_edge(4, 7, g).first, g), 7) ;}

// ------
// vertex
// ------

TYPED_TEST(TestGraph, Vertex_1) {
    TEST_FIXTURES

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);

    vertex_descriptor vd = vertex(0, g);
    ASSERT_EQ(vdA, vd);

    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(1, vs);}

TYPED_TEST(TestGraph, Vertex_2) {
    TEST_FIXTURES

    graph_type g;

    
    ASSERT_EQ(vertex(3,g), 3);}

TYPED_TEST(TestGraph, Vertex_3) {
    TEST_FIXTURES

    graph_type g;
    ASSERT_EQ(vertex(-2,g), -2);}

// --------
// vertices
// --------

TYPED_TEST(TestGraph, Vertices_1) {
    TEST_FIXTURES

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

TYPED_TEST(TestGraph, Vertices_2) {
    TEST_FIXTURES

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
    TEST_FIXTURES

    graph_type g;

    vertex_descriptor vdA = add_vertex(g);
    vertex_descriptor vdB = add_vertex(g);
    vertex_descriptor vdC = add_vertex(g);

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
        ASSERT_EQ(vdC, *b);
    ++b;
    ASSERT_EQ(e, b);}

// -----
// valid
// -----

// TYPED_TEST(TestGraph, valid_1) {
//     TEST_FIXTURES

//     graph_type g;
//     ASSERT_TRUE(g.valid());
// }