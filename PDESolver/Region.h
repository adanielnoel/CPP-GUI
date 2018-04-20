/*
    Created by Alejandro Daniel Noel on 17/05/2017.
*/

#ifndef SIMULATOR_REGION_H
#define SIMULATOR_REGION_H


#include <iostream>
#include <vector>
#include <deque>


namespace PDE {

class Region {
public:
    /**
     * Defines the minimum size of a Region in length. While regions may be rectangular
     * none of the sides must be shorter than this. It is mainly used to get the neighbour
     * next to the current Region. If the neighbour was smallest than this, the algorithm
     * could skip it.
     */
    static constexpr double minimumRegionSize = 0.00001;

    static constexpr short numberOfTimestepsToStore = 5;
    static constexpr short numberOfComponentsPerValue = 1;

    /**
     * If set to true the region can't be split or merged.
     */
    bool fixed = false;

    /**
     * Returns the regions that have no children, that is, those that are at the very ends
     * of the Region tree branches. These are the only ones that require an update on their
     * values, since the parents are automatically updated when calling {@link getValues()}.
     */
    inline std::vector<Region*> getChildlessRegions() {
        std::vector<Region*> result = {};
        // The private method of the same name gets the return vector by reference, thus avoiding
        // the creation of a new vector on each recursive call.
        _getChildlessRegions(result);
        return result;
    }


    /**
     * Defines different ways to treat neighbours.
     */
    enum NeighbourType {
        NEAREST,          ///> Used to get the nearest and smallest bounding region.
        COMPARABLE,       ///> Used to get the nearest region of size comparable to the current @see Region#getParentOfComparableSize()
    };

    /**
     * Holds the value being inserted in the current iteration. When the update of the full
     * grid is done, make sure to call {@link pushTempVal()}.
     */
    std::vector<double> tempVal;

    /**
     * Request a push of the {@link tempVal} into the private member {@link values} on all
     * the regions returned by {@link getChildlessRegions()}. This function must be called
     * at the root node after all the {@link tempVal} have been set so that the value is
     * saved for the next iterations. For the rest of values, the value is updated whenever
     * getValues() is called.
     */
    void pushTempValOnChildlessRegions();

    /**
     * Main constructor. Note that x and y are at the origin corner of the region, while the
     * values are actually calculated at its center. @see Region#getCenter()
     */
    Region(double x, double y, double width, double height);

    Region(double width, double height) : Region(0, 0, width, height) {};

    /**
     * Recursively deletes children.
     */
    ~Region();

    /**
     * Subdivides the Region into smaller regions. This sets the number of sub-regions to
     * \p divisions_i * \p divisions_j.
     * @param divisions_i divisions in the `i` direction.
     * @param divisions_j divisions in the `j` direction.
     * @param newShape the new layout of the Region. Default value is {2, 2}, that is, each
     *                 dimensional length is divided by 2 and the region is now composed of
     *                 4 smaller regions.
     */
    void subdivide(int divisions_i = 2, int divisions_j = 2);

    /**
     * Calls the {@link average()} and {@link deleteChildren()} methods recursively.
     */
    void merge();

    /**
     * Returns the value of the region. If it is subdivided into children regions it computes
     * the average of them.
     * Note: this function is the only that updates the contents of the {@link values} private
     * variable in the case that the Region has children.
     * @param distanceWeighted specifies if the average should be weighted with the distance of
     * the children's centers to the center of the current Region. Defaults to `true`.
     * @return the average of the values of the children regions in every available time step.
     *         The deque is a FIFO container for the time steps and the vector contains the
     *         actual values.
     */
    std::deque<std::vector<double>> getValues(bool distanceWeighted = true);

    /**
     * Deallocate children pointers recursively.
     */
    void deleteChildren();

    /**
     * Finds the smallest region that contains the point. If the region does not contain
     * the point the function automatically calls the method for the parent, if any.
     * If the coordinate coincides with the boundary of some regions, the region returned
     * is always that for which the coordinate is in the boundary with smaller coordinates,
     * that is, lower and left margins in a standard oriented cartesian coordinate system.
     * @param x coordinate of the point in global space.
     * @param y coordinate of the point in global space.
     * @param recursiveLevel variable used internally to determine how deep
     *                       in the recursive chain the current call is.
     *                       Not to be set by the user.
     * @return Pointer to the smallest Region object containing the point. Returns NULL if
     *         the point is not in the domain.
     */
    Region *getRegionWithPoint(double x, double y, int recursiveLevel = 0);

    /**
     * Finds the child Region that contains the point. Note that \p i and \p j are defined
     * in the current Region and hold no relation to those of other regions.
     * @param i coordinate of the point in grid space.
     * @param j coordinate of the point in grid space.
     * @return Pointer to the child Region object containing the point.
     */
    Region *getRegionAtGridPoint(int i, int j);

    /**
     * Finds the region that lies at the grid coordinates specified. Note that if the
     * coordinates are outside the parent Region an offset in world coordinates of size
     * \p offset_i * width and \p offset_j * height is used in combination with the function
     * {@link getRegionOfPoint()}.
     * @param offset_i
     * @param offset_j
     * @return
     */
    Region *getNeighbour(int offset_i, int offset_j, NeighbourType neighbourType = COMPARABLE);

    /**
     * @return lower-left corner of the Region
     */
    inline std::vector<double> getOrigin() { return {x, y}; };

    /**
     * @return center of the Region
     */
    inline std::vector<double> getCenter() { return {cx, cy}; };

    inline std::vector<double> getSize() { return {width, height};};

    /**
     * Sets all values to a default.
     * @param setValue Value to use. Default is 0.
     */
    void resetValues(double setValue = 0);

    /**
     * @return Vector of pointers to the children Region objects. Note that the vector may
     *         be empty if the Region has no children.
     */
    inline std::vector<Region> &getChildren() { return children; };

    inline bool isChildless() { return children.empty();}

    /**
     *
     * @param referenceRegion
     * @param size to compare with
     * @param comparability Coefficient that sets the allowed difference in sizes to bass
     *        the test of comparability. Defaults to 0.1 (10%).
     * @return
     */
    Region *getParentOfComparableSize(std::vector<double> size, double comparability = 0.1);

    bool containsPoint(double x, double y);

    bool isFixed();

private:
    double x = 0, y = 0, width, height, cx, cy;

    int shape[2] = {1, 1};

    /**
     * Reference to the children regions. A nullptr pointer indicates the Region is not subdivided.
     */
    std::vector<Region> children;
    Region *parent = nullptr;


    /**
     * The outer container of type queue stores current and previous solutions in a FIFO order.
     * The inner container contains the solutions of individual time steps. User may use it to
     * store single values as well as components of vectors, etc. Of course, knowledge on the
     * order follower when storing the values is required.
     * This variable is only useful when the shape of the Region is [1, 1], that is, it has no
     * children regions. If you want to know the average value of the region call the function
     * {@link average()}.
     */
    std::deque<std::vector<double> > values;

    void _getChildlessRegions(std::vector<Region*> &childlessRegions);

};

} /*Namespace PDE*/


#endif //SIMULATOR_REGION_H
