/**
 * \file GeoidEval.cpp
 * \brief Command line utility for evaluation geoid heights
 *
 * Copyright (c) Charles Karney (2009, 2010, 2011) <charles@karney.com>
 * and licensed under the MIT/X11 License.  For more information, see
 * http://geographiclib.sourceforge.net/
 *
 * Compile with -I../include and link with Geoid.o DMS.o
 *
 * See the <a href="GeoidEval.1.html">man page</a> for usage
 * information.
 **********************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <GeographicLib/Geoid.hpp>
#include <GeographicLib/DMS.hpp>
#include <GeographicLib/GeoCoords.hpp>

#include "GeoidEval.usage"

int main(int argc, char* argv[]) {
  try {
    using namespace GeographicLib;
    typedef Math::real real;
    bool cacheall = false, cachearea = false, verbose = false, cubic = true;
    real caches, cachew, cachen, cachee;
    std::string dir;
    std::string geoid = Geoid::DefaultGeoidName();
    std::string zone;
    Geoid::convertflag heightmult = Geoid::NONE;
    std::string istring, ifile, ofile;

    for (int m = 1; m < argc; ++m) {
      std::string arg(argv[m]);
      if (arg == "-a") {
        cacheall = true;
        cachearea = false;
      }
      else if (arg == "-c") {
        if (m + 4 >= argc) return usage(1, true);
        cacheall = false;
        cachearea = true;
        try {
          DMS::DecodeLatLon(std::string(argv[m + 1]), std::string(argv[m + 2]),
                            caches, cachew);
          DMS::DecodeLatLon(std::string(argv[m + 3]), std::string(argv[m + 4]),
                            cachen, cachee);
        }
        catch (const std::exception& e) {
          std::cerr << "Error decoding argument of -c: " << e.what() << "\n";
          return 1;
        }
        m += 4;
      } else if (arg == "--msltohae" || arg == "-msltohae")
        heightmult = Geoid::GEOIDTOELLIPSOID;
      else if (arg == "--haetomsl" || arg == "-haetomsl")
        heightmult = Geoid::ELLIPSOIDTOGEOID;
      else if (arg == "-z") {
        if (++m == argc) return usage(1, true);
        zone = argv[m];
        bool northp;
        int zonenum;
        try {
          UTMUPS::DecodeZone(zone, zonenum, northp);
          zone += ' ';
        }
        catch (const std::exception& e) {
          std::cerr << "Error decoding zone: " << e.what() << "\n";
          return 1;
        }
      } else if (arg == "-n") {
        if (++m == argc) return usage(1, true);
        geoid = argv[m];
      } else if (arg == "-d") {
        if (++m == argc) return usage(1, true);
        dir = argv[m];
      } else if (arg == "-l") {
        cubic = false;
      } else if (arg == "-v")
        verbose = true;
      else if (arg == "--input-string") {
        if (++m == argc) return usage(1, true);
        istring = argv[m];
      } else if (arg == "--input-file") {
        if (++m == argc) return usage(1, true);
        ifile = argv[m];
      } else if (arg == "--output-file") {
        if (++m == argc) return usage(1, true);
        ofile = argv[m];
      } else if (arg == "--version") {
        std::cout
          << argv[0]
          << ": $Id: fbae1ec93734f13474d4ea5149cd272904df0e77 $\n"
          << "GeographicLib version " << GEOGRAPHICLIB_VERSION_STRING << "\n";
        return 0;
      } else {
        int retval = usage(!(arg == "-h" || arg == "--help"), arg != "--help");
        if (arg == "-h")
          std::cout<< "\nDefault geoid path = \""   << Geoid::DefaultGeoidPath()
                   << "\"\nDefault geoid name = \"" << Geoid::DefaultGeoidName()
                   << "\"\n";
        return retval;
      }
    }

    if (!ifile.empty() && !istring.empty()) {
      std::cerr << "Cannot specify --input-string and --input-file together\n";
      return 1;
    }
    if (ifile == "-") ifile.clear();
    std::ifstream infile;
    std::istringstream instring;
    if (!ifile.empty()) {
      infile.open(ifile.c_str());
      if (!infile.is_open()) {
        std::cerr << "Cannot open " << ifile << " for reading\n";
        return 1;
      }
    } else if (!istring.empty()) {
      std::string::size_type m = 0;
      while (true) {
        m = istring.find(';', m);
        if (m == std::string::npos)
          break;
        istring[m] = '\n';
      }
      instring.str(istring);
    }
    std::istream* input = !ifile.empty() ? &infile :
      (!istring.empty() ? &instring : &std::cin);

    std::ofstream outfile;
    if (ofile == "-") ofile.clear();
    if (!ofile.empty()) {
      outfile.open(ofile.c_str());
      if (!outfile.is_open()) {
        std::cerr << "Cannot open " << ofile << " for wrting\n";
        return 1;
      }
    }
    std::ostream* output = !ofile.empty() ? &outfile : &std::cout;

    int retval = 0;
    try {
      const Geoid g(geoid, dir, cubic);
      try {
        if (cacheall)
          g.CacheAll();
        else if (cachearea)
          g.CacheArea(caches, cachew, cachen, cachee);
      }
      catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\nProceeding without a cache\n";
      }
      if (verbose) {
        std::cerr << "Geoid file: "    << g.GeoidFile()     << "\n"
                  << "Description: "   << g.Description()   << "\n"
                  << "Interpolation: " << g.Interpolation() << "\n"
                  << "Date & Time: "   << g.DateTime()      << "\n"
                  << "Offset (m): "    << g.Offset()        << "\n"
                  << "Scale (m): "     << g.Scale()         << "\n"
                  << "Max error (m): " << g.MaxError()      << "\n"
                  << "RMS error (m): " << g.RMSError()      << "\n";
        if (g.Cache())
          std::cerr<< "Caching:"
                   << "\n SW Corner: " << g.CacheSouth() << " " << g.CacheWest()
                   << "\n NE Corner: " << g.CacheNorth() << " " << g.CacheEast()
                   << "\n";
      }

      GeoCoords p;
      std::string s;
      const char* spaces = " \t\n\v\f\r,"; // Include comma as space
      while (std::getline(*input, s)) {
        try {
          real height = 0;
          if (heightmult) {
            std::string::size_type pb = s.find_last_not_of(spaces);
            std::string::size_type pa = s.find_last_of(spaces, pb);
            std::string::size_type px = s.find_last_not_of(spaces, pa);
            if (pa == std::string::npos || pb == std::string::npos ||
                px == std::string::npos)
              throw GeographicErr("Incomplete input: " + s);
            height = DMS::Decode(s.substr(pa + 1, pb - pa));
            s = s.substr(0, px + 1);
          }
          p.Reset(zone + s);
          if (heightmult) {
            real h = g(p.Latitude(), p.Longitude());
            *output << s << " "
                    << DMS::Encode(height + real(heightmult) * h,
                                   3, DMS::NUMBER) << "\n";
          } else {
            real gradn, grade;
            real h = g(p.Latitude(), p.Longitude(), gradn, grade);
            *output << DMS::Encode(h, 4, DMS::NUMBER) << " "
                    << DMS::Encode(gradn * 1e6, 2, DMS::NUMBER)
                    << (Math::isnan(gradn) ? " " : "e-6 ")
                    << DMS::Encode(grade * 1e6, 2, DMS::NUMBER)
                    << (Math::isnan(grade) ? "\n" : "e-6\n");
          }
        }
        catch (const std::exception& e) {
          *output << "ERROR: " << e.what() << "\n";
          retval = 1;
        }
      }
    }
    catch (const std::exception& e) {
      std::cerr << "Error reading " << geoid << ": " << e.what() << "\n";
      retval = 1;
    }
    return retval;
  }
  catch (const std::exception& e) {
    std::cerr << "Caught exception: " << e.what() << "\n";
    return 1;
  }
  catch (...) {
    std::cerr << "Caught unknown exception\n";
    return 1;
  }
}
