#ifndef TTmplDensityCluster_seen
#define TTmplDensityCluster_seen

#include <vector>
#include <list>
#include <functional>
#include <algorithm>


    template <typename T>
    struct decreasingClusterSize :
        public std::binary_function <T, T, bool > {
        bool operator() (const T& lhs, const T& rhs) {
            return lhs.size() > rhs.size();
        }
    };
    




template <typename T, typename MetricModel>
/// For a detailed description of the density-based clustering, Google keyword:
/// density-based clustering, or DBSCAN.  The first template argument provides
/// the class type for the elements that will be clustered, and the second
/// provides is a functor which calculates a distance metric between two
/// elements.  
///
/// Algorithm Reference
/// @INPROCEEDINGS{Ester96:dbscan,
///     author = {Martin Ester and Hans-peter Kriegel and Jörg S
///               and Xiaowei Xu},
///     title = {A density-based algorithm for discovering clusters in
///              large spatial databases with noise},
///     booktitle = {},
///     year = {1996},
///     pages = {226--231},
///     publisher = {AAAI Press}
/// }
///
/// CubeRecon Implementation (this template): Le Phuoc Trung, Clark McGrew
///
/// Example: A template class that performs density-based clustering using the
/// DBSCAN algorithm with a customized metric. Users have to define a class
/// that calculates the 'distance' between two points. For example, a template
/// class for clustering hits using time information:
/// \code
/// template<typename T>
/// class TimeDistance {
/// public:
///    double operator(T lhs, T rhs) {
///        return std::abs(lhs->GetTime() - rhs->GetTime());
///    }
/// };
/// \endcode
/// 
/// Example: Template class for clustering hits using position the distance in
/// the P0D X/Y plane is:
/// \code
/// template<typename T>
/// class SpatialDistance {
/// public:
///    double operator(T lhs, T rhs) {
///        double x1 = lhs->GetPosition().Z();
///        double y1 = 0.0;
///        if (lhs->IsXHit()) y1 = lhs->GetPosition().X();
///        if (lhs->IsYHit()) y1 = lhs->GetPosition().Y();
///        
///        double x2 = rhs->GetPosition().Z();
///        double y2 = 0.0;
///        if (rhs->IsXHit()) y2 = rhs->GetPosition().X();
///        if (rhs->IsYHit()) y2 = rhs->GetPosition().Y();
///        
///        return std::sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
///    }
/// };
/// \endcode
/// Here is a code snippet using the SpacialDistance template:
/// \code
///    // Work around template parsing bug in some GCC versions...
///    typedef CP::THandle<CP::THit> Arg;
///   
///    // Make a typedef for the ClusterAlgorithm.
///    typedef TTmplDensityCluster<Arg, SpatialDistance<Arg> > ClusterAlgorithm;
///
///    const double maxDist = 20*unit::cm;
///    const unsigned int minPoints = 4;
///    
///    std::unique_ptr<ClusterAlgorithm>
///        xcluster(new ClusterAlgorithm(minPoints, maxDist));
///    xcluster->Cluster(xHits);
///
///    std::unique_ptr<ClusterAlgorithm>
///        ycluster(new ClusterAlgorithm(minPoints, maxDist));
///    ycluster->Cluster(yHits);
/// \endcode
///
/// Example: If you know the exact type that will be passed to the
/// MetricModel, your metric model class doesn't need to be a template.  For
/// instance, if you want to cluster a vector of times that is defined as
/// \code
/// std::vector<double> timeVector
/// \endcode
/// you could use a very simple metric model
/// \code
/// class TimeMetricModel {
/// public:
///    double operator() (double lhs, double rhs) {
///       return std::abs(lhs-rhs);
///    }
/// }
/// \endcode
/// This will be used in code as follows:
/// \code
/// typedef TTmplDensityCluster<double,TimeMetricModel> TimeCluster;
///
/// TimeCluster timeCluster(4,40*unit::ns);
///
/// timeCluster.Cluster(timeVector);
/// \endcode
/// Notice that the first type of the TTmplDensityCluster template
/// instantiation ("double") matchs the argument types of
/// TimeMetricModel::operator().
///
/// The clustering algorithm can also be used directly with iterators, so that
/// the last example would be:
/// \code
/// typedef TTmplDensityCluster<double,TimeMetricModel> TimeCluster;
///
/// TimeCluster timeCluster(4,40*unit::ns);
///
/// timeCluster.Cluster(timeVector.begin(), timeVector.end());
/// \endcode
///
/// Example: Sometimes you will have a metric model that takes arguments in
/// the constructor.  An example of this might be a clustering where the X, Y
/// and Z dimensions are included in the metric using different scale factors
/// such as 
/// \code
/// class ScaledMetric {
/// private:
///   double fXScale;
///   double fYScale;
///   double fZScale;
/// public:
///    ScaledMetric(double xScale, double yScale, double zScale)
///         : fXScale(xScale), fYScale(yScale), fZScale(zScale) {}
///
///    operator () (const TVector3& lhs, const TVector3& rhs) {
///       double x = fXScale*(lhs.X()-rhs.X());
///       double y = fYScale*(lhs.Y()-rhs.Y());
///       double z = fZScale*(lhs.Z()-rhs.Z());
///       return std::sqrt(x*x + y*y + z*z);
///    }
/// }
/// \endcode
///
/// In this case, the clustering class can be used as follows:
/// \code
/// std::vector<TVector3> positionVector;
///
/// typedef TTmplDensityCluster<TVector3,ScaledMetric> ScaledCluster;
///
/// ScaledCluster ScaledCluster(4,40*unit::cm, ScaledMetric(1.0,0.5,1.0));
///
/// scaledCluster.Cluster(positionVector.begin(), positionVector.end());
/// // or
/// scaledCluster.Cluster(positionVector);
/// \endcode
///
/// Copyright (c) 2008-2015 by Le Phuoc Trung and Clark McGrew
///
/// Usage of the works is permitted provided that this instrument is retained
/// with the works, so that any entity that uses the works is notified of this
/// instrument.
///
/// DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.
///
class TTmplDensityCluster {
public:
    /// A collection of points for use in clustering.  A Points collection is
    /// returned by GetCluster().
    typedef std::list<T> Points;

