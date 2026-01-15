height = 3;


module screw_place(x,y,z) {
  translate([x,y,z])
  difference() {
    cylinder(h=height,r=4.0);
    cylinder(h=height,r=1.2);
   # cylinder(h=1,r1=2,r2=1,center=true);
    translate([0,0,z + height])
   # cylinder(h=1,r1=1,r2=2,center=true);
  }
}



translate([-41, -36, 0])
linear_extrude(height) {
    import("SRSE2-User_Eco1.svg");
}

screw_place(-33,20,0);
screw_place(33,20,0);
screw_place(-33,-20,0);
screw_place(33,-20,0);


$fn=90;