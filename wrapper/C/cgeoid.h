#if !defined(CGEOID_H)
#define CGEOID_H 1

/* See https://isocpp.org/wiki/faq/mixing-c-and-cpp */

#if defined(__cplusplus)
extern "C"
#endif
double HeightAboveEllipsoid(double lat, double lon, double h);

#endif  /* CGEOID_H */