    /// An iterator to move through the Points collection (mostly for internal
    /// use).
    typedef typename Points::iterator Iterator;

    /// A constant iterator to move through the Points collection (mostly for
    /// internal use).
    typedef typename Points::const_iterator ConstIterator;
    
    /// Create a density clustering class that requires at least minPts within
    /// a distance of maxDist.  The distance is defined by the return value of
    /// the MetricModel::operator() method (for example, TimeDistance or
    /// SpacialDistance described in the class documentation).  The
    /// MetricModel class will be constructed using the default constructor
    /// (e.g. MetricModel())
    explicit TTmplDensityCluster(unsigned int minPts, double maxDist);

    /// Create a density clustering class that requires at least minPts within
    /// a distance of maxDist.  The distance is defined by the return value of
    /// the MetricModel::operator() method (for example, TimeDistance or
    /// SpacialDistance described in the class documentation).  This form
    /// takes an explicitly constructed MetricModel class, so that code can
    /// pass arguments to the MetricModel constructor.
    explicit TTmplDensityCluster(unsigned int minPts, 
                                 double maxDist, 
                                 MetricModel metric);
    virtual ~TTmplDensityCluster() {}

    /// Cluster a vector of objects.  The results are accessed using
    /// GetCluster().
    void Cluster(const std::vector<T>&);
    
    /// Cluster a group of objects between the begin and end iterator.  The
    /// results are accessed using GetCluster().
    template <typename InputIterator>
    void Cluster(InputIterator begin, InputIterator end);

    /// Return the number of clusters found by the density clustering.  This
    /// is only valid after the Cluster() method has been used.
    unsigned int GetClusterCount() { return fClusters.size(); }

    /// Get the i-th cluster.  This is only valid after the cluster method has
    /// been used.  If the index is equal to the number of found clusters,
    /// then the return value will be the list of unclustered points.
    const Points& GetCluster(unsigned int i) const {
        if (i == fClusters.size()) return fRemainingPoints;
        return fClusters.at(i); 
    }
    
