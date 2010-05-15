#include "rgeos.h"

SEXP IntersectGpcGEOS(SEXP env, SEXP A, SEXP B) {

    GEOSGeom GCA, GCB, GCC;
    SEXP res;
    int pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    GCA = GCPPtsGC(env, A);
    GCB = GCPPtsGC(env, B);

    if ((GCC = GEOSIntersection_r(GEOShandle, GCA, GCB)) == NULL) {
        GEOSGeom_destroy_r(GEOShandle, GCA);
        GEOSGeom_destroy_r(GEOShandle, GCB);
        error("IntersectGpcGEOS: intersection not created");
    }

    GEOSGeom_destroy_r(GEOShandle, GCA);
    GEOSGeom_destroy_r(GEOShandle, GCB);
    PROTECT(res = GCGCPPts(env, GCC)); pc++;
    GEOSGeom_destroy_r(GEOShandle, GCC);
    UNPROTECT(pc);
    return(res);

}

SEXP UnionGpcGEOS(SEXP env, SEXP A, SEXP B) {

    GEOSGeom GCA, GCB, GCC;
    SEXP res;
    int pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    GCA = GCPPtsGC(env, A);
    GCB = GCPPtsGC(env, B);

    if ((GCC = GEOSUnion_r(GEOShandle, GCA, GCB)) == NULL) {
        GEOSGeom_destroy_r(GEOShandle, GCA);
        GEOSGeom_destroy_r(GEOShandle, GCB);
        error("UnionGpcGEOS: union not created");
    }

    GEOSGeom_destroy_r(GEOShandle, GCA);
    GEOSGeom_destroy_r(GEOShandle, GCB);
    PROTECT(res = GCGCPPts(env, GCC)); pc++;
    GEOSGeom_destroy_r(GEOShandle, GCC);
    UNPROTECT(pc);
    return(res);

}

SEXP SymDiffGpcGEOS(SEXP env, SEXP A, SEXP B) {

    GEOSGeom GCA, GCB, GCC;
    SEXP res;
    int pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    GCA = GCPPtsGC(env, A);
    GCB = GCPPtsGC(env, B);

    if ((GCC = GEOSSymDifference_r(GEOShandle, GCA, GCB)) == NULL) {
        GEOSGeom_destroy_r(GEOShandle, GCA);
        GEOSGeom_destroy_r(GEOShandle, GCB);
        error("SymDiffGpcGEOS: symmetric difference not created");
    }

    GEOSGeom_destroy_r(GEOShandle, GCA);
    GEOSGeom_destroy_r(GEOShandle, GCB);
    PROTECT(res = GCGCPPts(env, GCC)); pc++;
    GEOSGeom_destroy_r(GEOShandle, GCC);
    UNPROTECT(pc);
    return(res);

}


SEXP DiffGpcGEOS(SEXP env, SEXP A, SEXP B) {

    GEOSGeom GCA, GCB, GCC;
    SEXP res;
    int pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    GCA = GCPPtsGC(env, A);
    GCB = GCPPtsGC(env, B);

    if ((GCC = GEOSDifference_r(GEOShandle, GCA, GCB)) == NULL) {
        GEOSGeom_destroy_r(GEOShandle, GCA);
        GEOSGeom_destroy_r(GEOShandle, GCB);
        error("SymDiffGpcGEOS: symmetric difference not created");
    }

    GEOSGeom_destroy_r(GEOShandle, GCA);
    GEOSGeom_destroy_r(GEOShandle, GCB);
    PROTECT(res = GCGCPPts(env, GCC)); pc++;
    GEOSGeom_destroy_r(GEOShandle, GCC);
    UNPROTECT(pc);
    return(res);

}

