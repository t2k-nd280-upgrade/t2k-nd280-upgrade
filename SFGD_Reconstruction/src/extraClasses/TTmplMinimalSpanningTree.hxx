#ifndef TTmplMinimalSpanningTree_hxx_seen
#define TTmplMinimalSpanningTree_hxx_seen

#include <vector>
#include <list>

template <typename T, typename EdgeWeight, typename UserDataType = int>
/// This implements Prim's algorithm for finding a minimal spanning tree (MST)
/// for a graph with edges between all vertices.  The weight for an edge
/// between a pair of vertices is defined by the EdgeWeight functor which is
/// defined by the user.  For a graph of points in space, the EdgeWeight is
/// usually the distance between the points. The distance between points must
/// be greater than zero.
///
/// The MST is available as a constant vector of TreeVertex objects.  The
/// TreeVertex object has publicly accessible fields named Object, UserData,
/// Parent, Children, VertexDepth, EdgeSum, and ParentEdge.  These feels are
/// fully documented below in the TreeVertex class.  The first element in the
/// vector is the root of the MST.  The root is chosen in MakeTree as the
/// vertex in the set of vertices added using AddVertices (or AddVertex) that
/// is closest to the MakeTree rootVertex input argument .
///
/// The MST for a particular root vertex is unique, except when the graph
/// contains cycles with two or more equal weight edges.  In this case, there
/// is ambiguity about how the tree will go through the cycle (for this
/// implementation, the choice is determined by the order that vertices are
/// presented in AddVertices or AddVertex).  If the graph is expected to have
/// equal weight edges, then the ambiguity can be reduced by a careful choice
/// of the EdgeWeight so that the structure of the graph is essentially
/// unchanged, but the number of equal weight edges are reduced.  See the
/// example below where this is done.
///
/// Here's a example of a graph with an ambiguous MST for a root at vertex
/// one.
///
///   1 - 2
///   |   |
///   4 - 3
///
/// In this case, the trees rooted at vertex one are:
///
///  {[1,2], [2,3], [3,4]}
///  {[1,4], [3,4], [2,3]}
///  {[1,2], [1,4], [2,3]}
///  {[1,2], [1,4], [3,4]}
///
/// Algorithm References:
///
/// @ARTICLE{Jarnik:mst,
///     author = {Vojtech Jarnik},
///     title = {About a certain minimal problem},
///     journal = {Prace Moravske Prirodovedecke Spolecnosti (in Czech)},
///     volume = {6},
///     issue = {4},
///     year = {1930},
///     pages = {57--63}
/// }
/// @ARTICLE{Prim:mst,
///     author = {Robert Clay Prim},
///     title = {Shortest connection networks and some generalizations},
///     journal = {Bell System Technical Journal},
///     volume = {36},
///     issue = {6},
///     year = {1957},
///     pages = {1389--1401}
/// }
///
/// This TTmplMinimalSpanningTree template is designed to define a MST for
/// space points.  For instance, if you have a vertex defined as a vector, and
/// an edge weight defined as the distance between the vertices.
///
/// \code
/// class vector{float x; float y; float z};
/// class vectorDist {
///     float operator(const vector& lhs, const vector& rhs) {
///        float d = lhs.x-rhs.x;
///        float dist = d*d;
///        d = lhs.y-rhs.y;
///        dist += d*d;
///        d = lhs.z-rhs.z;
///        dist += d*d;
///        return std::sqrt(dist);
///     }
/// };
/// \endcode
///
/// Assuming you have a container full of vector objects with
///
/// \code
/// STANDARD_CONTAINER vectors;
/// typedef TTmplMinimalSpanningTree<vector,vectorDist,int> VectorMST;
/// VectorMST mst;
/// mst.AddVertices(vectors.begin(), vectors.end());
/// mst.MakeTree(vectors.first());
/// \endcode
///
/// The output of the tree can then be used by
///
/// \code
/// int deepest = 0;
/// int depth = -1;
/// const VectorMST::MinimalSpanningTree& tree = mst.GetTree();
/// for (int i = 0; i< tree.size(); ++i) {
///     if (depth < 0 || depth > tree[i].VertexDepth) {
///         depth = tree[i].VertexDepth;
///         deepest = i;
///     }
/// }
/// \endcode
///
/// From any vertex, you can go toward the root by using the Parent field.
///
/// \code
/// int v = deepest;
/// while (tree[v].Parent >= 0) {
///    v = tree[v].Parent;
/// }
/// \endcode
///
/// The UserData field is mutable, and you can change it (even though the tree
/// is a constant object).  The class of the UserData field can be specified
/// as a template parameter, and is usefule to store information generated why
/// traversing the tree.  For example, it can be used to mark if a particular
/// vertex has already been visited, or to contain the total charge of all of
/// the children (or both).
///
/// EXAMPLE: Handling pathelogical sets of vertices -- When the vertices are
/// on a regular grid (for example an equally spaced grid of pixels/voxels),
/// there are many special cases where an MST based on the distance between
/// the vertices will not be unique (see the example above).  In this common,
/// but pathelogic case, the degeneracy can often be broken using an intensity
/// associated with the pixel, (e.g. q), as well as a minimum spacing between
/// the pixels, (e.g. spacing).  An example of a EdgeWeight that will help
/// break the ambiguities is
///
/// \code
/// struct pixel{double x; double y; double q};
///
/// struct pixelDist {
///     double operator (const pixel& lhs, const pixel& rhs) {
///         const double spacing = 1.0;
///         double dist = hypot( lhs.x-rhs.x, lhs.y-rhs.y);
///         double qWeight = 0.1*spacing*exp(-1.0 - lhs.q - rhs.q));
///         return dist + qWeight;
///     }
/// };
/// \endcode
///
/// Copyright (C) 2019 Clark McGrew
///
/// Permission is hereby granted, free of charge, to any person obtaining a
/// copy of this software (the "Software"), to deal in the Software without
/// restriction, including without limitation the rights to use, copy, modify,
/// merge, publish, distribute, sublicense, and/or sell copies of the
/// Software, and to permit persons to whom the Software is furnished to do
/// so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be
/// included in all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
/// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
/// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
/// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
class TTmplMinimalSpanningTree {
public:
    TTmplMinimalSpanningTree() {};
    virtual ~TTmplMinimalSpanningTree() {};
    
