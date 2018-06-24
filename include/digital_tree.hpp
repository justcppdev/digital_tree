#include <cstddef>
#include <iosfwd>
#include <vector>

class digital_tree_t final
{
private:
    using size_t = std::size_t;
    using value_t = int;
    using callback_t = void(*)(value_t);
    using symbol_t = char;
    using string_t = symbol_t const *;
    
    struct node_t final
    {
        symbol_t symbol;
        bool has_value;
        value_t value;
        node_t * link;
        node_t * next;
        
        node_t( symbol_t _symbol ) noexcept;
        ~node_t() = default;
        
        node_t( node_t const & ) = delete;
        node_t & operator =( node_t const & ) = delete;
        node_t( digital_tree_t && ) = delete;
        node_t & operator =( node_t && ) = delete;
    };
    
    static
    bool
    find( node_t const * node, string_t string, size_t length, size_t index, value_t & value ) noexcept;
    
    static
    void
    insert( node_t * & node, string_t string, size_t length, size_t index, value_t value );
    
    static
    bool
    remove( node_t * & node, string_t string, size_t length, size_t index ) noexcept;
    
    static
    void
    traverse( node_t const * node, string_t string, size_t length, size_t index, bool check, callback_t callback );
    
    static
    void
    destroy( node_t const * node ) noexcept;
    
    static
    void
    print( node_t const * node, std::vector<symbol_t> & symbols, std::ostream & stream, bool & first );
private:
    node_t * root_;
public:
    digital_tree_t() noexcept;
    ~digital_tree_t();
    
    digital_tree_t( digital_tree_t const & ) = delete;
    digital_tree_t & operator =( digital_tree_t const & ) = delete;
    digital_tree_t( digital_tree_t && ) = delete;
    digital_tree_t & operator =( digital_tree_t && ) = delete;
    
    bool
    find( string_t string, value_t & value ) const noexcept;
    
    void
    insert( string_t string, value_t value );
    
    bool
    remove( string_t string ) noexcept;
    
    void
    traverse( string_t string, callback_t callback ) const;
public:
    void print( std::ostream & stream ) const;
};

auto operator <<( std::ostream & stream, digital_tree_t const & tree ) -> std::ostream &;
