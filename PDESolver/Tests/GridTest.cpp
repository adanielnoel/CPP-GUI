/*
    Created by Alejandro Daniel Noel on 17/05/2017.
*/

#define CATCH_CONFIG_MAIN
#include "Catch/catch.hpp"
#include "Region.h"

TEST_CASE("Get Region containing point", "[grid]"){
    PDE::Region baseRegion(2.0, 1.0);
    baseRegion.subdivide(4, 2);
    baseRegion.getChildren().at(7).subdivide(5, 12);
//    baseRegion.merge();
//    REQUIRE((baseRegion.getRegionAtGridPoint(0, 0)->getOrigin()[0] == 0 and
//             baseRegion.getRegionAtGridPoint(0, 0)->getOrigin()[1] == 0));
    double x = 1.9, y = 0.7;
    PDE::Region *containerRegion = baseRegion.getChildren().at(1).getRegionWithPoint(x, y);
    REQUIRE(containerRegion->containsPoint(x, y));
}

TEST_CASE("Get immediate neighbour", "[grid]") {
    PDE::Region baseRegion(3.5, 1.6);
    baseRegion.subdivide(10, 14);
    PDE::Region *aChild = baseRegion.getRegionAtGridPoint(5, 7);
    REQUIRE(aChild->getNeighbour(-1, 0)  == baseRegion.getRegionAtGridPoint(4, 7));
    REQUIRE(aChild->getNeighbour(-1, -1) == baseRegion.getRegionAtGridPoint(4, 6));
    REQUIRE(aChild->getNeighbour(-1, 1)  == baseRegion.getRegionAtGridPoint(4, 8));
    REQUIRE(aChild->getNeighbour(0, -1)  == baseRegion.getRegionAtGridPoint(5, 6));
    REQUIRE(aChild->getNeighbour(0, 1)   == baseRegion.getRegionAtGridPoint(5, 8));
    REQUIRE(aChild->getNeighbour(1, 0)   == baseRegion.getRegionAtGridPoint(6, 7));
    REQUIRE(aChild->getNeighbour(1, -1)  == baseRegion.getRegionAtGridPoint(6, 6));
    REQUIRE(aChild->getNeighbour(1, 1)   == baseRegion.getRegionAtGridPoint(6, 8));
}

TEST_CASE("Get non-immediate neighbour", "[grid]") {
    PDE::Region baseRegion(3.5, 1.6);
    baseRegion.subdivide(10, 14);
    PDE::Region *aChild = baseRegion.getRegionAtGridPoint(5, 7);
    REQUIRE(aChild->getNeighbour(-2, 0) == baseRegion.getRegionAtGridPoint(3, 7));
    REQUIRE(aChild->getNeighbour(-2, 1) == baseRegion.getRegionAtGridPoint(3, 8));
    REQUIRE(aChild->getNeighbour(-2, 2) == baseRegion.getRegionAtGridPoint(3, 9));
    REQUIRE(aChild->getNeighbour(-2, 3) == baseRegion.getRegionAtGridPoint(3, 10));
    REQUIRE(aChild->getNeighbour(2, 0)  == baseRegion.getRegionAtGridPoint(7, 7));
    REQUIRE(aChild->getNeighbour(2, 1)  == baseRegion.getRegionAtGridPoint(7, 8));
    REQUIRE(aChild->getNeighbour(2, 2)  == baseRegion.getRegionAtGridPoint(7, 9));
    REQUIRE(aChild->getNeighbour(2, 3)  == baseRegion.getRegionAtGridPoint(7, 10));
    REQUIRE(aChild->getNeighbour(-5, 3) == baseRegion.getRegionAtGridPoint(0, 10));
    REQUIRE(aChild->getNeighbour(2, 6)  == baseRegion.getRegionAtGridPoint(7, 13));
    REQUIRE(aChild->getNeighbour(2, -7) == baseRegion.getRegionAtGridPoint(7, 0));
    REQUIRE(aChild->getNeighbour(4, 3)  == baseRegion.getRegionAtGridPoint(9, 10));
}

TEST_CASE("Ask for non-existing neighbour", "[grid]") {
    PDE::Region baseRegion(3.5, 1.6);
    baseRegion.subdivide(10, 14);
    PDE::Region *aChild = baseRegion.getRegionAtGridPoint(5, 7);
    REQUIRE(aChild->getNeighbour(-6, 0) == nullptr);
    REQUIRE(aChild->getNeighbour(0, 7) == nullptr);
    REQUIRE(aChild->getNeighbour(-6, -100) == nullptr);
    REQUIRE(aChild->getNeighbour(-10, 100) == nullptr);
}

TEST_CASE("Get neighbours in adaptive mesh", "[grid]") {
    PDE::Region baseRegion(3.5, 1.6);
    baseRegion.subdivide(10, 14);
    baseRegion.getRegionAtGridPoint(4, 7)->subdivide(10, 9);
    PDE::Region *aChild = baseRegion.getRegionAtGridPoint(5, 7);
    REQUIRE(aChild->getNeighbour(-1, 0, PDE::Region::COMPARABLE) == baseRegion.getRegionAtGridPoint(4, 7));
    REQUIRE(aChild->getNeighbour(-1, 0, PDE::Region::NEAREST) == baseRegion.getRegionAtGridPoint(4, 7)->getRegionAtGridPoint(9, 4));
}

TEST_CASE("Average", "[grid]") {
    PDE::Region baseRegion(3, 2);
    baseRegion.subdivide(3, 2);
    baseRegion.getRegionAtGridPoint(0, 0)->tempVal = {10.0};
    baseRegion.getRegionAtGridPoint(1, 0)->tempVal = {36.0};
    baseRegion.getRegionAtGridPoint(2, 0)->tempVal = {75.0};
    baseRegion.getRegionAtGridPoint(0, 1)->tempVal = {12.0};
    baseRegion.getRegionAtGridPoint(1, 1)->tempVal = {4.0};
    baseRegion.getRegionAtGridPoint(2, 1)->tempVal = {600.0};
    baseRegion.pushTempValOnChildlessRegions();
    REQUIRE(std::abs(baseRegion.getValues(false).back()[0] - 122.83333333333) < 0.000000001); // Average unweighted
    REQUIRE(std::abs(baseRegion.getValues(true).back()[0] - 92.8269710586) < 0.000000001);    // Average weighted by distance from center
}

TEST_CASE("Management of the childlessRegions variable", "[grid]") {
    PDE::Region baseRegion(3, 2);
    baseRegion.subdivide(3, 2);
    baseRegion.getRegionAtGridPoint(1, 1)->subdivide(10, 20);
    baseRegion.getRegionAtGridPoint(1, 1)->getRegionAtGridPoint(1, 1)->subdivide(5, 4);
    std::vector<PDE::Region*> a = baseRegion.getChildlessRegions();
    REQUIRE(baseRegion.getChildlessRegions().size() == 224);
}