// 初始化 vectorList
vectorList U_Interpolation(solidPoints.size(), vector(0, 0, 0));
vectorList U_Structure(solidPoints.size(), vector(0, 0, 0));
vectorList F_Structure(solidPoints.size(), vector(0, 0, 0));
vectorList F_Fluid_collect(meshCenters.size(), vector(0, 0, 0));

// 计算 U_Interpolation

for (int i = 0; i < solidPoints.size(); ++i) {
const auto& supportCells = meshSupport[i];
const auto& weights = Dirac[i];
for (int idx = 0; idx < supportCells.size(); ++idx) {
int cellID = supportCells[idx];
vector cellVelocity = U[cellID];
scalar cellVolume = H[cellID].x() * H[cellID].y() * H[cellID].z();
U_Interpolation[i] += cellVelocity * weights[idx] * cellVolume;
}
}

// 计算 F_Structure
Foam::dimensionedScalar dt = runTime.deltaT();
scalar dtt = runTime.deltaT().value();
for (int i = 0; i < solidPoints.size(); ++i) {
F_Structure[i] = (U_Structure[i] - U_Interpolation[i]) / dtt;
}

// 计算 F_Fluid_collect
vector D_S = {0,0,0};
for (int i = 0; i < solidPoints.size(); ++i) {
for (int j = 0; j < meshSupport[i].size(); ++j) {
D_S = H[j] * DS[i];
F_Fluid_collect[meshSupport[i][j]].x() += F_Structure[i].x() * Dirac[i][j] * D_S.x();
F_Fluid_collect[meshSupport[i][j]].y() += F_Structure[i].y() * Dirac[i][j] * D_S.y();
F_Fluid_collect[meshSupport[i][j]].z() += F_Structure[i].z() * Dirac[i][j] * D_S.z();
}
}

// 将 F_Fluid_collect 赋值到 F_Fluid
std::copy(F_Fluid_collect.begin(), F_Fluid_collect.end(), F_Fluid.begin());
