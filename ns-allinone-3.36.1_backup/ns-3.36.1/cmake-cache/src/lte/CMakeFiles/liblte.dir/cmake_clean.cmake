file(REMOVE_RECURSE
  "../../../build/lib/libns3.36.1-lte-default.pdb"
  "../../../build/lib/libns3.36.1-lte-default.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/liblte.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
