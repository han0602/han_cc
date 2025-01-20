file(REMOVE_RECURSE
  "../../../build/lib/libns3.36.1-network-default.pdb"
  "../../../build/lib/libns3.36.1-network-default.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/libnetwork.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
