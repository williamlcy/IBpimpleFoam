#pragma once
#include "iostream"
#include "fvCFD.H"
#include "set"

inline void readVTKfile(const word& filename, Foam::List<vector>& points, Foam::List<List<label>>& connectivity, Foam::List<scalar>& cellData)
{
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        FatalErrorIn("readVTKfile") << "Failed to open file: " << filename << exit(FatalError);

    }
    std::string line;

    bool readingPoints = false;
    bool readingCell = false;
    bool readingCellData = false;
    int npoint;
    int ncells;
    int ncellData;
    int points_iter = 0;
    int cellss_iter = 0;
    int cellData_iter = 0;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        // 跳过注释行
        if (line.find('#') == 0)
            continue;
        if (line.find("POINTS") != std::string::npos) {
            readingPoints = true;
            std::string type;
            iss >> type >> npoint;  // 读取 "POINTS <numPoints>"
            points.resize(npoint);
            continue;
        }
        // 解析 POLYGONS 块
        if (line.find("CELLS") != std::string::npos) {
            readingCell = true;
            int nSum;
            std::string test;
            iss >> test >> ncells >> nSum;
            connectivity.resize(ncells);
            continue;
        }
        if (line.find("CELL_DATA") != std::string::npos) {
            readingCellData = true;
            std::string test;
            iss >> test >> ncellData;
            cellData.resize(ncellData);
            continue;
        }

        if (readingPoints) {
            if(points_iter == npoint)
            {
                readingPoints = false;
                continue;
            }
            double x, y, z;
            iss >> x >> y >> z;
            points[points_iter] = vector(x, y, z);
            points_iter++;
        }
        if (readingCell) {

            if(cellss_iter == ncells)
            {
                readingCell = false;
                continue;
            }
            int n;
            iss >> n;
            connectivity[cellss_iter].resize(n);
            for (int j = 0; j < n; j++) {
                iss >> connectivity[cellss_iter][j];
            }
            cellss_iter++;

        }
        if(readingCellData)
        {
            if(cellData_iter == ncellData)
            {
                readingCellData = false;
                continue;
            }
            if (line.find("SCALARS") != string::npos || line.find("LOOKUP_TABLE") != string::npos) {
                Info << "Skipping: " << line << endl;
                continue;  // 跳过当前循环的这行
            }
            int n;
            iss >> n;
            cellData[cellData_iter] = n;
            cellData_iter++;
        }
    }
    inputFile.close();
}

inline void extractVTKsurface(Foam::List<vector> &points, Foam::List<List<label>> &connectivity, Foam::List<scalar> &cellData,
                              Foam::List<vector> &new_points, Foam::List<List<label>> &new_connectivity) {
    Foam::List<List<label>> old_connectivity;
    for (int i = 0; i < cellData.size(); i++) {
        if (cellData[i] != -1) {
            old_connectivity.append(connectivity[i]);
        }
    }
    std::set<int> used_points;
    for (const auto &face: old_connectivity) {
        for (int idx: face) {
            used_points.insert(idx);
        }
    }

    std::map<int, int> old_to_new_index;
    int new_idx = 0;
    for (int old_idx: used_points) {
        old_to_new_index[old_idx] = new_idx++;
        new_points.push_back(points[old_idx]);
    }

    for (int i = 0; i < old_connectivity.size(); i++) {
        List<label> temp;
        for (int j = 0; j < old_connectivity[i].size(); j++) {
            temp.append(old_to_new_index[old_connectivity[i][j]]);
        }
        new_connectivity.append(temp);
    }

}

inline void writeVTKpolydata(const word& filename, Foam::List<vector>& points, Foam::List<List<label>>& connectivity)
{
    std::ofstream vtkFile(filename);
    if (!vtkFile.is_open()) {
        FatalErrorIn("writeVTKfile") << "无法打开文件！" << exit(FatalError);
    }
    vtkFile << "# vtk DataFile Version 5.1" << std::endl;
    vtkFile << "vtk output" << std::endl;
    vtkFile << "ASCII" << std::endl;
    vtkFile << "DATASET POLYDATA" << std::endl;

    vtkFile << "POINTS " << points.size() << " float" << std::endl;
    for(int i = 0 ;i< points.size();i++)
    {
        vtkFile << points[i].x() << " " << points[i].y() << " " << points[i].z() << std::endl;
    }
    vtkFile << "POLYGONS " << connectivity.size()+1 << " " << 4 * connectivity.size() << std::endl;
    vtkFile << "OFFSETS vtktypeint64" << std::endl;
    for (int i = 0;i<=connectivity.size();i++)
    {

        vtkFile << 4 * i << " ";
        if (i % 8 == 0 and i != 0 ) {
            vtkFile << std::endl;
        }
    }
    vtkFile << std::endl;
    vtkFile << "CONNECTIVITY vtktypeint64" << std::endl;

    for (int i = 0;i<connectivity.size();i++)
    {
        for(int j = 0 ;j<connectivity[i].size();j++)
        {
            vtkFile << connectivity[i][j] << " ";
        }
        vtkFile << std::endl;
    }

    vtkFile.close();
}

