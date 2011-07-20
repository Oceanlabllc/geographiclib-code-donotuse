function [area, perimeter] = polygonarea(latlong, a, f)
%polygonarea  Compute area of a geodesic polygon
%
%   [area, perimeter] = polygonarea(latlong)
%   [area, perimeter] = polygonarea(latlong, a, f)
%
%   latlong is an M x 2 matrix
%       latitude of vertices = latlong(:,1) in degrees
%       longitude of vertices = latlong(:,2) in degrees
%
%   area is the area in meters^2
%   perimeter is the perimeter in meters
%
%   a = major radius (meters)
%   f = flattening (0 means a sphere)
%   If a and f are omitted, the WGS84 values are used.
%
% Only simple polygons (which do not intersect themselves) are supported.
% There is no need to "close" the polygon. Counter-clockwise traversal
% counts as a positive area.  A polygon may encircle one or both poles.
% The total area of the WGS84 ellipsoid is given by
%   8 * polygonarea([ 0 0; 0 90; 90 0 ])
%
% This is an interface to the GeographicLib C++ class
%     Geodesic::PolygonArea
% See the documentation on this function for more information:
% http://geographiclib.sf.net/html/classGeographicLib_1_1PolygonArea
  error('Error: executing .m file instead of compiled routine');
end
% polygonarea.m
% Matlab .m file for finding polygon areas
%
% Copyright (c) Charles Karney (2011) <charles@karney.com> and
% licensed under the MIT/X11 License.  For more information, see
% http://geographiclib.sourceforge.net/
%
% $Id: 6e5fb482c0c2c1d543c110f912fc42888877906e $
