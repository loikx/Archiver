add_executable(${PROJECT_NAME} main.cpp ../lib/huffman_code/huffman.cpp ../lib/huffman_code/huffman.h ../lib/archiver/archiver.cpp ../lib/archiver/archiver.h)

target_link_libraries(${PROJECT_NAME} PRIVATE hamming)
target_link_libraries(${PROJECT_NAME} PRIVATE huffman)
target_link_libraries(${PROJECT_NAME} PRIVATE archiver)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})