    /// A class to hold each vertex in the tree.  The object holds several
    /// fields that are useful for the user and which are publicly exposed.
    /// This is available to the user as an element in a constant vector
    /// containing the minimal spanning tree.
    class TreeVertex {
    public:
        /// The object at this vertex.
        T Object;
        
        /// Some extra data the user might attach the vertex.  This defaults to
        /// being an integer, but could be any type.  It's set to mutable so it
        /// can be modified even when the tree is constant.
        mutable UserDataType UserData;
        
        /// The index in the MinimalSpanningTree of the parent of this vertex.
        /// If the vertex is the root of the tree, this will have a value of
        /// negative one.
        int Parent;
        
        typedef std::vector<std::size_t> Child;
        
        /// The indicies in the MinimalSpanningTree of the children of this
        /// vertex
        Child Children;
        
        /// The number of vertices between this vertex and the root.  The root
        /// has a VertexDepth of zero, the children of the root have a
        /// VertexDepth of one.
        int VertexDepth;
        
        /// The sum of the edge weights between this vertex and the root.
        /// The root has a EdgeSum of zero.
        double EdgeSum;
        
        /// The weight of the edge from this vertex to the parent.  The root
        /// has a ParentEdge of zero.
        double ParentEdge;
    };
    
    typedef std::vector<TreeVertex> MinimalSpanningTree;
    
    /// Reinitialize the class.  This clears all of the internal data
    /// structures.
    void Clear() {
        fVertices.clear();
        fTree.clear();
    }
    
    /// Add a container worth of vertices to be built into an MST.  This
    /// supports any container with a forward or backward iterator.  The
    /// object referenced by the iterator must be convertible to correct
    /// object type.  If the class contains an existing MST, it will be
    /// cleared.
    template <typename InputIterator>
    void AddVertices(InputIterator begin, InputIterator end) {
        while (begin != end) {
            fVertices.push_back(*begin);
            ++begin;
        }
        if (!fTree.empty()) fTree.clear();
    }
    
    /// Add a single vertex to be built into an MST.  This can be used in
    /// conjunction with the AddVertices method.  If the class contains an
    /// existing MST, it will be cleared.
    void AddVertex(T vertex) {
        fVertices.push_back(vertex);
        if (!fTree.empty()) fTree.clear();
    }
    
