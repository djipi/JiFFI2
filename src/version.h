#pragma

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Version information
#define MAJOR   1		// Major version number
#define MINOR   0		// Minor version number
#define PATCH   5		// Patch release number

#define VERSION_STRING STR(MAJOR) "." STR(MINOR) "." STR(PATCH)