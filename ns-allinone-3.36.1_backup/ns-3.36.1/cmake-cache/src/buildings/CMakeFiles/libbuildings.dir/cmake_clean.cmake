file(REMOVE_RECURSE
  "../../../build/lib/libns3.36.1-buildings-default.pdb"
  "../../../build/lib/libns3.36.1-buildings-default.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/libbuildings.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