SEXP UnaryUnionGpcGEOS(SEXP env, SEXP A) {

    GEOSGeom GCA, GCC;
    SEXP res;
    int pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    GCA = GCPPtsGC(env, A);
    if ((GCC = GEOSUnionCascaded_r(GEOShandle, GCA)) == NULL) {
        error("UnaryUnionGpcGEOS: unary union not created");
    }

    PROTECT(res = GCGCPPts(env, GCC)); pc++;
    UNPROTECT(pc);
    return(res);
}


SEXP checkHolesGPC(SEXP env, SEXP A) {

    GEOSGeom GC;

    GC = GCPPtsGC(env, A);

    return(GC_Contains(env, GC));

}


GEOSGeom GPCptPolygon(SEXP env, SEXP obj) {

    GEOSGeom g1, p1;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    g1 = GPCpt2LinearRing(env, obj);

    if ((p1 = GEOSGeom_createPolygon_r(GEOShandle, g1, NULL,
        (unsigned int) 0)) == NULL) {
        GEOSGeom_destroy_r(GEOShandle, g1);
        error("GPCptPolygon: Polygon not created");
    }

    return(p1);

}

GEOSGeom GPCpt2LinearRing(SEXP env, SEXP obj) {

    GEOSCoordSeq s;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    s = GPCpt2CoordSeq(env, obj);

    GEOSGeom gl;
    if ((gl = GEOSGeom_createLinearRing_r(GEOShandle, s)) == NULL) {
        GEOSGeom_destroy_r(GEOShandle, gl);
        error("GPCpt2LinearRing: linearRing not created");
    }
    if ((int) GEOSisValid_r(GEOShandle, gl) == 1) {
        if (GEOSNormalize_r(GEOShandle, gl) == -1)
            warning("GPCpt2LinearRing: normalization failure");
    } else {
        warning("GPCpt2LinearRing: validity failure");
    }

    return(gl);
}

GEOSCoordSeq GPCpt2CoordSeq(SEXP env, SEXP obj) {

    unsigned int i, n, m;
    n = (unsigned int) length(VECTOR_ELT(obj, 0));
    m = 2;

    GEOSCoordSeq s;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    s = GEOSCoordSeq_create_r(GEOShandle, (n+1), m);

    for(i=0; i<n; i++) {
        if (GEOSCoordSeq_setX_r(GEOShandle, s, i, 
            NUMERIC_POINTER(VECTOR_ELT(obj, 0))[i]) == 0) {
            GEOSCoordSeq_destroy_r(GEOShandle, s);
            error("GPCpt2CoordSeq: X not set for %d", i);
        }
        if (GEOSCoordSeq_setY_r(GEOShandle, s, i, 
            NUMERIC_POINTER(VECTOR_ELT(obj, 1))[i]) == 0) {
            GEOSCoordSeq_destroy_r(GEOShandle, s);
            error("GPCpt2CoordSeq: Y not set for %d", i);
        }
    }
// enforce ring closure
    if (GEOSCoordSeq_setX_r(GEOShandle, s, n, 
        NUMERIC_POINTER(VECTOR_ELT(obj, 0))[0]) == 0) {
        GEOSCoordSeq_destroy_r(GEOShandle, s);
        error("GPCpt2CoordSeq: X not set for %d", i);
    }
    if (GEOSCoordSeq_setY_r(GEOShandle, s, n, 
        NUMERIC_POINTER(VECTOR_ELT(obj, 1))[0]) == 0) {
        GEOSCoordSeq_destroy_r(GEOShandle, s);
        error("GPCpt2CoordSeq: Y not set for %d", i);
    }

    return(s);

}