    /// Build a MST from the existing vertices.  The root vertex will be
    /// chosen to be the object that is closest to the input rootVertex.
    void MakeTree(T& rootVertex) {
        // Make sure the fTree is empty, and then reserve enough space so that
        // pushes are "cheap".
        fTree.clear();
        fTree.reserve(fVertices.size());
        
        // An iterator to memorize the closest vertex that has been found.
        // It's reused several times.  It's initialized with an invalid value.
        VertexIterator closest = fVertices.end();
        
        // The edge weight associated with the closest vertex.  It's
        // initialized with an invalid value.
        double minimumEdge = -1.0;
        
        // A tree vertex that used to build a new vertex before pushing it
        // onto the tree.
        TreeVertex tv;
        
        // Find the first vertex for the tree.  This will be the closest
        // vertex to the rootVertex.  Normally that will be the rootVertex,
        // but the provided rootVertex doesn't have to exist in the set of
        // vertices that will be built into the tree.
        for (VertexIterator v = fVertices.begin();
             v != fVertices.end(); ++v) {
            double edge = fEdgeWeight(rootVertex,*v);
            if (minimumEdge < 0 || edge < minimumEdge) {
                minimumEdge = edge;
                closest = v;
            }
        }
        tv.Object = *closest;
        tv.Parent = -1;
        tv.VertexDepth = 0;
        tv.EdgeSum = 0.0;
        tv.ParentEdge = 0.0;
        fTree.push_back(tv);
        fVertices.erase(closest);
        
        // Run the simplest version of Prim's algorithm.  This keeps adding
        // verticies until there aren't any more.  Nota Bene: This is an n^2
        // implementation!
        while (!fVertices.empty()) {
            std::size_t parent = 0;
            closest = fVertices.end();
            minimumEdge = -1;
            for (std::size_t t = 0; t<fTree.size(); ++t) {
                for (VertexIterator v = fVertices.begin();
                     v != fVertices.end(); ++v) {
                    double edge = fEdgeWeight(fTree[t].Object, *v);
                    if (minimumEdge < 0 || edge < minimumEdge) {
                        minimumEdge = edge;
                        closest = v;
                        parent = t;
                    }
                }
            }
            tv.Parent = parent;
            tv.Object = *closest;
            tv.VertexDepth = fTree[parent].VertexDepth + 1;
            tv.ParentEdge = minimumEdge;
            tv.EdgeSum = fTree[parent].EdgeSum + tv.ParentEdge;
            fTree[parent].Children.push_back(fTree.size());
            fTree.push_back(tv);
            fVertices.erase(closest);
        }
    }
    
    /// Return a constant reference to the MST.
    const MinimalSpanningTree& GetTree() {
        return fTree;
    }
    
private:
    
    typedef std::list<T> VertexList;
    typedef typename VertexList::iterator VertexIterator;
    
    /// A class that calculates the distance between points.  The EdgeWeight
    /// class must define (at least) an method equivalent to "double
    /// operator() (T lhs, T rhs)".  Examples of possible operators are
    /// \code
    /// double operator() (T lhs, T rhs);
    /// double operator() (const T& lhs, const T& rhs);
    /// \endcode
    EdgeWeight fEdgeWeight;
    
    /// The internal tree that is constructed.  It is exposed to the user by
    /// GetTree();
    MinimalSpanningTree fTree;
    
    // An internal field that tracks the vertices that still need to be
    // attached.
    VertexList fVertices;
};
#endif

#ifdef TTmplMinimalSpanningTree_test_code
// This is the code for a simple test program.  It can be used by copying this
// file to "test_code.cxx" and running
// g++ -DTTmplMinimalSpanningTree_test_code test_code.cxx
// ./a.out

#include <iostream>
#include <cmath>

class pixel {
public:
    double x;
    double y;
    double q;
};

struct pixelDist {
    double operator ()(const pixel& lhs, const pixel& rhs) {
        double dist = std::hypot(lhs.x-rhs.x, lhs.y-rhs.y);
        double weight = 0.1*std::exp(-1.0-lhs.q-rhs.q);
        return dist + weight;
    }
};

int main(int argc, char**argv) {
    // A vector of 2D vertices with a "charge" in the last element.
    pixel vtx[12] = {
        {0, 0, 3},  {1, 1, 3}, {2, 2, 3}, {3, 3, 4}, {4, 4, 3}, {5, 5, 3},
        {3, 4, 2},  {3, 5, 2},  {3, 6, 2},
        {0, 1, 1}, {1, 0, 2}
    };
    
    typedef TTmplMinimalSpanningTree<pixel,pixelDist,double> VtxMST;
    VtxMST mst;
    
    mst.AddVertices(&vtx[0], &vtx[11]);
    
    mst.MakeTree(vtx[0]);
    
    const VtxMST::MinimalSpanningTree& tree = mst.GetTree();
    
    for (int i = 0; i < tree.size(); ++i) {
        tree[i].UserData = 0.1*i; // Check the user data is mutable
        std::cout << i
        << " parent " << tree[i].Parent
        << " x " << tree[i].Object.x
        << " y " << tree[i].Object.y
        << " q " << tree[i].Object.q
        << " data " << tree[i].UserData
        << " depth " << tree[i].VertexDepth
        << " root edge " << tree[i].EdgeSum
        << " parent edge " << tree[i].ParentEdge
        << std::endl;
        std::cout << "          ";
        for (VtxMST::TreeVertex::Child::const_iterator
             c = tree[i].Children.begin();
             c !=  tree[i].Children.end(); ++c) {
            std::cout << " " << *c;
        }
        std::cout << std::endl;
    }
    
    return 0;
}
#endif
