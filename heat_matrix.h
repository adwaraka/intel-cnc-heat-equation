#ifndef heat_matrix_H_ALREADY_INCLUDED
#define heat_matrix_H_ALREADY_INCLUDED

#include <cnc/cnc.h>
#include <cnc/debug.h>

// Forward declaration of the context class (also known as graph)
struct heat_matrix_context;

// The step classes

struct compute_heat
{
    int execute( const MatrixKey & t, heat_matrix_context & c ) const;
};

// The context class
struct heat_matrix_context : public CnC::context< heat_matrix_context >
{
    // Item collections
    CnC::item_collection< MatrixKey, float > matrix_value;

    // Tag collections
    CnC::tag_collection< MatrixKey > position;

    // The context class constructor
#pragma warning(push)
#pragma warning(disable: 4355)
    heat_matrix_context()
        : CnC::context< heat_matrix_context >(),
          // Initialize each item collection
          matrix_value( this ),
          // Initialize each tag collection
          position( this, false )
    {
        // Prescriptive relations
        prescribe( position, compute_heat() );
    }
#pragma warning(pop)
};

#endif // heat_matrix_H_ALREADY_INCLUDED
