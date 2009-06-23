#ifndef GGL_PROJECTIONS_BIPC_HPP
#define GGL_PROJECTIONS_BIPC_HPP

// Generic Geometry Library - projections (based on PROJ4)
// This file is automatically generated. DO NOT EDIT.

// Copyright Barend Gehrels (1995-2009), Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande (2008-2009)
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is converted from PROJ4, http://trac.osgeo.org/proj
// PROJ4 is originally written by Gerald Evenden (then of the USGS)
// PROJ4 is maintained by Frank Warmerdam
// PROJ4 is converted to Geometry Library by Barend Gehrels (Geodan, Amsterdam)

// Original copyright notice:
 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include <boost/math/special_functions/hypot.hpp>

#include <ggl/projections/impl/base_static.hpp>
#include <ggl/projections/impl/base_dynamic.hpp>
#include <ggl/projections/impl/projects.hpp>
#include <ggl/projections/impl/factory_entry.hpp>

namespace ggl { namespace projection
{
    #ifndef DOXYGEN_NO_IMPL
    namespace impl { namespace bipc{ 
            static const double EPS = 1e-10;
            static const double EPS10 = 1e-10;
            static const double ONEEPS = 1.000000001;
            static const int NITER = 10;
            static const double lamB = -.34894976726250681539;
            static const double n = .63055844881274687180;
            static const double F = 1.89724742567461030582;
            static const double Azab = .81650043674686363166;
            static const double Azba = 1.82261843856185925133;
            static const double T = 1.27246578267089012270;
            static const double rhoc = 1.20709121521568721927;
            static const double cAzc = .69691523038678375519;
            static const double sAzc = .71715351331143607555;
            static const double C45 = .70710678118654752469;
            static const double S45 = .70710678118654752410;
            static const double C20 = .93969262078590838411;
            static const double S20 = -.34202014332566873287;
            static const double R110 = 1.91986217719376253360;
            static const double R104 = 1.81514242207410275904;

            struct par_bipc
            {
                int noskew;
            };

            // template class, using CRTP to implement forward/inverse
            template <typename Geographic, typename Cartesian, typename Parameters>
            struct base_bipc_spheroid : public base_t_fi<base_bipc_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>
            {

                 typedef double geographic_type;
                 typedef double cartesian_type;

                par_bipc m_proj_parm;

                inline base_bipc_spheroid(const Parameters& par)
                    : base_t_fi<base_bipc_spheroid<Geographic, Cartesian, Parameters>,
                     Geographic, Cartesian, Parameters>(*this, par) {}

                inline void fwd(geographic_type& lp_lon, geographic_type& lp_lat, cartesian_type& xy_x, cartesian_type& xy_y) const
                {
                	double cphi, sphi, tphi, t, al, Az, z, Av, cdlam, sdlam, r;
                	int tag;
                
                	cphi = cos(lp_lat);
                	sphi = sin(lp_lat);
                	cdlam = cos(sdlam = lamB - lp_lon);
                	sdlam = sin(sdlam);
                	if (fabs(fabs(lp_lat) - HALFPI) < EPS10) {
                		Az = lp_lat < 0. ? PI : 0.;
                		tphi = HUGE_VAL;
                	} else {
                		tphi = sphi / cphi;
                		Az = atan2(sdlam , C45 * (tphi - cdlam));
                	}
                	if( (tag = (Az > Azba)) ) {
                		cdlam = cos(sdlam = lp_lon + R110);
                		sdlam = sin(sdlam);
                		z = S20 * sphi + C20 * cphi * cdlam;
                		if (fabs(z) > 1.) {
                			if (fabs(z) > ONEEPS) throw proj_exception();
                			else z = z < 0. ? -1. : 1.;
                		} else
                			z = acos(z);
                		if (tphi != HUGE_VAL)
                			Az = atan2(sdlam, (C20 * tphi - S20 * cdlam));
                		Av = Azab;
                		xy_y = rhoc;
                	} else {
                		z = S45 * (sphi + cphi * cdlam);
                		if (fabs(z) > 1.) {
                			if (fabs(z) > ONEEPS) throw proj_exception();
                			else z = z < 0. ? -1. : 1.;
                		} else
                			z = acos(z);
                		Av = Azba;
                		xy_y = -rhoc;
                	}
                	if (z < 0.) throw proj_exception();;
                	r = F * (t = pow(tan(.5 * z), n));
                	if ((al = .5 * (R104 - z)) < 0.) throw proj_exception();;
                	al = (t + pow(al, n)) / T;
                	if (fabs(al) > 1.) {
                		if (fabs(al) > ONEEPS) throw proj_exception();
                		else al = al < 0. ? -1. : 1.;
                	} else
                		al = acos(al);
                	if (fabs(t = n * (Av - Az)) < al)
                		r /= cos(al + (tag ? t : -t));
                	xy_x = r * sin(t);
                	xy_y += (tag ? -r : r) * cos(t);
                	if (this->m_proj_parm.noskew) {
                		t = xy_x;
                		xy_x = -xy_x * cAzc - xy_y * sAzc; 
                		xy_y = -xy_y * cAzc + t * sAzc; 
                	}
                }

