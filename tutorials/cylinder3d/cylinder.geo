Point(1) = {0, 0, -2, 1};
//+
Point(2) = {0.707, 0.707, -2, 1.0};
//+
Point(3) = {-0.707, -0.707, -2, 1.0};
//+
Point(4) = {0.707, -0.707, -2, 1.0};
//+
Point(5) = {-0.707, 0.707, -2, 1.0};
//+
Point(6) = {0.3535, 0.3535, -2, 1.0};
//+
Point(7) = {-0.3535, -0.3535, -2, 1.0};
//+
Point(8) = {0.3535, -0.3535, -2, 1.0};
//+
Point(9) = {-0.3535, 0.3535, -2, 1.0};


//+
Line(1) = {9, 6};
//+
Line(2) = {6, 8};
//+
Line(3) = {8, 7};
//+
Line(4) = {7, 9};
//+
Circle(5) = {5, 1, 3};
//+
Circle(6) = {3, 1, 4};
//+
Circle(7) = {4, 1, 2};
//+
Circle(8) = {2, 1, 5};
//+
Line(9) = {5, 9};
//+
Line(10) = {6, 2};
//+
Line(11) = {8, 4};
//+
Line(12) = {7, 3};
//+
Curve Loop(1) = {1, 2, 3, 4};
//+
Plane Surface(1) = {1};
//+
Curve Loop(2) = {10, 8, 9, 1};
//+
Plane Surface(2) = {2};
//+
Curve Loop(3) = {5, -12, 4, -9};
//+
Plane Surface(3) = {3};
//+
Curve Loop(4) = {3, 12, 6, -11};
//+
Plane Surface(4) = {4};
//+
Curve Loop(5) = {7, -10, 2, 11};
//+
Plane Surface(5) = {5};
//+
Transfinite Curve {12, 11, 10, 9} = 10 Using Progression 1;
//+
Transfinite Curve {7, 2, 4, 5, 1, 8, 6, 3} = 15 Using Progression 1;
//+
Transfinite Surface {3} = {5, 9, 7, 3};
//+
Transfinite Surface {4} = {7, 3, 4, 8};
//+
Transfinite Surface {5} = {8, 4, 2, 6};
//+
Transfinite Surface {2} = {2, 6, 9, 5};
//+
Transfinite Surface {1} = {6, 8, 7, 9};
//+
Recombine Surface {5, 4, 1, 3, 2};
//+
Extrude {0, 0, 4.0} {
  Surface{2}; Surface{5}; Surface{4}; Surface{1}; Surface{3}; Layers {10}; Recombine;
}
//+
Physical Surface("top", 123) = {34, 56, 100, 122, 78};
//+
Physical Surface("bottom", 124) = {4, 5, 2, 3, 1};
//+
Physical Surface("cylinder", 125) = {43, 73, 109, 25};

Mesh.SaveAll = 1;

