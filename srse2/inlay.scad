module screw_place(x,y,z) {
  translate([x,y,z])
  difference() {
    cylinder(h=3,r=4.8);
    cylinder(h=3,r=1.2);
   # cylinder(h=1,r1=2,r2=1,center=true);
    translate([0,0,z + 3])
   # cylinder(h=1,r1=1,r2=2,center=true);
  }
}


translate([-35,-18,0])
    cube([4,35,3]);

translate([31,-18,0]) 
    cube([4,35,3]);

translate([-28,-20,0]) {
    rotate([0,0,45])
    cube([15,4,3]);
}

rotate([0,0,45]) {
    translate([20,-11.5,0])
    cube([15,4,3]);
}

rotate([0,0,-45]) {
    translate([-33,-11.5,0])
    cube([15,4,3]);
}

rotate([0,0,-45]) {
    translate([20,7,0])
    cube([15,4,3]);
}

translate([-22,-10,0])
    cube([4,20,3]);

translate([19,-10,0])
    cube([4,20,3]);


screw_place(-33,20,0);
screw_place(33,20,0);
screw_place(-33,-20,0);
screw_place(33,-20,0);

translate([-20,-5,0])
cube([40,5,3]);

$fn=90;