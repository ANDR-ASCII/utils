cmake_minimum_required(VERSION 2.8.11)

project(utils)

set(SourcesDir src)
set(HeadersDir include)

set(AllHeadersDir
	${SourcesDir} 
	${HeadersDir}
)

aux_source_directory(${SourcesDir} SourcesList)
file(GLOB_RECURSE HeadersList "include/*.h")

include_directories(${AllHeadersDir})

add_executable(
	utils
	${SourcesList}
	${HeadersList}
)

target_link_libraries(utils)