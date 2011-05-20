/**
 * \file mgrsreverse.cpp
 * \brief Matlab mex file for UTM/UPS to MGRS conversions
 *
 * Copyright (c) Charles Karney (2010) <charles@karney.com> and licensed under
 * the LGPL.  For more information, see http://geographiclib.sourceforge.net/
 **********************************************************************/

// Compile in Matlab with
// [Unix]
// mex -I/usr/local/include -L/usr/local/lib -Wl,-rpath=/usr/local/lib -lGeographic mgrsreverse.cpp
// [Windows]
// mex -I../include -L../windows/Release -lGeographic mgrsreverse.cpp

// "$Id: mgrsreverse.cpp 6906 2010-12-02 22:10:56Z karney $";

#include "GeographicLib/MGRS.hpp"
#include "mex.h"

using namespace std;
using namespace GeographicLib;

void mexFunction( int nlhs, mxArray* plhs[],
                  int nrhs, const mxArray* prhs[] ) {

  if (nrhs != 1)
    mexErrMsgTxt("One input argument required.");
  else if (nlhs > 1)
    mexErrMsgTxt("Only one output argument can be specified.");

  if (!mxIsCell(prhs[0]) || mxGetN(prhs[0]) != 1)
    mexErrMsgTxt("mgrs coordinates should be in a M x 1 cell array.");

  int m = mxGetM(prhs[0]);
  plhs[0] = mxCreateDoubleMatrix(m, 5, mxREAL);

  double* x = mxGetPr(plhs[0]);
  double* y = x + m;
  double* zone = x + 2*m;
  double* hemi = x + 3*m;
  double* prec = x + 4*m;

  string mgrsstr;
  for (int i = 0; i < m; ++i) {
    try {
      mxArray* mgrs = mxGetCell(prhs[0], i);
      if (!mxIsChar(mgrs) || mxGetM(mgrs) != 1)
        throw GeographicErr("Cell element not a string");
      int n = mxGetN(mgrs);
      mxChar* mgrschar = mxGetChars(mgrs);
      mgrsstr.resize(n);
      for (int k = 0; k < n; ++k)
        mgrsstr[k] = mgrschar[k];
      int ZONE, PREC;
      bool HEMI;
      MGRS::Reverse(mgrsstr, ZONE, HEMI, x[i], y[i], PREC);
      zone[i] = ZONE;
      hemi[i] = HEMI ? 1 : 0;
      prec[i] = PREC;
    }
    catch (const std::exception& e) {
      mexWarnMsgTxt(e.what());
      x[i] = y[i] = Math::NaN();
      zone[i] = UTMUPS::INVALID; hemi[i] = 0; prec[i] = -1;
    }
  }
}