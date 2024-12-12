#pragma once

#include "unordered_map"
#include "fvCFD.H"

struct VectorComparator {
    bool operator()(const vector &v1, const vector &v2) const {
        // 设置浮点数比较的精度
        scalar epsilon = 1e-6;
        if (mag(v1.x() - v2.x()) > epsilon) return v1.x() < v2.x();
        if (mag(v1.y() - v2.y()) > epsilon) return v1.y() < v2.y();
        if (mag(v1.z() - v2.z()) > epsilon) return v1.z() < v2.z();
        return false;
    }
};

inline vector planeNormal(const vector &a, const vector &b, const vector &c, const vector &internal_point) {
    vector v1 = b - a;
    vector v2 = c - a;
    vector normal = v1 ^ v2;
    if (dot(normal, (a - internal_point)) > 0) {
        return normal.normalise();
    } else {
        return -normal.normalise();
    }
}

inline bool isCollinear(const vector &a, const vector &b, const vector &c) {
    vector v1 = b - a;
    vector v2 = c - a;
    vector normal = v1 ^ v2;
    return normal.mag() < 1e-6;
}

inline bool nearlyEqual(double a, double b, double epsilon = 1e-9) {
    return std::fabs(a - b) <= epsilon;
}

bool areAligned(const vector &A, const vector &B, const vector &C, double epsilon = 1e-9) {
    bool sameX = nearlyEqual(A.x(), B.x(), epsilon) && nearlyEqual(B.x(), C.x(), epsilon);
    bool sameY = nearlyEqual(A.y(), B.y(), epsilon) && nearlyEqual(B.y(), C.y(), epsilon);
    bool sameZ = nearlyEqual(A.z(), B.z(), epsilon) && nearlyEqual(B.z(), C.z(), epsilon);

    return (sameX || sameY || sameZ);
}

inline vector planeNormal(const vector &a, const vector &b, const vector &c) {
    vector v1 = b - a;
    vector v2 = c - a;
    vector normal = v1 ^ v2;
    if (isCollinear(a, b, c))
        FatalErrorIn("planeNormal") << "三点共线！" << exit(FatalError);
    return normal.normalise();
}

bool pickNonCollinearNonAlignedPoints(
        const polyPatch &p,
        vector &A,
        vector &B,
        vector &C
) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, p.size() - 1); // 注意 p.size() - 1

    for (int attempt = 0; attempt < 10000; ++attempt) {
        int index1 = dist(gen);
        int index2 = dist(gen);
        int index3 = dist(gen);

        A = p.faceCentres()[index1];
        B = p.faceCentres()[index2];
        C = p.faceCentres()[index3];

        if (areAligned(A, B, C) && !isCollinear(A, B, C)) {
            return true;
        }
    }
    return false; // 未找到满足条件的三点
}