                inline void inv(cartesian_type& xy_x, cartesian_type& xy_y, geographic_type& lp_lon, geographic_type& lp_lat) const
                {
                	double t, r, rp, rl, al, z, fAz, Az, s, c, Av;
                	int neg, i;
                
                	if (this->m_proj_parm.noskew) {
                		t = xy_x;
                		xy_x = -xy_x * cAzc + xy_y * sAzc; 
                		xy_y = -xy_y * cAzc - t * sAzc; 
                	}
                	if( (neg = (xy_x < 0.)) ) {
                		xy_y = rhoc - xy_y;
                		s = S20;
                		c = C20;
                		Av = Azab;
                	} else {
                		xy_y += rhoc;
                		s = S45;
                		c = C45;
                		Av = Azba;
                	}
                	rl = rp = r = boost::math::hypot(xy_x, xy_y);
                	fAz = fabs(Az = atan2(xy_x, xy_y));
                	for (i = NITER; i ; --i) {
                		z = 2. * atan(pow(r / F,1 / n));
                		al = acos((pow(tan(.5 * z), n) +
                		   pow(tan(.5 * (R104 - z)), n)) / T);
                		if (fAz < al)
                			r = rp * cos(al + (neg ? Az : -Az));
                		if (fabs(rl - r) < EPS)
                			break;
                		rl = r;
                	}
                	if (! i) throw proj_exception();;
                	Az = Av - Az / n;
                	lp_lat = asin(s * cos(z) + c * sin(z) * cos(Az));
                	lp_lon = atan2(sin(Az), c / tan(z) - s * cos(Az));
                	if (neg)
                		lp_lon -= R110;
                	else
                		lp_lon = lamB - lp_lon;
                }
            };

            // Bipolar conic of western hemisphere
            template <typename Parameters>
            void setup_bipc(Parameters& par, par_bipc& proj_parm)
            {
            	proj_parm.noskew = pj_param(par.params, "bns").i;
                // par.inv = s_inverse;
                // par.fwd = s_forward;
            	par.es = 0.;
            }

        }} // namespace impl::bipc
    #endif // doxygen 

    /*!
        \brief Bipolar conic of western hemisphere projection
        \ingroup projections
        \tparam Geographic latlong point type
        \tparam Cartesian xy point type
        \tparam Parameters parameter type
        \par Projection characteristics
         - Conic
         - Spheroid
        \par Example
        \image html ex_bipc.gif
    */
    template <typename Geographic, typename Cartesian, typename Parameters = parameters>
    struct bipc_spheroid : public impl::bipc::base_bipc_spheroid<Geographic, Cartesian, Parameters>
    {
        inline bipc_spheroid(const Parameters& par) : impl::bipc::base_bipc_spheroid<Geographic, Cartesian, Parameters>(par)
        {
            impl::bipc::setup_bipc(this->m_par, this->m_proj_parm);
        }
    };

    #ifndef DOXYGEN_NO_IMPL
    namespace impl
    {

        // Factory entry(s)
        template <typename Geographic, typename Cartesian, typename Parameters>
        class bipc_entry : public impl::factory_entry<Geographic, Cartesian, Parameters>
        {
            public :
                virtual projection<Geographic, Cartesian>* create_new(const Parameters& par) const
                {
                    return new base_v_fi<bipc_spheroid<Geographic, Cartesian, Parameters>, Geographic, Cartesian, Parameters>(par);
                }
        };

        template <typename Geographic, typename Cartesian, typename Parameters>
        inline void bipc_init(impl::base_factory<Geographic, Cartesian, Parameters>& factory)
        {
            factory.add_to_factory("bipc", new bipc_entry<Geographic, Cartesian, Parameters>);
        }

    } // namespace impl 
    #endif // doxygen

}} // namespace ggl::projection

#endif // GGL_PROJECTIONS_BIPC_HPP
