/**
 * \file LocalCartesian.hpp
 * \brief Header for GeographicLib::LocalCartesian class
 *
 * Copyright (c) Charles Karney (2008, 2009, 2010, 2011) <charles@karney.com>
 * and licensed under the MIT/X11 License.  For more information, see
 * http://geographiclib.sourceforge.net/
 **********************************************************************/

#if !defined(GEOGRAPHICLIB_LOCALCARTESIAN_HPP)
#define GEOGRAPHICLIB_LOCALCARTESIAN_HPP "$Id: 245e250778ab49feb229bcb178d720e56994f30e $"

#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/Constants.hpp>

namespace GeographicLib {

  /**
   * \brief Local Cartesian coordinates
   *
   * Convert between geodetic coordinates latitude = \e lat, longitude = \e
   * lon, height = \e h (measured vertically from the surface of the ellipsoid)
   * to local cartesian coordinates (\e x, \e y, \e z).  The origin of local
   * cartesian coordinate system is at \e lat = \e lat0, \e lon = \e lon0, \e h
   * = \e h0. The \e z axis is normal to the ellipsoid; the \e y axis points
   * due north.  The plane \e z = - \e h0 is tangent to the ellipsoid.
   *
   * The conversions all take place via geocentric coordinates using a
   * Geocentric object (by default Geocentric::WGS84).
   **********************************************************************/

  class GEOGRAPHIC_EXPORT LocalCartesian {
  private:
    typedef Math::real real;
    static const size_t dim_ = 3;
    static const size_t dim2_ = dim_ * dim_;
    const Geocentric _earth;
    real _lat0, _lon0, _h0;
    real _x0, _y0, _z0, _r[dim2_];
    void IntForward(real lat, real lon, real h, real& x, real& y, real& z,
                    real M[dim2_]) const throw();
    void IntReverse(real x, real y, real z, real& lat, real& lon, real& h,
                    real M[dim2_]) const throw();
    void MatrixMultiply(real M[dim2_]) const throw();
  public:

    /**
     * Constructor setting the origin.
     *
     * @param[in] lat0 latitude at origin (degrees).
     * @param[in] lon0 longitude at origin (degrees).
     * @param[in] h0 height above ellipsoid at origin (meters); default 0.
     * @param[in] earth Geocentric object for the transformation; default
     *   Geocentric::WGS84.
     **********************************************************************/
    LocalCartesian(real lat0, real lon0, real h0 = 0,
                   const Geocentric& earth = Geocentric::WGS84) throw()
      : _earth(earth)
    { Reset(lat0, lon0, h0); }

    /**
     * Default constructor.
     *
     * @param[in] earth Geocentric object for the transformation; default
     *   Geocentric::WGS84.
     *
     * Sets \e lat0 = 0, \e lon0 = 0, \e h0 = 0.
     **********************************************************************/
    explicit LocalCartesian(const Geocentric& earth = Geocentric::WGS84)
      throw()
      : _earth(earth)
    { Reset(real(0), real(0), real(0)); }

    /**
     * Reset the origin.
     *
     * @param[in] lat0 latitude at origin (degrees).
     * @param[in] lon0 longitude at origin (degrees).
     * @param[in] h0 height above ellipsoid at origin (meters); default 0.
     **********************************************************************/
    void Reset(real lat0, real lon0, real h0 = 0)
      throw();

    /**
     * Convert from geodetic to local cartesian coordinates.
     *
     * @param[in] lat latitude of point (degrees).
     * @param[in] lon longitude of point (degrees).
     * @param[in] h height of point above the ellipsoid (meters).
     * @param[out] x local cartesian coordinate (meters).
     * @param[out] y local cartesian coordinate (meters).
     * @param[out] z local cartesian coordinate (meters).
     *
     * \e lat should be in the range [-90, 90]; \e lon and \e lon0 should be in
     * the range [-180, 360].
     **********************************************************************/
    void Forward(real lat, real lon, real h, real& x, real& y, real& z)
      const throw() {
      IntForward(lat, lon, h, x, y, z, NULL);
    }