void removeDuplicates(
        vectorList &origPoints,
        vectorList &origNormals,
        vectorList &origTangential,
        scalarList &origDS,
        vectorList &outPoints,
        vectorList &outNormals,
        vectorList &outTangential,
        scalarList &outDS
) {
    std::map<vector, int, VectorComparator> uniquePoints;
    for (int i = 0; i < origPoints.size(); ++i) {
        uniquePoints[origPoints[i]] = i;
    }
    origPoints.clear();
    for (const auto &pair : uniquePoints) {
        origPoints.push_back(pair.first);
        origNormals.push_back(outNormals[pair.second]);
        origTangential.push_back(outTangential[pair.second]);
        origDS.push_back(outDS[pair.second]);
    }
    outPoints.clear();
    outDS.clear();
    outTangential.clear();
    outNormals.clear();
    for (int i = 0 ;i< origPoints.size();i++) {
        outPoints.push_back(origPoints[i]);
        outNormals.push_back(origNormals[i]);
        outTangential.push_back(origTangential[i]);
        outDS.push_back(origDS[i]);
    }
}
void computeHForCells(const fvMesh& mesh, vectorList& H){
    const Foam::vectorField &meshCenters = mesh.cellCentres();
    const labelListList &c2p = mesh.cellPoints();
    const pointField &meshPoints = mesh.points();
    for (int i = 0; i < meshCenters.size(); i++) {
        const labelList &cellpointsindex = c2p[i];
        scalar hx, hy, hz;
        hx = hy = hz = 0;
        for (int j = 0; j < cellpointsindex.size(); j++) {
            vector temp = meshPoints[cellpointsindex[j]] - meshPoints[cellpointsindex[0]];
            if (hx < fabs(temp.x())) {
                hx = fabs(temp.x());
            }
            if (hy < fabs(temp.y())) {
                hy = fabs(temp.y());
            }
            if (hz < fabs(temp.z())) {
                hz = fabs(temp.z());
            }
        }
        H[i].x() = hx;
        H[i].y() = hy;
        H[i].z() = hz;
    }
}
inline Foam::List<List<label>>
findSurfaceofPoint(std::unordered_map<int, Foam::List<List<label>>> map, const int target) {
    Foam::List<List<label>> result;
    if (map.find(target) != map.end()) {
        for (const auto &triplet: map[target]) {
            List<label> temp;
            for (int value: triplet) {
                temp.append(value);
            }
            result.append(temp);
        }
    } else {
        cout << "没有包含 " << target << " 的三元组。\n";
    }
    return result;

}

inline vector getInternalPoint(List<vector> &solidPoints) {
    vector internalPoint = {0, 0, 0};
    for (const auto &solidPoint: solidPoints) {
        internalPoint += solidPoint;
    }
    internalPoint /= solidPoints.size();
    return internalPoint;
}

inline bool isPointInPlane(const vector &point, const vector &normal, const vector &a) {
    return std::abs(dot(point - a, normal)) < 1e-6;
}

inline scalar calArea3Point(const vector &a, const vector &b, const vector &c) {
    vector v1 = b - a;
    vector v2 = c - a;
    return 0.5 * mag(v1 ^ v2);
}

inline scalar calArea4Point(const vector &a, const vector &b, const vector &c, const vector &d) {

    return calArea3Point(a, b, c) + calArea3Point(a, c, d);
}

vector projectPoint2Plane(vector point, vector planeNormal, vector planePoint) {
    vector v = point - planePoint;
    scalar dist = dot(v, planeNormal);

    return point - dist * planeNormal;
}

inline void getNormal(vectorList &solidPoints, std::unordered_map<int, Foam::List<List<label>>> &valueToTriplets,
                      vector &internalPoint, vectorList &solidPointNormals, vectorList &solidPointTangential,
                      List<scalar> &DS) {
    solidPointNormals.resize(solidPoints.size());
    solidPointTangential.resize(solidPoints.size());
    DS.resize(solidPoints.size());
    for (int i = 0; i < solidPoints.size(); i++) {
        vector normal = {0, 0, 0};
        scalar DS_temp = 0;
        List<List<label> > faces = findSurfaceofPoint(valueToTriplets, i);
        for (auto face: faces) {
            normal += planeNormal(solidPoints[face[0]], solidPoints[face[1]], solidPoints[face[2]], internalPoint);
            DS_temp += calArea4Point(solidPoints[face[0]], solidPoints[face[1]], solidPoints[face[2]],
                                     solidPoints[face[3]]);
        }
        solidPointNormals[i] = normal / faces.size();
        DS[i] = DS_temp / faces.size();

        if (std::abs(dot(solidPointNormals[i], vector(0, 0, 1))) < 0.1) {
            solidPointTangential[i] = solidPointNormals[i] ^ vector(0, 0, 1);
        } else if (std::abs(dot(solidPointNormals[i], vector(0, 1, 0))) < 0.1) {
            solidPointTangential[i] = solidPointNormals[i] ^ vector(0, 1, 0);
        } else {
            solidPointTangential[i] = solidPointNormals[i] ^ vector(1, 0, 0);
        }
        solidPointTangential[i] = solidPointTangential[i].normalise();

    }
}