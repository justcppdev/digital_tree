#include <cassert>
#include <cstring>
#include <iostream>

#include "digital_tree.hpp"

digital_tree_t::node_t::
node_t( symbol_t _symbol ) noexcept
: symbol{ _symbol }
, has_value{ false }
, value{ 0 }
, link{ nullptr }
, next{ nullptr }
{}

bool
digital_tree_t::
find( node_t const * node, string_t string, size_t length, size_t index, value_t & value ) noexcept
{
    assert( string && index < length );
    
    if( !node ) {
        return false;
    }
    
    if( node->symbol == string[ index ] ) {
        if( index == length - 1 ) {
            if( node->has_value ) {
                value = node->value;
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return find( node->link, string, length, index + 1, value );
        }
    }
    else {
        return find( node->next, string, length, index, value );
    }
}

bool
digital_tree_t::
remove( node_t * & node, string_t string, size_t length, size_t index ) noexcept
{
    assert( string && index < length );
    
    if( !node ) {
        return false;
    }
    
    if( node->symbol == string[ index ] ) {
        if( index == length - 1 ) {
            if( node->has_value ) {
                if( node->link ) {
                    node->has_value = false;
                }
                else {
                    auto next = node->next;
                    delete node;
                    node = next;
                }
                
                return true;
            }
            else {
                return false;
            }
        }
        else {
            bool removed = remove( node->link, string, length, index + 1 );
            if( removed && !node->link ) {
                auto next = node->next;
                delete node;
                node = next;
            }
            
            return removed;
        }
    }
    else {
        return remove( node->next, string, length, index );
    }
}

void
digital_tree_t::
insert( node_t * & node, string_t string, size_t length, size_t index, value_t value )
{
    assert( string && index < length );
    
    if( !node ) {
        node = new node_t( string[ index ] );
    }
    
    if( node->symbol == string[ index ] ) {
        if( index == length - 1 ) {
            node->has_value = true;
            node->value = value;
        }
        else {
            insert( node->link, string, length, index + 1, value );
        }
    }
    else {
        insert( node->next, string, length, index, value );
    }
}

void
digital_tree_t::
traverse( node_t const * node, string_t string, size_t length, size_t index, bool need_check, callback_t callback )
{
    assert( string );
    
    if( !node ) {
        return;
    }
    
    // >
    bool need_check_for_link = need_check;
    if( !need_check || index > length - 1 || node->symbol > string[ index ] ) {
        callback( node->value );
        need_check_for_link = false;
    }
    
    // >=
    if( !need_check_for_link || index > length - 1 || node->symbol >= string[ index ] ) {
        traverse( node->link, string, length, index + 1, need_check_for_link, callback );
    }
    
    traverse( node->next, string, length, index, need_check, callback );
}

digital_tree_t::
digital_tree_t() noexcept
: root_{ nullptr }
{}

digital_tree_t::
~digital_tree_t()
{
    destroy( root_ );
}

bool
digital_tree_t::
find( string_t string, value_t & value ) const noexcept
{
    if( string && strlen( string ) != 0 ) {
        return find( root_, string, strlen( string ), 0, value );
    }
    
    return false;
}

void
digital_tree_t::
insert( string_t string, value_t value )
{
    if( string && strlen( string ) != 0 ) {
        insert( root_ , string, strlen( string ), 0, value );
    }
}

bool
digital_tree_t::
remove( string_t string ) noexcept
{
    if( string && strlen( string ) != 0 ) {
        return remove( root_, string, strlen( string ), 0);
    }
    
    return false;
}

void
digital_tree_t::
traverse( string_t string, callback_t callback ) const
{
    if( string ) {
        traverse( root_, string, strlen( string ), 0, true, callback );
    }
}

void
digital_tree_t::
destroy( node_t const * node ) noexcept
{
    if( node ) {
        destroy( node->link );
        destroy( node->next );
        delete node;
    }
}

void
digital_tree_t::
print( node_t const * node, std::vector<symbol_t> & symbols, std::ostream & stream, bool & first )
{
    if( !node ) {
        return;
    }
    
    symbols.push_back( node->symbol );
    if( node->has_value ) {
        if ( first ) {
            first = false;
        }
        else {
            stream << '\n';
        }
        
        for( auto symbol : symbols ) {
            stream << symbol;
        }
        stream << node->value;
    }
    
    print( node->link , symbols, stream, first );
    
    symbols.pop_back();
    print( node->next , symbols, stream, first );
}

void
digital_tree_t::
print( std::ostream & stream ) const
{
    std::vector<symbol_t> symbols;
    bool first = true;
    print( root_, symbols, stream, first );
}

auto operator <<( std::ostream & stream, digital_tree_t const & tree ) -> std::ostream &
{
    tree.print( stream );
    
    return stream;
}
