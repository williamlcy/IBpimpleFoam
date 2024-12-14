//+
Point(1) = {0, 0.2, -5, 1.0};
//+
Point(2) = {-10, -10, -5, 1.0};
//+
Point(3) = {10, 10, -5, 1.0};
//+
Point(4) = {-10, 10, -5, 1.0};
//+
Point(5) = {10, -10, -5, 1.0};
//+
Point(6) = {0, -10, -5, 1.0};
//+
Point(7) = {0, 10, -5, 1.0};
//+
Point(8) = {-10, 0.2, -5, 1.0};
//+
Point(9) = {10, 0.2, -5, 1.0};
//+
//+
Line(1) = {4, 8};
//+
Line(2) = {8, 2};
//+
Line(3) = {2, 6};
//+
Line(4) = {6, 5};
//+
Line(5) = {5, 9};
//+
Line(6) = {9, 3};
//+
Line(7) = {3, 7};
//+
Line(8) = {7, 4};
//+
Transfinite Curve {-8} = 50 Using Progression 1;
//+
Transfinite Curve {3} = 50 Using Progression 1;
//+
Transfinite Curve {7, -4} = 50 Using Progression 1;
//+
Transfinite Curve {1, -2, 5, -6} = 50 Using Progression 1;
//+
Curve Loop(1) = {1, 2, 3, 4, 5, 6, 7, 8};
//+
Plane Surface(1) = {1};
//+
Transfinite Surface {1} = {4, 3, 5, 2};
//+
Recombine Surface {1};
//+
Extrude {0, 0, 10} {
  Surface{1}; Layers {50}; Recombine;
}
//+
Physical Surface("inlet", 51) = {21, 25};
//+
Physical Surface("outlet", 52) = {41, 37};
//+
Physical Surface("wall", 53) = {49, 45, 29, 33};
//+
Physical Surface("frontAndBackPlanes", 54) = {50, 1};
//+
Physical Volume("fluid", 55) = {1};
