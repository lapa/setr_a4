# Set the source files that make up the library
set(SENSORS_SRCS
   common_funcs.h
   co2_levels.h co2_levels.c
   humidity.h humidity.c
   temperature.h temperature.c
)

# Set the library type (can be static or dynamic)
SET (LIB_TYPE STATIC)

# Create the library
add_library (SensorsLib ${LIB_TYPE} ${SENSORS_SRCS})
