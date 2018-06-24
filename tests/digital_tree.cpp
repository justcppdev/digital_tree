#include <catch.hpp>
#include <sstream>
#include <string>

#include "digital_tree.hpp"

std::string representation( digital_tree_t const & tree )
{
    std::ostringstream stream;
    stream << tree;
    return stream.str();
}

// a -> b -> c(1) -> e(2)
// |    |
// v    v
// c(4) d(3)
// |
// v
// b(5)
TEST_CASE("inserting")
{
    digital_tree_t tree;
    tree.insert("abc", 1);
    REQUIRE( representation( tree ) == "abc1" );
    
    tree.insert("abce", 2);
    REQUIRE( representation( tree ) == "abc1\nabce2" );
    
    tree.insert("abd", 3);
    REQUIRE( representation( tree ) == "abc1\nabce2\nabd3" );
    
    tree.insert("c", 4);
    REQUIRE( representation( tree ) == "abc1\nabce2\nabd3\nc4" );
    
    tree.insert("b", 5);
    REQUIRE( representation( tree ) == "abc1\nabce2\nabd3\nc4\nb5" );
}

// a -> b -> c(1) -> e(2)
// |    |
// v    v
// c(4) d(3)
// |
// v
// b(5)
TEST_CASE("searching")
{
    digital_tree_t tree;
    tree.insert("abc", 1);
    tree.insert("abce", 2);
    tree.insert("abd", 3);
    tree.insert("c", 4);
    tree.insert("b", 5);
    
    int value;
    REQUIRE( tree.find( "abc" , value ) );
    REQUIRE( value == 1 );
    REQUIRE( tree.find( "abce" , value ) );
    REQUIRE( value == 2 );
    REQUIRE( tree.find( "abd" , value ) );
    REQUIRE( value == 3 );
    REQUIRE( tree.find( "c" , value ) );
    REQUIRE( value == 4 );
    REQUIRE( tree.find( "b" , value ) );
    REQUIRE( value == 5 );
    REQUIRE( !tree.find( "d" , value ) );
}

// a -> b -> c(1) -> e(2)
// |    |
// v    v
// c(4) d(3)
// |
// v
// b(5)
TEST_CASE("removing")
{
    digital_tree_t tree;
    tree.insert("abc", 1);
    tree.insert("abce", 2);
    tree.insert("abd", 3);
    tree.insert("c", 4);
    tree.insert("b", 5);
    REQUIRE( representation( tree ) == "abc1\nabce2\nabd3\nc4\nb5" );
    
    REQUIRE( tree.remove( "abc" ) );
    REQUIRE( representation( tree ) == "abce2\nabd3\nc4\nb5" );
    
    REQUIRE( tree.remove( "abd" ) );
    REQUIRE( representation( tree ) == "abce2\nc4\nb5" );
    
    REQUIRE( tree.remove( "c" ) );
    REQUIRE( representation( tree ) == "abce2\nb5" );
    
    REQUIRE( tree.remove( "b" ) );
    REQUIRE( representation( tree ) == "abce2" );
    
    REQUIRE( tree.remove( "abce" ) );
    REQUIRE( representation( tree ) == "" );
    
    REQUIRE( !tree.remove( "foo" ) );
}

// a -> b -> c(1) -> e(2)
// |    |
// v    v
// c(4) d(3)
// |
// v
// b(5)
std::vector<int> values;

TEST_CASE("traversing")
{
    digital_tree_t tree;
    tree.insert("abc", 1);
    tree.insert("abce", 2);
    tree.insert("abd", 3);
    tree.insert("c", 4);
    tree.insert("b", 5);
    
    tree.traverse( "abc" , []( int value ) {
        values.push_back( value );
    } );
    REQUIRE( values == std::vector<int>( {2, 3, 4, 5} ) );
    
    values.clear();
    tree.traverse( "abce" , []( int value ) {
        values.push_back( value );
    } );
    REQUIRE( values == std::vector<int>( {3, 4, 5} ) );
    
    values.clear();
    tree.traverse( "abd" , []( int value ) {
        values.push_back( value );
    } );
    REQUIRE( values == std::vector<int>( {4, 5} ) );
    
    values.clear();
    tree.traverse( "c" , []( int value ) {
        values.push_back( value );
    } );
    REQUIRE( values == std::vector<int>( {} ) );
    
    values.clear();
    tree.traverse( "b" , []( int value ) {
        values.push_back( value );
    } );
    REQUIRE( values == std::vector<int>( {4} ) );
}