GEOSGeom GCPPtsGC(SEXP env, SEXP pls) {

    SEXP comm;
    GEOSGeom *geoms;
    GEOSGeom Pol, GC;
    int npls, i, pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    npls = length(pls);
    PROTECT(comm = SP_PREFIX(comment2comm)(pls)); pc++;

    if (comm == R_NilValue) {
        geoms = (GEOSGeom *) R_alloc((size_t) npls, sizeof(GEOSGeom));

        for (i=0; i<npls; i++) {
            Pol = GPCptPolygon(env, VECTOR_ELT(pls, i));
            geoms[i] = Pol;
        }
        if ((GC = GEOSGeom_createCollection_r(GEOShandle, GEOS_MULTIPOLYGON,
            geoms, npls)) == NULL) {
            error("GCPPtsGC: collection not created");
        }
    } else {

        int nErings = length(comm);
        geoms = (GEOSGeom *) R_alloc((size_t) nErings, sizeof(GEOSGeom));
        for (i=0; i<nErings; i++) {
            Pol = GPCpt_i_Polygon(env, pls, VECTOR_ELT(comm, i));
            geoms[i] = Pol;
        }
        if ((GC = GEOSGeom_createCollection_r(GEOShandle, GEOS_MULTIPOLYGON,
            geoms, nErings)) == NULL) {
            error("GCPPtsGC: collection not created");
        }
    }

    UNPROTECT(pc);
    return(GC);

}

GEOSGeom GPCpt_i_Polygon(SEXP env, SEXP pls, SEXP vec) {

    GEOSGeom res, pol, hole;
    GEOSGeom *holes;

    int n = length(vec);
    int i, j;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    i = INTEGER_POINTER(vec)[0]-R_OFFSET;

    pol = GPCpt2LinearRing(env, VECTOR_ELT(pls, i));
    if (n == 1) {
        if ((res = GEOSGeom_createPolygon_r(GEOShandle, pol, NULL,
            (unsigned int) 0)) == NULL) {
            GEOSGeom_destroy_r(GEOShandle, pol);
            error("GPCpt_i_Polygon: Polygon not created");
        }
    } else {
        holes = (GEOSGeom *) R_alloc((size_t) (n-1), sizeof(GEOSGeom));
        for (j=1; j<n; j++) {
            i = INTEGER_POINTER(vec)[j]-R_OFFSET;
            hole = GPCpt2LinearRing(env, VECTOR_ELT(pls, i));
            holes[(j-1)] = hole;
        }
        if ((res = GEOSGeom_createPolygon_r(GEOShandle, pol, holes,
            (unsigned int) (n-1))) == NULL) {
            GEOSGeom_destroy_r(GEOShandle, pol);
            error("GPCpt_i_Polygon: Polygon not created");
        }
    }
    return(res);
}

