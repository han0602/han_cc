file(REMOVE_RECURSE
  "../../../build/lib/libns3.36.1-mmwave-default.pdb"
  "../../../build/lib/libns3.36.1-mmwave-default.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/libmmwave.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
