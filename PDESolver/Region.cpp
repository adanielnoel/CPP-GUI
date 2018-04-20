/*
    Created by Alejandro Daniel Noel on 17/05/2017.
*/

#include "Region.h"
#include "LinearAlgebra.h"

namespace PDE {

Region::Region(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {
    cx = x + width / 2;
    cy = y + height / 2;
    resetValues();
}

void Region::subdivide(int divisions_i, int divisions_j) {
    if (isFixed()) return;
    if (shape[0] != 1 or shape[1] != 1) {
        std::cout << "WARNING: Reshaping region" << std::endl;
        // TODO: improve reshaping so that values are taken from the closest Regions and not from the average
        merge();
    }
    if (divisions_i == 1 and divisions_j == 1) merge();
    else if (divisions_i < 1 or divisions_j < 1) throw std::invalid_argument("Invalid division");
    else if (divisions_i > 1 or divisions_j > 1) {
        shape[0] = divisions_i;
        shape[1] = divisions_j;
        // Store in an order such that the elements can be found by j * shape[1] + i. So the outer
        // loop is in the j direction and the inner loop in the x direction
        for (int j = 0; j < divisions_j; j++) {
            for (int i = 0; i < divisions_i; i++) {
                Region newRegion(this->x + i * width / divisions_i,    /* x coordinate */
                                 this->y + j * height / divisions_j,   /* y coordinate */
                                 width / divisions_i,                  /* width dimension */
                                 height / divisions_j);                /* height dimension */
                newRegion.values = this->values;
                newRegion.parent = this;
                children.push_back(newRegion);
            }
        }
    }

}

Region *Region::getRegionWithPoint(double x, double y, int recursiveLevel) {
//    std::cout << recursiveLevel << std::endl;
    // Use of lowestInt() instead of the cast Int() is to handle negatives, since -0.1 should become -1
    int relative_i = lowestInt((x - this->x) * shape[0] / width);
    int relative_j = lowestInt((y - this->y) * shape[1] / height);
    // If one of the children contains the point
    if (relative_i >= 0 and relative_i < shape[0] and relative_j >= 0 and relative_j < shape[1]) {
        if (!children.empty()) {
            return getRegionAtGridPoint(relative_i, relative_j)->getRegionWithPoint(x, y, recursiveLevel + 1);
        } else return this;
    } else if (parent != nullptr && recursiveLevel <= 0) {
        return parent->getRegionWithPoint(x, y, recursiveLevel - 1);
    }
    return nullptr;
}


Region *Region::getRegionAtGridPoint(int i, int j) {
    if (i < shape[0] and j < shape[1]) {
        return &children.at((unsigned) j * shape[0] + i);
    }
    return nullptr;
}

void Region::merge() {
    if (isFixed()) return;
    // Recursive call
    for (auto child : children) child.merge();
    getValues();
    deleteChildren();
}

void Region::resetValues(double setValue) {
    values.clear();
    for (int i = 0; i < numberOfTimestepsToStore; i++) {
        std::vector<double> resetValues;
        for (int j = 0; j < numberOfComponentsPerValue; j++) {
            resetValues.push_back(setValue);
        }
        values.push_back(resetValues);
    }
}

std::deque<std::vector<double>> Region::getValues(bool distanceWeighted) {
    // TODO: Make sure parent `values` has same size as children
    // Condition for end of recursive average
    if (!children.empty()) {
        // Set current Region values to 0
        resetValues();
        double cumulativeInverseLength = 0;
        for (auto child : children) {
            // Recursive average
            child.getValues();
            if (distanceWeighted) {
                double distance = vector_distance(child.getCenter(), getCenter());
                cumulativeInverseLength += 1/distance;
                for (unsigned int t = 0; t < values.size(); t++) {
                    for (unsigned int v = 0; v < values.at(t).size(); v++) {
                        values.at(t).at(v) += child.values.at(t).at(v) / distance;
                    }
                }
            } else {
                for (unsigned int t = 0; t < values.size(); t++) {
                    for (unsigned int v = 0; v < values.at(t).size(); v++) {
                        values.at(t).at(v) += child.values.at(t).at(v) / children.size();
                    }
                }
            }
        }
        if (distanceWeighted) {
            for (unsigned int t = 0; t < values.size(); t++) {
                for (unsigned int v = 0; v < values.at(t).size(); v++) {
                    values.at(t).at(v) /= cumulativeInverseLength;
                }
            }
        }
    }
    return values;
}


bool Region::containsPoint(double x, double y) {
    return (this->x < x and x < this->x + this->width and
            this->y < y and y < this->y + this->height);
}

Region *Region::getNeighbour(int offset_i, int offset_j, Region::NeighbourType neighbourType) {
    // Look for the closest neighbour in the direction (offset_i, offset_j)
    Region *immediateNeighbour = getRegionWithPoint(
            offset_i == 0 ? cx : offset_i < 0 ? x - Region::minimumRegionSize : x + width + Region::minimumRegionSize,
            offset_j == 0 ? cy : offset_j < 0 ? y - Region::minimumRegionSize : y + height + Region::minimumRegionSize);

    if (immediateNeighbour == nullptr) return nullptr;

    // If requested that the neighbour must be of comparable size, recursively look for the right parent
    if (neighbourType == COMPARABLE) {
        immediateNeighbour = immediateNeighbour->getParentOfComparableSize(getSize());
    }

    // If the requested neighbour is in the immediate vicinity, return it
    if (abs(offset_i) <= 1 and abs(offset_j) <= 1) return immediateNeighbour;

    // If the requested neighbour is further away, recursively keep looking for it
    else return immediateNeighbour->getNeighbour(offset_i == 0 ? 0 : offset_i < 0 ? offset_i + 1 : offset_i - 1,
                                                 offset_j == 0 ? 0 : offset_j < 0 ? offset_j + 1 : offset_j - 1,
                                                 neighbourType);
}

Region *Region::getParentOfComparableSize(std::vector<double> size, double comparability) {
    // Check if the difference between the reference size and the current Region's
    // size is within the comparability margin with respect to the reference size
    if (fabs(width - size[0]) < size[0] * comparability or
        fabs(height - size[1]) < size[1] * comparability) {
        return this;
    }

    // If the Region is still too small, ask the parent recursively
    return parent->getParentOfComparableSize(size, comparability);
}

Region::~Region() {
    deleteChildren();
}

void Region::deleteChildren() {
    // Delete all children. Note that the destructor calls deleteChildren() on the child
    // being deleted, therefore effectively making the deletion recursive.
    children.clear();

    // Update shape
    shape[0] = 1;
    shape[1] = 1;
}

void Region::pushTempValOnChildlessRegions() {
    auto childlessRegions = getChildlessRegions();
    for (auto &childlessChild : childlessRegions) {
        childlessChild->values.push_back(childlessChild->tempVal);
        // Maintain the maximum size of the deque
        if (childlessChild->values.size() > numberOfTimestepsToStore) childlessChild->values.pop_front();
    }
}

void Region::_getChildlessRegions(std::vector<Region*> &childlessRegions) {
    for (Region &child : children) {
        if (child.isChildless()) childlessRegions.push_back(&child);
        else child._getChildlessRegions(childlessRegions);
    }
}

bool Region::isFixed() {
    if (fixed) return true;
    for (auto child : children) return child.isFixed();
    return false;
}


} /*Namespace PDE*/


