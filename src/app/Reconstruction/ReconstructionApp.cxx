#include "TrackReconstruction.hh"
#include "LightYield.hh"
#include "NeutronAnalysis.hh"
#include "StoppingProton.hh"
#include "SFGD_Reconstruction.hh"

#include <iostream>

/** @brief Method that transforms a const char into int (useful for switch logic) */
constexpr unsigned int
str2int(const char *str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Missing option: trackrecon, lightyield, neutron, stopproton, protomc, sfgdrecon" << std::endl;
        return 1;
    }

    char **subArgv = new char *[argc - 1];
    subArgv[0] = argv[0];
    // Skip 2nd argument which is the main option
    for (int i = 2; i < argc; i++)
    {
        subArgv[i - 1] = argv[i];
    }

    switch (str2int(argv[1]))
    {
    case str2int("trackrecon"):
        TrackReconstruction(argc - 1, subArgv);
        break;
    case str2int("lightyield"):
        LightYield(argc - 1, subArgv);
        break;
    case str2int("neutron"):
        NeutronAnalysis(argc - 1, subArgv);
        break;
    case str2int("stopproton"):
        StoppingProton(argc - 1, subArgv);
        break;
    case str2int("sfgdrecon"):
        SFGD_Reconstruction(argc - 1, subArgv);
        break;
    default:
        std::cerr << "Usage: ReconstructionApp <option> <arguments...>\nOption: trackrecon, lightyield, neutron, stopproton, protomc, sfgdrecon" << std::endl;
        return 0;
    }

    return 1;
}
