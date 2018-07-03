cmake_minimum_required(VERSION 3.1)
project( ShapeRecognition )
set(EXECUTABLE_OUTPUT_PATH "../bin")

find_package( OpenCV REQUIRED )
add_executable( ShapeRecognition  main.cpp )
target_link_libraries( ShapeRecognition  ${OpenCV_LIBS} )