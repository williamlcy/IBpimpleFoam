# IBpimpleFoam

This code develops a ***three-dimensional incompressible immersed boundary method solver*** based on pimpleFoam combined with ***direct-momentum forced immersed boundary***
***

# Installation
Requirement: 
- OpenFOAM v2312

*Step 1*, install [OpenFOAMv2312](https://www.openfoam.com/news/main-news/openfoam-v2312 "OpenFOAMv2312"). **Test** make sure it can run in both serial and parallel mode.


*Step 2*, follow the code
```bash
git clone https://github.com/williamlcy/IBpimpleFoam.git

cd IBpimpleFoam/solver

wmake
```
# Included tutorials

- 2D flow past cylinder
  ![cylinder2d](./fig/cylinder2d.gif)
- 3D flow past cylinder
  ![cylinder3d](./fig/cylinder3d.gif)
