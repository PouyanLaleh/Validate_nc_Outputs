#include <netcdf>
#include <iostream>
#include <string>
#include <vector>

using namespace netCDF;
using namespace netCDF::exceptions;

void list_nc_variables(const std::string& filename) {
    try {
        // Open the NetCDF file
        NcFile dataFile(filename, NcFile::read);

        // Print the names of all variables
        std::cout << "Variables in the file:" << std::endl;
        for (const auto& var_name : dataFile.getVars()) {
            std::cout << var_name.first << std::endl;
        }

        // Print the details of each variable
        for (const auto& var_name : dataFile.getVars()) {
            NcVar var = dataFile.getVar(var_name.first);
            std::cout << "\nVariable: " << var_name.first << std::endl;
            std::cout << "  Dimensions: ";
            for (const auto& dim : var.getDims()) {
                std::cout << dim.getName() << " ";
            }
            std::cout << std::endl;
            std::cout << "  Shape: ";
            for (const auto& dim : var.getDims()) {
                std::cout << dim.getSize() << " ";
            }
            std::cout << std::endl;
            std::cout << "  Data type: " << var.getType().getName() << std::endl;

            std::cout << "  Attributes: " << std::endl;
            for (const auto& attr_name : var.getAtts()) {
                NcVarAtt att = var.getAtt(attr_name.first);
                std::cout << "    " << attr_name.first << ": ";

                // Determine the type of the attribute
                switch (att.getType().getId()) {
                    case NC_CHAR: {
                        // Read character attributes
                        std::vector<char> values(100);  // Assuming a max size of 100; adjust as needed
                        att.getValues(values.data());
                        std::string str(values.begin(), values.end());
                        std::cout << "\"" << str << "\"";
                        break;
                    }
                    case NC_BYTE:
                    case NC_SHORT:
                    case NC_INT:
                    case NC_UBYTE:
                    case NC_USHORT:
                    case NC_UINT:
                    case NC_INT64:
                    case NC_UINT64: {
                        // Read integer attributes
                        std::vector<int> values(100);  // Assuming a max size of 100; adjust as needed
                        att.getValues(reinterpret_cast<int*>(values.data()));
                        for (const auto& val : values)
                            std::cout << val << " ";
                        break;
                    }
                    case NC_FLOAT: {
                        // Read float attributes
                        std::vector<float> values(100);  // Assuming a max size of 100; adjust as needed
                        att.getValues(values.data());
                        for (const auto& val : values)
                            std::cout << val << " ";
                        break;
                    }
                    case NC_DOUBLE: {
                        // Read double attributes
                        std::vector<double> values(100);  // Assuming a max size of 100; adjust as needed
                        att.getValues(values.data());
                        for (const auto& val : values)
                            std::cout << val << " ";
                        break;
                    }
                    default:
                        std::cout << "(unsupported type)";
                }
                std::cout << std::endl;
            }
        }
    } catch (NcException& e) {
        std::cerr << "NetCDF error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <netcdf_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    list_nc_variables(filename);

    return 0;
}