SEXP GCGCPPts(SEXP env, GEOSGeom Geom) {
    SEXP res, comment;
    int empty, pc=0;
    int n, nn, nr, i, j, k, jj;
    int *comm;
    GEOSGeom GC, lr;
    char buf[BUFSIZ];

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    empty = (int) GEOSisEmpty_r(GEOShandle, Geom);
    if (empty == 2) error("GCGCPPts: empty error");
    if (empty == 1) {
        PROTECT(res = NEW_LIST(0)); pc++;
    } else if (GEOSGeomTypeId_r(GEOShandle, Geom) == GEOS_POLYGON) {
        n = GEOSGetNumInteriorRings_r(GEOShandle, Geom) + 1;
        PROTECT(res = NEW_LIST(n)); pc++;

        if ((lr = (GEOSGeometry *) GEOSGetExteriorRing_r(GEOShandle, Geom))
            == NULL) error("GCGCPPts: exterior ring failure");
        comm = (int *) R_alloc((size_t) n, sizeof(int));

        SET_VECTOR_ELT(res, 0, rgeos_LinearRingGCPPts(env, lr, FALSE));

        comm[0] = 0;

        for (i=1; i<n; i++) {
            if ((lr = (GEOSGeometry *) GEOSGetInteriorRingN_r(GEOShandle,
                 Geom, (int) (i-1))) == NULL)
                    error("GCGCPPts: interior ring failure");
            comm[i] = 1;
            SET_VECTOR_ELT(res, i, rgeos_LinearRingGCPPts(env, lr, TRUE));
        }

    } else if (GEOSGeomTypeId_r(GEOShandle, Geom) == GEOS_MULTIPOLYGON) {
        nn = GEOSGetNumGeometries_r(GEOShandle, Geom);

        for (i=0, n=0; i<nn; i++) {
            n += GEOSGetNumInteriorRings_r(GEOShandle, (GEOSGeometry *) 
                GEOSGetGeometryN_r(GEOShandle, Geom, i)) + 1;
        }

        PROTECT(res = NEW_LIST(n)); pc++;
        comm = (int *) R_alloc((size_t) n, sizeof(int));

        for (i=0, j=0; i<nn; i++) {

            GC = (GEOSGeometry *) GEOSGetGeometryN_r(GEOShandle, Geom, i);

            if ((lr = (GEOSGeometry *) GEOSGetExteriorRing_r(GEOShandle, GC))
                == NULL) error("GCGCPPts: exterior ring failure");

            SET_VECTOR_ELT(res, j, rgeos_LinearRingGCPPts(env, lr, FALSE));

            comm[j] = 0;
            jj = j;
            j++;
            
            nr = GEOSGetNumInteriorRings_r(GEOShandle, GC);
            for (k=0; k<nr; k++) {
                if ((lr = (GEOSGeometry *) GEOSGetInteriorRingN_r(GEOShandle,
                     GC, (int) i)) == NULL)
                     error("GCGCPPts: interior ring failure");
                comm[j] = jj;
                SET_VECTOR_ELT(res, j, rgeos_LinearRingGCPPts(env, lr, TRUE));
                j++;
            }
        }
    }

    SP_PREFIX(comm2comment)(buf, comm, n);
    PROTECT(comment = NEW_CHARACTER(1)); pc++;
    SET_STRING_ELT(comment, 0, COPY_TO_USER_STRING(buf));

    setAttrib(res, install("comment"), comment);

    UNPROTECT(pc);
    return(res);

}

SEXP rgeos_LinearRingGCPPts(SEXP env, GEOSGeom lr, int hole) {
    GEOSCoordSeq s;
    unsigned int n;
    SEXP res, nms;
    double val;
    int i, pc=0;

    GEOSContextHandle_t GEOShandle = getContextHandle(env);

    if ((s = (GEOSCoordSequence *) GEOSGeom_getCoordSeq_r(GEOShandle, lr))
        == NULL) error("rgeos_LinearRingGCPPts: CoordSeq failure");

    GEOSCoordSeq_getSize_r(GEOShandle, s, &n);

    PROTECT(res = NEW_LIST(3)); pc++;
    SET_VECTOR_ELT(res, 0, NEW_NUMERIC(n));
    SET_VECTOR_ELT(res, 1, NEW_NUMERIC(n));
    SET_VECTOR_ELT(res, 2, NEW_LOGICAL(1));
    PROTECT(nms = NEW_CHARACTER(3)); pc++;
    SET_STRING_ELT(nms, 0, COPY_TO_USER_STRING("x"));
    SET_STRING_ELT(nms, 1, COPY_TO_USER_STRING("y"));
    SET_STRING_ELT(nms, 2, COPY_TO_USER_STRING("hole"));
    setAttrib(res, R_NamesSymbol, nms);

    LOGICAL_POINTER(VECTOR_ELT(res, 2))[0] = hole;

    for (i=0; i<n; i++) {
        GEOSCoordSeq_getX_r(GEOShandle, s, i, &val);
        NUMERIC_POINTER(VECTOR_ELT(res, 0))[i] = val;
        GEOSCoordSeq_getY_r(GEOShandle, s, i, &val);
        NUMERIC_POINTER(VECTOR_ELT(res, 1))[i] = val;
    }
    
    GEOSCoordSeq_destroy_r(GEOShandle,s); 
    
    UNPROTECT(pc);
    return(res);
}


