#include "catch.hpp"
#include "anndemo/algorithm/lib.hpp"

TEST_CASE( "build system works", "[algorithm]" ) {
    REQUIRE( add(1, 1) == 2 );
}