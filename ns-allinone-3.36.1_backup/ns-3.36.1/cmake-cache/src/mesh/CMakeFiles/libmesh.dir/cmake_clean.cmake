file(REMOVE_RECURSE
  "../../../build/lib/libns3.36.1-mesh-default.pdb"
  "../../../build/lib/libns3.36.1-mesh-default.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/libmesh.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
