#include <netcdf>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>


using namespace netCDF;
using namespace netCDF::exceptions;

void printFish() {

         std::cout <<     "\033[31m" <<"    ._      ._      ._      ._             ._    "  << std::endl;
         std::cout <<     "\033[31m" <<"       _.-._)`\_.-._)`\_.-._)`\_.-._)`\_.-._)`\_ "    << "\033[0m"  << std::endl;
         std::cout <<     "\033[31m" <<"                                                 " << "\033[0m"   << std::endl;
         std::cout <<     "\033[31m" <<"           o         _,           _,             " << "\033[0m"   << std::endl;
         std::cout <<     "\033[31m" <<"            o      .' (        .-' /             "  << "\033[0m"   << std::endl;
         std::cout <<     "\033[31m" <<"           o     _/..._'.    .'   /              "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"      (    o .-'`      ` '-./  _.'               "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"       )    ( o)           ;= <_         (       "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"      (      '-.,\\__ __.-;`\   '.        )      "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"       )  )       \) |`\ \)  '.   \      (   (   "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"      (  (           \_/       '-._\      )   )  "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"       )  ) jgs                     `    (   (   "  << "\033[0m" << std::endl;
         std::cout <<     "\033[31m" <<"     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ "  << "\033[0m" << std::endl;
         std::cout <<     "     src : NEMO/src/OCE/nemogem                  "  << std::endl;

}

std::ostream& operator<<(std::ostream& os, const std::vector<NcDim>& dims) {
    os << "[";
    for (size_t i = 0; i < dims.size(); ++i) {
        os << dims[i].getName() << ": " << dims[i].getSize();
        if (i < dims.size() - 1) os << ", ";
    }
    os << "]";
    return os;
}

bool compare_dimensions(const std::vector<NcDim>& dims1, const std::vector<NcDim>& dims2) {
    if (dims1.size() != dims2.size()) {
        return false;
    }
    for (size_t i = 0; i < dims1.size(); ++i) {
        if (dims1[i].getSize() != dims2[i].getSize()) {
            return false;
        }
    }
    return true;
}

bool compare_variable(const std::string& file1, const std::string& file2, const std::string& variable_name, double tolerance = 0) {
    try {
        // Open the NetCDF files
        NcFile dataFile1(file1, NcFile::read);
        NcFile dataFile2(file2, NcFile::read);

        // Get the variable
        NcVar var1 = dataFile1.getVar(variable_name);
        NcVar var2 = dataFile2.getVar(variable_name);

        if (var1.isNull() || var2.isNull()) {
            std::cerr << "Error: Variable " << variable_name << " not found in one or both files." << std::endl;
            return false;
        }

        // Get dimensions
        auto dims1 = var1.getDims();
        auto dims2 = var2.getDims();

        if (!compare_dimensions(dims1, dims2)) {
            std::cerr << "Error: Dimensions of the variable " << variable_name << " do not match." << std::endl;
            std::cerr << "Dims1: " << dims1 << std::endl;
            std::cerr << "Dims2: " << dims2 << std::endl;
            return false;
        }

        // Read data from the variables
        std::vector<size_t> dimSizes;
        for (const auto& dim : dims1) {
            dimSizes.push_back(dim.getSize());
        }

        size_t totalSize = 1;
        for (size_t size : dimSizes) {
            totalSize *= size;
        }

        std::vector<double> data1(totalSize);
        std::vector<double> data2(totalSize);

        var1.getVar(data1.data());
        var2.getVar(data2.data());

        // Compare the data
       //	 std::cerr << "Total Size " << totalSize << std::endl;
        for (size_t i = 0; i < totalSize; ++i) {
            if (std::abs(data1[i] - data2[i]) > tolerance) {
                std::cout << "Variable: " << variable_name << " Index: " << i << " nc1: " << data1[i] << " nc2: " << data2[i] << " diff: " << data1[i] - data2[i] << std::endl;
                return false;
            }
            if(std::abs(data1[i] - data2[i]) > 1e-3)
                std::cout << "Variable: " << variable_name << " Index: " << i << " nc1: " << data1[i] << " nc2: " << data2[i] << " diff: " << data1[i] - data2[i] << std::endl;
        }

        return true;

    } catch (NcException& e) {
        std::cerr << "NetCDF error: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <file1> <file2> <variable_name1> [<variable_name2> ... <variable_name20>]" << std::endl;
        return 1;
    }
    std::string green = "\033[32m";
    std::string reset = "\033[0m";
    std::string file1 = argv[1];
    std::string file2 = argv[2];

    bool all_identical = true;
    
    //printFish();
    for (int i = 3; i < argc; ++i) {
        std::string variable_name = argv[i];
        bool are_identical = compare_variable(file1, file2, variable_name);

        if (are_identical) {
         //   std::cout << "The variable " << variable_name << " is identical in both files." << std::endl;
        } else {
            std::cout << "The variable " << variable_name << " is not identical in both files." << std::endl;
            all_identical = false;
        }
    }

    if (all_identical) {
	    std::cout << green << "All variables are identical" << reset
              << " in " << file1 << " & " << file2 << std::endl;
	    return 1;
    } else {
        std::cout << "Not all variables are identical in both files." << std::endl;
	return 0;
    }

    return 0;
}

