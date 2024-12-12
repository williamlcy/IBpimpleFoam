IOdictionary solidDict
        (
                IOobject(
                        "solidProperties",
                        runTime.constant(),
                        mesh,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE
                )
        );
Foam::dictionary testDict = solidDict.subDict("meta");
word solidName, outputName;
bool on_2d;
testDict.lookup("solidName") >> solidName;
testDict.lookup("outputName") >> outputName;
testDict.lookup("on_twod") >> on_2d;
List<vector> points;
List<List<label> > connectivity;
List<scalar> cellData;
readVTKfile(solidName, points, connectivity, cellData);
List<vector> solidPoints;
List<List<label> > solidConnectivity;
extractVTKsurface(points, connectivity, cellData, solidPoints, solidConnectivity);
vector internalPoint = getInternalPoint(solidPoints);

std::unordered_map<int, Foam::List<List<label> > > valueToTriplets;
for (const auto &triplet: solidConnectivity) {
for (int value: triplet) {
valueToTriplets[value].push_back(triplet);
}
}
List<vector> solidPointNormals, solidPointTangential;
List<scalar> DS;
getNormal(solidPoints, valueToTriplets, internalPoint, solidPointNormals, solidPointTangential, DS);

writeVTKpolydata(outputName, solidPoints, solidConnectivity);
if (on_2d) {

const fvBoundaryMesh &boundaryMesh = mesh.boundary();
vector normal = {0, 0, 0};
vector facepoint = {0, 0, 0};
// 遍历所有边界
for (label i = 0; i < boundaryMesh.size(); ++i) {
const fvPatch &patch = boundaryMesh[i];
if (patch.type() == "empty") {
const polyPatch &p = patch.patch();
vector A, B, C;
if (pickNonCollinearNonAlignedPoints(p, A, B, C)) {
normal = planeNormal(A, B, C);
facepoint = A;
} else {
Info << "Warning: Could not find suitable points on empty boundary" << endl;
}
}
}
vectorList solidPointNew;
vectorList solidPointNormalNew, solidPointTangentialNew;
scalarList DSNew;

for (const auto &solidPoint: solidPoints) {
solidPointNew.push_back(projectPoint2Plane(solidPoint, normal, facepoint));
}
removeDuplicates(
        solidPointNew, solidPointNormalNew, solidPointTangentialNew, DSNew,
        solidPoints, solidPointNormals, solidPointTangential, DS
);
}