    /**
     * Convert from geodetic to local cartesian coordinates and return rotation
     * matrix.
     *
     * @param[in] lat latitude of point (degrees).
     * @param[in] lon longitude of point (degrees).
     * @param[in] h height of point above the ellipsoid (meters).
     * @param[out] x local cartesian coordinate (meters).
     * @param[out] y local cartesian coordinate (meters).
     * @param[out] z local cartesian coordinate (meters).
     * @param[out] M if the length of the vector is 9, fill with the rotation
     *   matrix in row-major order.
     *
     * Pre-multiplying a unit vector in local cartesian coordinates at (lat,
     * lon, h) by \e M transforms the vector to local cartesian coordinates at
     * (lat0, lon0, h0).
     **********************************************************************/
    void Forward(real lat, real lon, real h, real& x, real& y, real& z,
                 std::vector<real>& M)
      const throw()  {
      if (M.end() == M.begin() + dim2_) {
        real t[dim2_];
        IntForward(lat, lon, h, x, y, z, t);
        copy(t, t + dim2_, M.begin());
      } else
        IntForward(lat, lon, h, x, y, z, NULL);
    }

    /**
     * Convert from local cartesian to geodetic coordinates.
     *
     * @param[in] x local cartesian coordinate (meters).
     * @param[in] y local cartesian coordinate (meters).
     * @param[in] z local cartesian coordinate (meters).
     * @param[out] lat latitude of point (degrees).
     * @param[out] lon longitude of point (degrees).
     * @param[out] h height of point above the ellipsoid (meters).
     *
     * The value of \e lon returned is in the range [-180, 180).
     **********************************************************************/
    void Reverse(real x, real y, real z, real& lat, real& lon, real& h)
      const throw() {
      IntReverse(x, y, z, lat, lon, h, NULL);
    }

    /**
     * Convert from local cartesian to geodetic coordinates and return rotation
     * matrix.
     *
     * @param[in] x local cartesian coordinate (meters).
     * @param[in] y local cartesian coordinate (meters).
     * @param[in] z local cartesian coordinate (meters).
     * @param[out] lat latitude of point (degrees).
     * @param[out] lon longitude of point (degrees).
     * @param[out] h height of point above the ellipsoid (meters).
     * @param[out] M if the length of the vector is 9, fill with the rotation
     *   matrix in row-major order.
     *
     * Pre-multiplying a unit vector in local cartesian coordinates at (lat0,
     * lon0, h0) by the transpose of \e M transforms the vector to local
     * cartesian coordinates at (lat, lon, h).
     **********************************************************************/
    void Reverse(real x, real y, real z, real& lat, real& lon, real& h,
                 std::vector<real>& M)
      const throw() {
      if (M.end() == M.begin() + dim2_) {
        real t[dim2_];
        IntReverse(x, y, z, lat, lon, h, t);
        copy(t, t + dim2_, M.begin());
      } else
        IntReverse(x, y, z, lat, lon, h, NULL);
    }

    /** \name Inspector functions
     **********************************************************************/
    ///@{
    /**
     * @return latitude of the origin (degrees).
     **********************************************************************/
    Math::real LatitudeOrigin() const throw() { return _lat0; }

    /**
     * @return longitude of the origin (degrees).
     **********************************************************************/
    Math::real LongitudeOrigin() const throw() { return _lon0; }

    /**
     * @return height of the origin (meters).
     **********************************************************************/
    Math::real HeightOrigin() const throw() { return _h0; }

    /**
     * @return \e a the equatorial radius of the ellipsoid (meters).  This is
     *   the value of \e a inherited from the Geocentric object used in the
     *   constructor.
     **********************************************************************/
    Math::real MajorRadius() const throw() { return _earth.MajorRadius(); }

    /**
     * @return \e f the flattening of the ellipsoid.  This is the value
     *   inherited from the Geocentric object used in the constructor.
     **********************************************************************/
    Math::real Flattening() const throw()
    { return _earth.Flattening(); }

    /**
     * <b>DEPRECATED</b>
     * @return \e r the inverse flattening of the ellipsoid.
     **********************************************************************/
    Math::real InverseFlattening() const throw()
    { return _earth.InverseFlattening(); }
    ///@}
  };

} // namespace GeographicLib

#endif  // GEOGRAPHICLIB_LOCALCARTESIAN_HPP
