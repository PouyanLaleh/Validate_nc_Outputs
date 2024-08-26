#First load netcdf and compiler modules

# Compile both programs

mpicxx -I/netcdf_module_path/include -L/-I/netcdf_module_path/lib -lnetcdf -lnetcdf_c++4 list_nc_variables.cpp -o list_nc_variables // provides all variables in a given nc file
mpicxx -I/netcdf_module_path/include -L/netcdf_module_path/lib -lnetcdf -lnetcdf_c++4 compare_nc.cpp -o compare_nc  // compares all variables in two netcdf file

# run check_all_grids.sh PATH1 PATH2

compares all nc files in two given paths that are paths of the reference and the modified versions
