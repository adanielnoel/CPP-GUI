
function(list_assert_duplicates list_id)

    # message(STATUS "list data: ${list_id}")

    list(LENGTH list_id _len_before)
    list(REMOVE_DUPLICATES list_id)
    list(LENGTH list_id _len_after)
    # message(STATUS "list size ${_len_before} -> ${_len_after}")
    if(NOT _len_before EQUAL _len_after)
        message(FATAL_ERROR "duplicate found in list which should not contain duplicates: ${list_id}")
    endif()
    unset(_len_before)
    unset(_len_after)
endfunction(list_assert_duplicates)

function(custom_add_library name sources includes)
    include_directories("${includes}")
    add_library(${name} ${sources})

    list_assert_duplicates("${sources}")
    list_assert_duplicates("${includes}")
    message(STATUS "Configured library ${name}")
endfunction(custom_add_library)