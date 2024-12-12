const Foam::vectorField &meshCenters = mesh.cellCentres();
scalar epsilon = 0.0001;
computeHForCells(mesh, H);
labelListList meshSupport;
scalarListList Dirac;
meshSupport.resize(solidPoints.size());
Dirac.resize(solidPoints.size());
for (int i = 0; i < solidPoints.size(); i++) {
scalar X = solidPoints[i].x();
scalar Y = solidPoints[i].y();
scalar Z = solidPoints[i].z();
for (int j = 0; j < meshCenters.size(); j++) {
scalar rx = fabs(meshCenters[j].x() - X) / H[j].x();
scalar ry = fabs(meshCenters[j].y() - Y) / H[j].y();
scalar rz = fabs(meshCenters[j].z() - Z) / H[j].z();
scalar rxc = rx - 1.5;
scalar ryc = ry - 1.5;
scalar rzc = rz - 1.5;
if ((rxc < epsilon) && (ryc < epsilon) && (rzc < epsilon)) {
meshSupport[i].push_back(j);
Dirac[i].push_back(Delta(rx) * Delta(ry) * Delta(rz) / (H[j].x() * H[j].y() * H[j].z()));
}
}
}