    /// A convenient member that returns the points in a cluster.  This is a
    /// relatively expensive method since the points are returned as a vector
    /// (by value).  If the index is greater than the number of found clusters,
    /// then the return value will be the list of unclustered points.
    std::vector<T> GetPoints(unsigned int) const;

    /// Validate that the all of the points are either part of a cluster, or
    /// part of the unclustered points.
    void Check();
     
protected:
    /// Find the set of points with highest density in input.  If the density
    /// is greater that fMinPoints, then return the set of points in output.
    void FindSeeds(const Points& input, Points& output);

    /// Find the neighbors for a point in the input points.  A point is not a
    /// neighbor to itself.  Neighbors are defined as all points for which the
    /// fMetricModel returns a value less than fMaxDist.  Any neighbors
    /// that are found will be returned in output.
    std::size_t GetNeighbors(T point, const Points& input, Points& output);

    /// Count the neighbors for a point in the input points, but do not return
    /// a copy of the neighbors.  The only difference between this and
    /// GetNeighbors is that the neighboring points are not returned by
    /// CountNeighbors.
    std::size_t CountNeighbors(T point, const Points& input);
    
    /// Remove the seeds from the inputs.  This changes the seeds since they
    /// are sorted as part of the algorithm.  The inputs must already be
    /// sorted.
    void RemoveSeeds(Points& inputs, Points& seeds);

private:
    /// The minimum number of points that must be within the fMaxDist
    /// radius of the current point.  If there are at least fMinPoints with in
    /// the fMaxDist of the current point, the cluster will be expanded by
    /// the neighbors.  The distance between points is defined by
    /// fMetricModel.
    unsigned int fMinPoints;

    /// The maximum distance between points for which points are defined as
    /// being neighbors.  The distance between the points is defined by
    /// fMetricModel.
    double fMaxDist;

    /// A class that calculates the distance between points.  The MetricModel
    /// class must define (at least) an method equivalent to "double
    /// operator() (T lhs, T rhs)".  Examples of possible operators are 
    /// \code
    /// double operator() (T lhs, T rhs);
    /// double operator() (const T& lhs, const T& rhs);
    /// \endcode
    MetricModel fMetricModel;

    /// An internal vector of clusters used to cache results.
    std::vector<Points> fClusters;

    /// An internal collection of Points holding the points that have not yet
    /// been added to a cluster.
    Points fRemainingPoints;
};

////////////////////////////////////////////////////////////////
// Define the TTmplDensityCluster class methods.
////////////////////////////////////////////////////////////////

template <typename T, typename MetricModel>
TTmplDensityCluster<T, MetricModel>::TTmplDensityCluster(unsigned int MinPts,
                                                         double maxDist,
                                                         MetricModel metric) :
    fMinPoints(MinPts),
    fMaxDist(maxDist),
    fMetricModel(metric) { }

template <typename T, typename MetricModel>
TTmplDensityCluster<T, MetricModel>::TTmplDensityCluster(unsigned int MinPts,
                                                         double maxDist) :
    fMinPoints(MinPts),
    fMaxDist(maxDist),
    fMetricModel(MetricModel()) { }
  
template <typename T, typename MetricModel>
void TTmplDensityCluster<T, MetricModel>::Cluster(const std::vector<T>& pnts) {
    Cluster(pnts.begin(), pnts.end());
}

