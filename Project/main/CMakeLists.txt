# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCDIRS ". ../Hal/Source/")

set(COMPONENT_ADD_INCLUDEDIRS ". ../Hal/Include/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHARDWARE_TESTER=1")

register_component()

# Create a SPIFFS image from the contents of the 'spiffs_image' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash'. 
#spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)