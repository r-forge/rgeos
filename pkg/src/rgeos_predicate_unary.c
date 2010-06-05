#include "rgeos.h"

SEXP rgeos_isvalid(SEXP env, SEXP spgeom, SEXP byid) {
    return( rgeos_unarypredfunc(env,spgeom,byid, &GEOSisValid_r) );
}

SEXP rgeos_issimple(SEXP env, SEXP spgeom, SEXP byid) {
    return( rgeos_unarypredfunc(env,spgeom,byid, &GEOSisSimple_r) );
}

SEXP rgeos_isring(SEXP env, SEXP spgeom, SEXP byid) {
    return( rgeos_unarypredfunc(env,spgeom,byid, &GEOSisRing_r) );
}

SEXP rgeos_hasz(SEXP env, SEXP spgeom, SEXP byid) {
    return( rgeos_unarypredfunc(env,spgeom,byid, &GEOSHasZ_r) );
}

SEXP rgeos_isempty(SEXP env, SEXP spgeom, SEXP byid) {
    return( rgeos_unarypredfunc(env,spgeom,byid, &GEOSisEmpty_r) );
}


SEXP rgeos_unarypredfunc(SEXP env, SEXP spgeom, SEXP byid, 
                       char (*unarypredfunc)(GEOSContextHandle_t, const GEOSGeom)) {

    SEXP ans;
    GEOSGeom geom, curgeom;
    
    int val;
    int i, n, pc=0;
    int type;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);
    
    geom = rgeos_convert_R2geos(env, spgeom);
    type = GEOSGeomTypeId_r(GEOShandle, geom);

    n = 1;
    if (LOGICAL_POINTER(byid)[0] && type == GEOS_GEOMETRYCOLLECTION)
        n = GEOSGetNumGeometries_r(GEOShandle, geom);

    if (n == -1) error("rgeos_unarypredfunc: invalid number of subgeometries");
    
    PROTECT(ans = NEW_LOGICAL(n)); pc++;

    curgeom = geom;
    for(i=0; i<n; i++) {
        if ( n > 1) {
            curgeom = (GEOSGeom) GEOSGetGeometryN_r(GEOShandle, geom, i);
            if (curgeom == NULL) 
                error("rgeos_unarypredfunc: unable to get subgeometries");
        }

        val = (int) unarypredfunc(GEOShandle, curgeom);
        if (val == 2)
            error("rgeos_unarypredfunc: test failed");

        LOGICAL_POINTER(ans)[i] = val;
    }

    GEOSGeom_destroy_r(GEOShandle, geom);
    
    UNPROTECT(pc);
    return(ans);
}


SEXP rgeos_isvalidreason(SEXP env, SEXP spgeom, SEXP byid) {
    SEXP ans;
    GEOSGeom geom, curgeom;
    
    int val;
    int i, n, pc=0;
    int type;
    char *buf;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);
    
    geom = rgeos_convert_R2geos(env, spgeom);
    type = GEOSGeomTypeId_r(GEOShandle, geom);

    n = 1;
    if (LOGICAL_POINTER(byid)[0] && type == GEOS_GEOMETRYCOLLECTION)
        n = GEOSGetNumGeometries_r(GEOShandle, geom);

    if (n == -1) error("rgeos_isvalidreason: invalid number of subgeometries");
    
    PROTECT(ans = NEW_CHARACTER(n)); pc++;

    curgeom = geom;
    for(i=0; i<n; i++) {
        if ( n > 1) {
            curgeom = (GEOSGeom) GEOSGetGeometryN_r(GEOShandle, geom, i);
            if (curgeom == NULL) 
                error("rgeos_isvalidreason: unable to get subgeometries");
        }

        buf = (char *) GEOSisValidReason_r(GEOShandle, curgeom);
        if (buf[0] == 2)
            error("rgeos_isvalidreason: test failed");

        SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(buf));
        
        GEOSFree_r(GEOShandle, buf);
    }

    GEOSGeom_destroy_r(GEOShandle, geom);
    
    UNPROTECT(pc);
    return(ans);
}