template <typename T, typename MetricModel>
template <typename InputIterator>
void TTmplDensityCluster<T, MetricModel>::Cluster(InputIterator begin, 
                                                  InputIterator end) {
    Points seeds;

    // Clear out the  internal data structures.
    fClusters.clear();
    fRemainingPoints.clear();

    // Move the input points into the fRemainingPoints and the sort them so
    // they are in a defined order.
    std::copy(begin, end, std::back_inserter(fRemainingPoints));
    fRemainingPoints.sort();

    // Now continue removing points until there aren't any more points, or a
    // seed isn't found.
    while (!fRemainingPoints.empty()) {
        FindSeeds(fRemainingPoints,seeds);
        if (seeds.size() < fMinPoints) break;

        // Remove the seeds from the remaining points.
        RemoveSeeds(fRemainingPoints,seeds);

        // Add the seeds to the cluster that's being built.
        Points cluster;
        std::copy(seeds.begin(), seeds.end(), std::back_inserter(cluster));

        Points tmp;
        while (!seeds.empty()) {
            T currentP = seeds.front();
            seeds.erase(seeds.begin());
            tmp.clear();
            std::size_t i = GetNeighbors(currentP, fRemainingPoints, tmp);
            i += CountNeighbors(currentP, seeds);
            i += 1;             // Include the current point in the count.
            if (i < fMinPoints) continue;
            std::copy(tmp.begin(),tmp.end(),std::back_inserter(seeds));
            std::copy(tmp.begin(),tmp.end(),std::back_inserter(cluster));
            RemoveSeeds(fRemainingPoints,tmp);
        }

        fClusters.push_back(cluster);
    }

    std::sort(fClusters.begin(), fClusters.end(),
              decreasingClusterSize<Points>());
}
    
template <typename T, typename MetricModel>
void TTmplDensityCluster<T, MetricModel>::Check() {
    typedef typename std::vector<Points>::const_iterator Iterator;
    std::size_t clusterPointCount = 0;
    for (Iterator c = fClusters.begin(); c != fClusters.end(); ++c) {
        std::size_t clusterSize = c->size();
        clusterPointCount += clusterSize;
    }

    std::size_t totalPointCount = clusterPointCount + fRemainingPoints.size();
      
}

    
template <typename T, typename MetricModel>
void TTmplDensityCluster<T, MetricModel>::FindSeeds(
    const Points& in, Points& out) {
    out.clear();
    Points seeds;
    int seedsFound = 0;
    for (ConstIterator h = in.begin(); h != in.end(); ++h) {
        seeds.clear();
        std::size_t i = GetNeighbors(*h, in, seeds); 
        i += 1;                 // Include the current point in the count.
        if (i < fMinPoints) continue;
        if (out.size() < i) { 
            ++seedsFound;       // Count the number of "largest" seeds found.
            out = seeds;        // Copy the points in the seed to output
            out.push_back(*h);  // Add the starting hit to the output.
            if (seedsFound > 5 && i > 3*fMinPoints) break;
        }
    }
    // This returns with the best seed found.
}

template <typename T, typename MetricModel>
std::size_t
TTmplDensityCluster<T, MetricModel>::GetNeighbors(
    T pnt, const Points& in, Points& out) {
    std::size_t size = 0;
    for (ConstIterator h = in.begin(); h != in.end(); ++h) {
        if (pnt == (*h)) continue;
        double distance = fMetricModel(pnt, *h);
        if (distance < fMaxDist) {
            out.push_back(*h);
            ++size;
        }
    }
    return size;
}


template <typename T, typename MetricModel>
std::size_t
TTmplDensityCluster<T, MetricModel>::CountNeighbors(T pnt, const Points& in) {
    std::size_t size = 0;
    for (ConstIterator h = in.begin(); h != in.end(); ++h) {
        if (pnt == (*h)) continue;
        double distance = fMetricModel(pnt, *h);
        if (distance < fMaxDist) {
            ++size;
        }
    }
    return size;
}


template <typename T, typename MetricModel>
void
TTmplDensityCluster<T, MetricModel>::RemoveSeeds(Points& input, Points& seeds) {
    Iterator target = input.begin();
    seeds.sort();
    for (ConstIterator h = seeds.begin(); h != seeds.end(); ++h) {
        while (target!=input.end()) {
            if (*target < *h) ++target;
            else if (*h == *target) {
                target = input.erase(target);
            }
            else break;
        }
    }
}


template <typename T, typename MetricModel>
std::vector<T>
TTmplDensityCluster<T, MetricModel>::GetPoints(unsigned int index) const {
    std::vector<T> points;
    const Points& cluster = GetCluster(index);
    std::copy(cluster.begin(), cluster.end(), std::back_inserter(points));
    return points;
}


#endif
