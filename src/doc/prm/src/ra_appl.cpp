/*! \file ra_appl.cpp
    \brief Example application subclasses of rt_appl
   This file is an example file, containing to example applications.
*/

/** @addtogroup rt */
/*@{*/


/** @defgroup rt_ex Application examples
 *  @ingroup applex
 *  @{
 */
#include "pwr.h"
#include "rt_appl.h"


//! A simple c++ application example
/*! ra_appl is a subclass to rt_appl and implements the virtual
 * functions open(), close(), and scan().
 *
 * @see ra_appl.cpp
 */
class ra_appl : public rt_appl {
public:
 ra_appl() : rt_appl( "ra_appl", errh_eAnix_appl1) {}
 void open();
 void close();
 void scan();
};


void ra_appl::open()
{
 // Link to database objects
}

void ra_appl::close()
{
 // Unlink to database objects
}

void ra_appl::scan()
{
 // Do something
}

int main()
{
 ra_appl appl;

 appl.init();
 appl.register_appl( "Noder-node-MyAppl");

 appl.mainloop();
}

   
/** @} */
/** @} */
