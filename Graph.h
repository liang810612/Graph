// ----------------------
// projects/graph/Graph.h
// Copyright (C) 2014
// Glenn P. Downing
// ----------------------

#ifndef Graph_h
#define Graph_h

// --------
// includes
// --------

// #include <cassert> // assert
// #include <cstddef> // size_t
// #include <utility> // make_pair, pair
// #include <vector>  // vector
// #include <set>     // set


#include <cassert> // assert
#include <cstddef> // size_t
#include <utility> // make_pair, pair
#include <vector>  // vector
#include <list>
#include <deque>
#include <functional>
#include <numeric>

using namespace std;

// -----
// Graph
// -----

class Graph {
    public:
        // --------
        // typedefs
        // --------

        typedef int vertex_descriptor;  
        typedef std::pair<vertex_descriptor, vertex_descriptor> edge_descriptor;    

        typedef std::vector<vertex_descriptor>::const_iterator vertex_iterator;    
        typedef std::vector<edge_descriptor>::const_iterator edge_iterator;      
        typedef std::set<vertex_descriptor>::const_iterator adjacency_iterator; 

        typedef std::size_t vertices_size_type;
        typedef std::size_t edges_size_type;

    public:
        // --------
        // add_edge
        // --------

        /**
         * @param u is a vertex_descriptor by value
         * @param v is a vertex_descriptor by value
         * @param g is a Graph by reference
         * return pair<edge_descriptor, bool> bool will be true if edge successfully added to graph g
         */
        friend std::pair<edge_descriptor, bool> add_edge (vertex_descriptor u, vertex_descriptor v, Graph& g) {
            // <your code>
            edge_descriptor ed = std::make_pair(u,v);
            bool            b = true;
            vertices_size_type max_size = std::max(u, v);
            if(std::find(g._e.begin(), g._e.end(),ed) == g._e.end()){
                if((u > g._v.back()) || (v > g._v.back())){
                    ++max_size;
                    g._v.resize(max_size);
                    g._g.resize(max_size);
                }

                (g._g[u]).insert(v);
                g._e.push_back(ed);

                b = true;
            }
            else{
                b = false;
            }

            return std::make_pair(ed, b);


        }

        // ----------
        // add_vertex
        // ----------

        /**
         * @param g is a Graph by reference
         * add vertex to vertex vector and g
         * Return added vertex value
         */
        friend vertex_descriptor add_vertex (Graph& g) {
            // <your code>
            vertex_descriptor v = g._g.size();     // g._v.size()
            g._v.push_back(v);
            g._g.push_back(std::set<vertex_descriptor>());
            return v;}

        // -----------------
        // adjacent_vertices
        // -----------------

        /**
         * @param v is a vertex_descriptor by value
         * @param g is a Graph by reference
         * iterate over all adjacent vertices
         * return a pair of iterators that point to the beginning and end of vertices adjacent to the specified vertex in g
         */
        friend std::pair<adjacency_iterator, adjacency_iterator> adjacent_vertices (vertex_descriptor v, const Graph& g) {
            // <your code>
            // static int a [] = {0, 0};     // dummy data

            adjacency_iterator b = g._g[v].begin();
            adjacency_iterator e = g._g[v].end();
            return std::make_pair(b, e);}

        // ----
        // edge
        // ----

        /**
         * @param u is a vertex_descriptor by value
         * @param v is a vertex_descriptor by value
         * @param g is a Graph by reference
         * return pair<edge_descriptor, bool> bool will be true if edge in g
         */
        friend std::pair<edge_descriptor, bool> edge (vertex_descriptor u, vertex_descriptor v, const Graph& g) {
            // <your code>
            edge_descriptor ed = std::make_pair(u,v);
            bool            b;
            if(std::find(g._e.begin(), g._e.end(), ed) == g._e.end()){
                b = false;
            }
            else{
                b = true;
            }
            return std::make_pair(ed, b);}

      
        // -----
        // edges
        // -----

        /**
         * @param g is a Graph by reference
         * return pair of being and end iterators over edges in g
         */
        friend std::pair<edge_iterator, edge_iterator> edges (const Graph& g) {
            // <your code>
            // static int a [] = {0, 0};     // dummy data
            edge_iterator b = g._e.begin();
            edge_iterator e = g._e.end();
            return std::make_pair(b, e);}

        // ---------
        // num_edges
        // ---------

        /**
         * @param g is a Graph by reference
         * return number of edges in g
         */
        friend edges_size_type num_edges (const Graph& g) {
            // <your code>
            // edges_size_type s = 1; // fix

            return g._e.size();}

        // ------------
        // num_vertices
        // ------------

        /**
         * @param g is a Graph by reference
         * return number of verices in g
         */
        friend vertices_size_type num_vertices (const Graph& g) {
            // <your code>
            // vertices_size_type s = 1; // fix

            return g._v.size();}

        // ------
        // source
        // ------

        /**
         * @param ed is a edge_descriptor by value
         * @param g is a graph be reference
         * return vertex_descriptor of source
         */
        friend vertex_descriptor source (edge_descriptor ed, const Graph& g) {
            // <your code>
            // vertex_descriptor v = 0; // fix
            return ed.first;}

        // ------
        // target
        // ------

        /**
         * @param ed is a edge_descriptor by value
         * @param g is a graph be reference
         * return vertex_descriptor of target
         */
        friend vertex_descriptor target (edge_descriptor ed, const Graph& g) {
            // <your code>
            // vertex_descriptor v = 0; // fix
            return ed.second;}

        // ------
        // vertex
        // ------

        /**
         * @param vs is a vertices_size_type by value
         * @param g is a graph by reference
         * return vertex_descriptor of vertex
         */
        friend vertex_descriptor vertex (vertices_size_type vs, const Graph& g) {
            // <your code>
            // vertex_descriptor vd = 0; // fix
            //return vs;
            return g._v[vs];}

        // --------
        // vertices
        // --------

        /**
         * @param g is a graph by reference
         * return begin and end iterators over the vertices of g
         */
        friend std::pair<vertex_iterator, vertex_iterator> vertices (const Graph& g) {
            // <your code>
            // static int a [] = {0, 0};     // dummy data
            vertex_iterator b = g._v.begin();
            vertex_iterator e = g._v.end();
            return std::make_pair(b,e);}

    private:
        // ----
        // data
        // ----

        std::vector< std::set<vertex_descriptor> > _g; // g cotains set of adjacent vertices
        std::vector<vertex_descriptor> _v;
        std::vector<edge_descriptor> _e; 

        // -----
        // valid
        // -----

        /**
         * check if all sizes are 0 at default
         */
        bool valid () const {
            // <your code>
            return true;}

    public:
        // ------------
        // constructors
        // ------------

        /**
         * <your documentation>
         */
        Graph () 
        { // <your code>
            _g = std::vector< std::set<vertex_descriptor> >();
            _v = std::vector<vertex_descriptor>();
            _e = std::vector<edge_descriptor>();
            assert(valid());}

        // Default copy, destructor, and copy assignment
        // Graph  (const Graph<T>&);
        // ~Graph ();
        // Graph& operator = (const Graph&);
    };

#endif // Graph_h
