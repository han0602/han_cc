file(REMOVE_RECURSE
  "../../../build/lib/libns3.36.1-propagation-default.pdb"
  "../../../build/lib/libns3.36.1-propagation-default.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/libpropagation.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