inline void writeVTKpolydata(const word& filename, Foam::List<vector>& points, Foam::List<List<label>>& connectivity, Foam::List<vector>& polydata)
{
    std::ofstream vtkFile(filename);
    if (!vtkFile.is_open()) {
        FatalErrorIn("writeVTKfile") << "无法打开文件！" << exit(FatalError);
    }
    vtkFile << "# vtk DataFile Version 5.1" << std::endl;
    vtkFile << "vtk output" << std::endl;
    vtkFile << "ASCII" << std::endl;
    vtkFile << "DATASET POLYDATA" << std::endl;

    vtkFile << "POINTS " << points.size() << " float" << std::endl;
    for(int i = 0 ;i< points.size();i++)
    {
        vtkFile << points[i].x() << " " << points[i].y() << " " << points[i].z() << std::endl;
    }
    vtkFile << "POLYGONS " << connectivity.size()+1 << " " << 4 * connectivity.size() << std::endl;
    vtkFile << "OFFSETS vtktypeint64" << std::endl;
    for (int i = 0;i<=connectivity.size();i++)
    {

        vtkFile << 4 * i << " ";
        if (i % 8 == 0 and i != 0 ) {
            vtkFile << std::endl;
        }
    }
    vtkFile << std::endl;
    vtkFile << "CONNECTIVITY vtktypeint64" << std::endl;

    for (int i = 0;i<connectivity.size();i++)
    {
        for(int j = 0 ;j<connectivity[i].size();j++)
        {
            vtkFile << connectivity[i][j] << " ";
        }
        vtkFile << std::endl;
    }
    vtkFile << "POINT_DATA " << polydata.size() << std::endl;
    vtkFile << "NORMALS "<< "normals" << " float" << std::endl;
    for(auto & solidPointNormal : polydata)
    {
        vtkFile << solidPointNormal.x() << " " << solidPointNormal.y() << " " << solidPointNormal.z() << std::endl;
    }

    vtkFile.close();
}

inline void writeVTKpolydata(const word& filename, Foam::List<vector>& points, Foam::List<List<label>>& connectivity, Foam::List<scalar>& polydata)
{
    std::ofstream vtkFile(filename);
    if (!vtkFile.is_open()) {
        FatalErrorIn("writeVTKfile") << "无法打开文件！" << exit(FatalError);
    }
    vtkFile << "# vtk DataFile Version 5.1" << std::endl;
    vtkFile << "vtk output" << std::endl;
    vtkFile << "ASCII" << std::endl;
    vtkFile << "DATASET POLYDATA" << std::endl;

    vtkFile << "POINTS " << points.size() << " float" << std::endl;
    for(int i = 0 ;i< points.size();i++)
    {
        vtkFile << points[i].x() << " " << points[i].y() << " " << points[i].z() << std::endl;
    }
    vtkFile << "POLYGONS " << connectivity.size()+1 << " " << 4 * connectivity.size() << std::endl;
    vtkFile << "OFFSETS vtktypeint64" << std::endl;
    for (int i = 0;i<=connectivity.size();i++)
    {

        vtkFile << 4 * i << " ";
        if (i % 8 == 0 and i != 0 ) {
            vtkFile << std::endl;
        }
    }
    vtkFile << std::endl;
    vtkFile << "CONNECTIVITY vtktypeint64" << std::endl;

    for (int i = 0;i<connectivity.size();i++)
    {
        for(int j = 0 ;j<connectivity[i].size();j++)
        {
            vtkFile << connectivity[i][j] << " ";
        }
        vtkFile << std::endl;
    }
    vtkFile << "POINT_DATA " << polydata.size() << std::endl;
    vtkFile << "SCALARS "<< "normals" << " float" << std::endl;
    vtkFile << "LOOKUP_TABLE default" << std::endl;
    for(double i : polydata)
    {
        vtkFile << i << std::endl;
    }
    vtkFile.close();
}