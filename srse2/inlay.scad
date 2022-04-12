module screw_place() {
  difference() {
    cylinder(h=3,r=5);
    cylinder(h=3,r=1);
  }
}


translate([-35,-18,0])
    cube([4,35,2]);

translate([31,-18,0])
    cube([4,35,2]);

translate([-28,-20,0]) {
    rotate(45,0,0)
    cube([15,4,3]);
}

rotate(45,0,0) {
    translate([20,-11.5,0])
    cube([15,4,3]);
}

rotate(-45,0,0) {
    translate([-33,-11.5,0])
    cube([15,4,3]);
}

rotate(-45,0,0)
    translate([20,7,0])
    cube([15,4,3]);

translate([-22,-10,0])
    cube([4,20,3]);

translate([19,-10,0])
    cube([4,20,3]);

translate([-33,20,0])
screw_place();

translate([33,20,0])
screw_place();

translate([-33,-20,0])
screw_place();

translate([33,-20,0])
screw_place();

translate([-20,-5,0])
cube([40,5,3]);