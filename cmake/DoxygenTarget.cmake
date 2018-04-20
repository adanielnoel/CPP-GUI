function(PrepareDocTarget)
    # add a target to generate API documentation with Doxygen
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.ini ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile.ini @ONLY)
        add_custom_target(doc ALL
                          ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile.ini
                          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR/doc}
                          COMMENT "Generating API documentation with Doxygen" VERBATIM
                          )
        set(INSTALL_DOC_DIR ${CMAKE_BINARY_DIR}/doc/html)
        file(MAKE_DIRECTORY ${INSTALL_DOC_DIR}) # needed for install

        install(DIRECTORY ${INSTALL_DOC_DIR} DESTINATION share/${PROJECT_NAME}-${VERSION_MAJOR} COMPONENT doc)
    endif(DOXYGEN_FOUND)
endfunction()
