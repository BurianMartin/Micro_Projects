#include <string>
#include <iostream>

#define DEBUG

#ifdef DEBUG
#define err_log(x)      \
    {                   \
        std::cerr << x; \
    }

#define debug_log(x)    \
    {                   \
        std::cerr << x; \
    }
#else
#define err_log(x)
#define debug_log(x)
#endif

#define std_log(x)      \
    {                   \
        std::cout << x; \
    }
