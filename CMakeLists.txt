project(PDFHUMMUS)
cmake_minimum_required (VERSION 2.6)
add_definitions(-DFONT_fILE="${PROJECT_SOURCE_DIR}/Materials/fonts/arial.ttf")
add_definitions(-DMATERIALS_DIR="${PROJECT_SOURCE_DIR}/Materials/")

if(NOT NO_DCT)
	ADD_SUBDIRECTORY(LibJpeg)
endif(NOT NO_DCT)
ADD_SUBDIRECTORY(Zlib)
if(NOT NO_TIFF)
	ADD_SUBDIRECTORY(LibTiff)
endif(NOT NO_TIFF)
ADD_SUBDIRECTORY(FreeType)
ADD_SUBDIRECTORY(PDFWriter)
ADD_SUBDIRECTORY(PDFWriterTestPlayground)


#add_dependencies(test PDFWriter) #add_dependencies makes sure that dependencies are built before main target


include_directories (${PDFWriter_SOURCE_DIR}) 
add_executable( test executable.cpp)

target_link_libraries (test PDFWriter)
target_link_libraries (test  FreeType)
target_link_libraries (test LibJpeg )
target_link_libraries (test Zlib )
target_link_libraries (test LibTiff )


